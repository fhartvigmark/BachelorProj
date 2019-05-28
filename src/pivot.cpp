#include "pivot.h"

std::tuple<int, int, int> pivot::findPivot(enhancedgraph *g, int color, int method, int low, int high) {
	std::tuple<int, int, int> retVal;
	TimePoint start = g->startTimer();

	switch (method)
	{
		case 0:
			retVal = pivot::getPivot(g, color, low, high);
			break;
		case 1:
			retVal = pivot::getParPivotMaxDegree(g, color, false, low, high);
			break;
		case 2:
			retVal = pivot::getParPivotMaxDegreeColor(g, color, false, low, high);
			break;
		case 3:
			retVal = pivot::getPivotRand(g, color, low, high);
			break;
		case 4:
			retVal = pivot::getParPivotMaxDegreeCalc(g, color, false, low, high);
			break;
	}

	g->endTimer(start, eTimer::PIVOT);
	g->reportPivot(color, std::get<0>(retVal));
	return retVal;
}

std::tuple<int, int, int> pivot::findParPivot(enhancedgraph *g, int color, int method, int low, int high) {
	std::tuple<int, int, int> retVal;
	TimePoint start = g->startTimer();

	switch (method)
	{
		case 0:
			retVal = pivot::getParPivot(g, color, true, low, high);
			break;
		case 1:
			retVal = pivot::getParPivotMaxDegree(g, color, true, low, high);
			break;
		case 2:
			retVal = pivot::getParPivotMaxDegreeColor(g, color, true, low, high);
			break;
		case 3:
			retVal = pivot::getParPivotRand(g, color, true, low, high);
			break;
		case 4:
			retVal = pivot::getParPivotMaxDegreeCalc(g, color, true, low, high);
			break;
	}

	g->endTimer(start, eTimer::PIVOT);
	g->reportPivot(color, std::get<0>(retVal));
	return retVal;
}



//--------------------------------
//		Sequential pivots
//--------------------------------

//Returns the first node matching the search color or -1 if no such node exist
std::tuple<int, int, int> pivot::getPivot(enhancedgraph *g, int color, int low, int high)
{
    ColorMap *colorMap = g->colors;
	for (int i = low; i < high+1; i++) {
		if (colorMap->GetDat(i) == color) {
			return std::make_tuple(i, i, high);
		}
	}

	return std::make_tuple(-1, -1, high);
};

//Returns the node with the highest (in * out-degree) 
//matching the search color or -1 if no such node exist
std::tuple<int, int, int> pivot::getPivotMaxDegree(enhancedgraph *g, int color, int low, int high)
{
    int bestNode = -1;
    int bestDegree = -1;
    ColorMap *colors = g->colors;
    PNGraph graph = g->graph;

    for (TNGraph::TNodeI NI = graph->BegNI(); NI < graph->EndNI(); NI++)
    {
        if(colors->GetDat(NI.GetId())==color){
            int newDeg = NI.GetInDeg() * NI.GetOutDeg();
            if (newDeg>bestDegree){
                bestNode = NI.GetId();
                bestDegree = newDeg;
            }
        }
    }
    return std::make_tuple(bestNode, low, high);
};

//Returns the node with the highest (in * out-degree) 
//only counting edges going to nodes of the same color
//matching the search color or -1 if no such node exist
std::tuple<int, int, int> pivot::getPivotMaxDegreeColor(enhancedgraph *g, int color, int low, int high)
{
    int bestNode = -1;
    int bestDegree = -1;
    ColorMap *colors = g->colors;
    PNGraph graph = g->graph;
    for (TNGraph::TNodeI NI = graph->BegNI(); NI < graph->EndNI(); NI++)
    {
        int node = NI.GetId();
        if (colors->GetDat(node) == color)
        {
            int inDegree = 0;
            TNGraph::TNodeI NodeI = graph->GetNI(node);
	
            int v = 0;
            for (v = 0; v < NodeI.GetInDeg(); v++)
            {
                const int outNode = NodeI.GetInNId(v);
	
                if (colors->GetDat(outNode) == color && outNode != node)
                {
                    inDegree += 1;
                }
            }
	
            int outDegree = 0;
            NodeI = graph->GetNI(node);
	
            for (v = 0; v < NodeI.GetOutDeg(); v++)
            {
                const int outNode = NodeI.GetOutNId(v);
	
                if (colors->GetDat(outNode) == color && outNode != node)
                {
                    outDegree += 1;
                }
            }
	
            int newDeg = inDegree * outDegree;
            if (newDeg > bestDegree)
            {
                bestNode = NI.GetId();
                bestDegree = newDeg;
            }
        }
    }
    return std::make_tuple(bestNode, low, high);
};

//Performs a simple random walk on the output of getPivot()
std::tuple<int, int, int> pivot::getPivotRand(enhancedgraph *g, int color, int low, int high)
{
	std::tuple<int, int, int> node = getPivot(g, color, low, high);

	if (std::get<0>(node) == -1) {
		return node;
	}

	int bestNode = Random::randwalk(g, color, std::get<0>(node), g->RAND_WALK_ITERATIONS);
    return std::make_tuple(bestNode, std::get<1>(node) , std::get<2>(node));
};



