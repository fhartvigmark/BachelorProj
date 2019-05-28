#include "pivot.h"

int pivot::findPivot(enhancedgraph *g, int color, int method) {
	int retVal;
	TimePoint start = g->startTimer();

	switch (method)
	{
		case 0:
			retVal = pivot::getPivot(g, color);
			break;
		case 1:
			retVal = pivot::getParPivotMaxDegree(g, color, false);
			break;
		case 2:
			retVal = pivot::getParPivotMaxDegreeColor(g, color, false);
			break;
		case 3:
			retVal = pivot::getPivotRand(g, color);
			break;
		case 4:
			retVal = pivot::getParPivotMaxDegreeCalc(g, color, false);
			break;
	}

	g->endTimer(start, eTimer::PIVOT);
	g->reportPivot(color, retVal);
	return retVal;
}

int pivot::findParPivot(enhancedgraph *g, int color, int method) {
	int retVal;
	TimePoint start = g->startTimer();

	switch (method)
	{
		case 0:
			retVal = pivot::getParPivot(g, color, true);
			break;
		case 1:
			retVal = pivot::getParPivotMaxDegree(g, color, true);
			break;
		case 2:
			retVal = pivot::getParPivotMaxDegreeColor(g, color, true);
			break;
		case 3:
			retVal = pivot::getParPivotRand(g, color, true);
			break;
		case 4:
			retVal = pivot::getParPivotMaxDegreeCalc(g, color, true);
			break;
	}

	g->endTimer(start, eTimer::PIVOT);
	g->reportPivot(color, retVal);
	return retVal;
}



//--------------------------------
//		Sequential pivots
//--------------------------------

//Returns the first node matching the search color or -1 if no such node exist
int pivot::getPivot(enhancedgraph *g, int color)
{
    ColorMap *colorMap = g->colors;
	for (int i = colorMap->BegI(); i < colorMap->EndI(); i++) {
		if (colorMap->GetDat(i) == color) {
			return i;
		}
	}

    return -1;
};

//Returns the node with the highest (in * out-degree) 
//matching the search color or -1 if no such node exist
int pivot::getPivotMaxDegree(enhancedgraph *g, int color)
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
    return bestNode;
};

//Returns the node with the highest (in * out-degree) 
//only counting edges going to nodes of the same color
//matching the search color or -1 if no such node exist
int pivot::getPivotMaxDegreeColor(enhancedgraph *g, int color)
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
    return bestNode;
};

//Performs a simple random walk on the output of getPivot()
int pivot::getPivotRand(enhancedgraph *g, int color)
{
	int node = getPivot(g, color);

	if (node == -1) {
		return node;
	}

	return Random::randwalk(g, color, node, g->RAND_WALK_ITERATIONS);
};



//--------------------------------
// 		Parallel pivots
//--------------------------------

//Parallel version of getPivot()
int pivot::getParPivot(enhancedgraph *g, int color, bool parallel)
{
    ColorMap *colorMap = g->colors;
	int retVal = -1;

	#pragma omp parallel for schedule(static) if(parallel)
	for (int i = colorMap->BegI(); i < colorMap->EndI(); i++) {
		if (colorMap->GetDat(i) == color) {

			#pragma omp cancellation point for	
			#pragma omp critical
			{
				retVal = i;
			}
			#pragma omp cancel for
		}
	}

    return retVal;
};

//Parallel version of getPivotMaxDegree()
int pivot::getParPivotMaxDegree(enhancedgraph *g, int color, bool parallel)
{
	ColorMap *colors = g->colors;
    PNGraph graph = g->graph;

	struct Compare max; 
	max.val = -1; 
	max.node = -1;
	#pragma omp parallel for reduction(maximum:max) schedule(static) if(parallel)
	for (int i = colors->BegI(); i < colors->EndI(); i++) {

		const TNGraph::TNodeI NI = graph->GetNI(i);

		if(colors->GetDat(i)==color){
            int newDeg = NI.GetInDeg() * NI.GetOutDeg();

			if(newDeg > max.val) { 
				max.val = newDeg;
				max.node = i;
			}
        }
	}

    return max.node;
};

//Parallel version of getPivotMaxDegree()
int pivot::getParPivotMaxDegreeCalc(enhancedgraph *g, int color, bool parallel)
{
	ColorMap *colors = g->colors;
	ColorMap *degree = g->degree;
    PNGraph graph = g->graph;

	struct Compare max; 
	max.val = -1; 
	max.node = -1;
	#pragma omp parallel for reduction(maximum:max) schedule(static) if(parallel)
	for (int i = degree->BegI(); i < degree->EndI(); i++) {
		if(colors->GetDat(i)==color){
            int newDeg = degree->GetDat(i);

			if(newDeg > max.val) { 
				max.val = newDeg;
				max.node = i;
			}
        }
	}

    return max.node;
};

//Parallel version of getPivotMaxDegreeColor()
int pivot::getParPivotMaxDegreeColor(enhancedgraph *g, int color, bool parallel)
{
	ColorMap *colors = g->colors;
    PNGraph graph = g->graph;

	struct Compare max; 
	max.val = -1; 
	max.node = -1;
	#pragma omp parallel for reduction(maximum:max) schedule(static) if(parallel)
	for (int i = colors->BegI(); i < colors->EndI(); i++) {
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
        }
	}

    return max.node;
};

//Performs a simple random walk on the output of getParPivot()
int pivot::getParPivotRand(enhancedgraph *g, int color, bool parallel)
{
    int node = getParPivot(g, color, parallel);

	if (node == -1) {
		return node;
	}

	return Random::randwalk(g, color, node, g->RAND_WALK_ITERATIONS);
};