//--------------------------------
// 		Parallel pivots
//--------------------------------

//Parallel version of getPivot()
std::tuple<int, int, int> pivot::getParPivot(enhancedgraph *g, int color, bool parallel, int low, int high)
{
    ColorMap *colorMap = g->colors;
	int retVal = -1;

	#pragma omp parallel for schedule(static) if(parallel)
	for (int i = low; i < high+1; i++) {
		if (colorMap->GetDat(i) == color) {

			#pragma omp cancellation point for	
			#pragma omp critical
			{
				retVal = i;
			}
			#pragma omp cancel for
		}
	}

	return std::make_tuple(retVal, low, high);
};

//Parallel version of getPivotMaxDegree()
std::tuple<int, int, int> pivot::getParPivotMaxDegree(enhancedgraph *g, int color, bool parallel, int low, int high)
{
	ColorMap *colors = g->colors;
    PNGraph graph = g->graph;
	int min_i = high;
	int max_i = low;

	struct Compare max; 
	max.val = -1; 
	max.node = -1;
	#pragma omp parallel for reduction(maximum:max) reduction(min:min_i) reduction(max:max_i) schedule(static) if(parallel)
	for (int i = low; i < high+1; i++) {

		const TNGraph::TNodeI NI = graph->GetNI(i);

		if(colors->GetDat(i)==color){
            int newDeg = NI.GetInDeg() * NI.GetOutDeg();

			if(newDeg > max.val) { 
				max.val = newDeg;
				max.node = i;
			}

			if (i < min_i) {
				min_i = i;
			}
			if (i > max_i) {
				max_i = i;
			}
        }
	}

	return std::make_tuple(max.node, min_i, max_i);
};

//Parallel version of getPivotMaxDegree()
std::tuple<int, int, int> pivot::getParPivotMaxDegreeCalc(enhancedgraph *g, int color, bool parallel, int low, int high)
{
	ColorMap *colors = g->colors;
	ColorMap *degree = g->degree;
    PNGraph graph = g->graph;
	int min_i = high;
	int max_i = low;

	struct Compare max; 
	max.val = -1; 
	max.node = -1;
	#pragma omp parallel for reduction(maximum:max) reduction(min:min_i) reduction(max:max_i) schedule(static) if(parallel)
	for (int i = low; i < high+1; i++) {
		if(colors->GetDat(i)==color){
            int newDeg = degree->GetDat(i);

			if(newDeg > max.val) { 
				max.val = newDeg;
				max.node = i;
			}

			if (i < min_i) {
				min_i = i;
			}
			if (i > max_i) {
				max_i = i;
			}
        }
	}

	return std::make_tuple(max.node, min_i, max_i);
};

//Parallel version of getPivotMaxDegreeColor()
std::tuple<int, int, int> pivot::getParPivotMaxDegreeColor(enhancedgraph *g, int color, bool parallel, int low, int high)
{
	ColorMap *colors = g->colors;
    PNGraph graph = g->graph;
	int min_i = high;
	int max_i = low;

	struct Compare max; 
	max.val = -1; 
	max.node = -1;
	#pragma omp parallel for reduction(maximum:max) reduction(min:min_i) reduction(max:max_i) schedule(static) if(parallel)
	for (int i = low; i < high+1; i++) {
		const TNGraph::TNodeI NI = graph->GetNI(i);

        if (colors->GetDat(i) == color)
        {
            int inDegree = 0;
            TNGraph::TNodeI NodeI = graph->GetNI(i);

            int v = 0;
            for (v = 0; v < NodeI.GetInDeg(); v++)
            {
                const int outNode = NodeI.GetInNId(v);

                if (colors->GetDat(outNode) == color && outNode != i)
                {
                    inDegree += 1;
                }
            }

            int outDegree = 0;
            NodeI = graph->GetNI(i);

            for (v = 0; v < NodeI.GetOutDeg(); v++)
            {
                const int outNode = NodeI.GetOutNId(v);

                if (colors->GetDat(outNode) == color && outNode != i)
                {
                    outDegree += 1;
                }
            }

            int newDeg = inDegree * outDegree;
            if (newDeg > max.val) { 
				max.val = newDeg;
				max.node = i;
			}

			if (i < min_i) {
				min_i = i;
			}
			if (i > max_i) {
				max_i = i;
			}
        }
	}

	return std::make_tuple(max.node, min_i, max_i);    
};

//Performs a simple random walk on the output of getParPivot()
std::tuple<int, int, int> pivot::getParPivotRand(enhancedgraph *g, int color, bool parallel, int low, int high)
{
	std::tuple<int, int, int> node = getParPivot(g, color, parallel, low, high);

	if (std::get<0>(node) == -1) {
		return node;
	}

	int bestNode = Random::randwalk(g, color, std::get<0>(node), g->RAND_WALK_ITERATIONS);
    return std::make_tuple(bestNode, std::get<1>(node) , std::get<2>(node));
};









