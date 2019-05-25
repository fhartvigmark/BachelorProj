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
			retVal = pivot::getPivotMaxDegree(g, color);
			break;
		case 2:
			retVal = pivot::getPivotMaxDegreeColor(g, color);
			break;
		case 3:
			retVal = pivot::getParPivot(g, color);
			break;
		case 4:
			retVal = pivot::getParPivotMaxDegree(g, color);
			break;
		case 5:
			retVal = pivot::getParPivotMaxDegreeColor(g, color);
			break;
		case 6:
			retVal = pivot::getPivotRand(g, color);
			break;
		case 7:
			retVal = pivot::getParPivotRand(g, color);
			break;
	}

	g->endTimer(start, eTimer::PIVOT);
	g->reportPivot(color, retVal);
	return retVal;
}



//--------------------------------
//		Local methods
//--------------------------------

int myRand(unsigned int seed, int limit) {
	return (rand_r(&seed)%limit);
}

int randstep(enhancedgraph *g, int color, int node, unsigned int seed) {
	TIntH *colors = g->colors;
    PNGraph graph = g->graph;

	//Find number of edges of same color
	int edges = 0;

	TNGraph::TNodeI NodeI = graph->GetNI(node);
	int v;
	for (v = 0; v < NodeI.GetOutDeg(); v++)
	{
		const int outNode = NodeI.GetOutNId(v);

		if (colors->GetDat(outNode) == color)
		{
			edges += 1;
		}
	}
	
	NodeI = graph->GetNI(node);
	for (v = 0; v < NodeI.GetInDeg(); v++)
	{
		const int outNode = NodeI.GetInNId(v);

		if (colors->GetDat(outNode) == color)
		{
			edges += 1;
		}
	}
	
	if (edges <= 0) {
		return node;
	}
	int index = myRand(seed, edges);
	edges = 0;

	//Find the edges with chosen index
	NodeI = graph->GetNI(node);
	for (v = 0; v < NodeI.GetOutDeg(); v++)
	{
		const int outNode = NodeI.GetOutNId(v);

		if (colors->GetDat(outNode) == color)
		{
			if (edges == index) {
				return outNode;
			}
			edges += 1;
		}
	}
	
	NodeI = graph->GetNI(node);
	for (v = 0; v < NodeI.GetInDeg(); v++)
	{
		const int outNode = NodeI.GetInNId(v);

		if (colors->GetDat(outNode) == color)
		{
			if (edges == index) {
				return outNode;
			}
			edges += 1;
		}
	}

	return node;
}

//Performs a simple random walk for k iterations starting from the given node
//only looks at in/out edges of same color as start node
int randwalk(enhancedgraph *g, int color, int node, const int k) {
	unsigned int seed = time(NULL);
	int currentNode = node;

	for (int i = 0; i < k; i++) {
		currentNode = randstep(g, color, currentNode, seed);
	}

	return currentNode;
}



//--------------------------------
//		Sequential pivots
//--------------------------------

//Returns the first node matching the search color or -1 if no such node exist
int pivot::getPivot(enhancedgraph *g, int color)
{
    TIntH *colorMap = g->colors;
	TIntV *Ids = g->NIds;
	int retVal = -1;
	for (int i = 0; i < Ids->Len(); i++) {
		if (colorMap->GetDat(Ids->GetVal(i)) == color) {
			retVal = Ids->GetVal(i);
		}
	}
    //for (THashKeyDatI<TInt, TInt> i = colorMap->BegI(); i < colorMap->EndI(); i++)
    //{
    //    if (i.GetDat()==color)
    //    {
    //        return i.GetKey();
    //    }
    //    
    //}
    return -1;
};

//Returns the node with the highest (in * out-degree) 
//matching the search color or -1 if no such node exist
int pivot::getPivotMaxDegree(enhancedgraph *g, int color)
{
    int bestNode = -1;
    int bestDegree = -1;
    TIntH *colors = g->colors;
    PNGraph graph = g->graph;
	TIntV *Ids = g->NIds;
	for (int i = 0; i < Ids->Len(); i++) {
		const TNGraph::TNodeI NI = graph->GetNI(Ids->GetVal(i));

		if(colors->GetDat(NI.GetId())==color){
            int newDeg = NI.GetInDeg() * NI.GetOutDeg();

			if (newDeg>bestDegree){
                bestNode = NI.GetId();
                bestDegree = newDeg;
            }
        }
	}

    //for (TNGraph::TNodeI NI = graph->BegNI(); NI < graph->EndNI(); NI++)
    //{
    //    if(colors->GetDat(NI.GetId())==color){
    //        int newDeg = NI.GetInDeg() * NI.GetOutDeg();
    //        if (newDeg>bestDegree){
    //            bestNode = NI.GetId();
    //            bestDegree = newDeg;
    //        }
    //    }
    //}
    return bestNode;
};

//Returns the node with the highest (in * out-degree) 
//only counting edges going to nodes of the same color
//matching the search color or -1 if no such node exist
int pivot::getPivotMaxDegreeColor(enhancedgraph *g, int color)
{
    int bestNode = -1;
    int bestDegree = -1;
    TIntH *colors = g->colors;
    PNGraph graph = g->graph;
	TIntV *Ids = g->NIds;
	for (int i = 0; i < Ids->Len(); i++) {
		const TNGraph::TNodeI NI = graph->GetNI(Ids->GetVal(i));
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
            if (newDeg > bestDegree) {
                bestNode = NI.GetId();
                bestDegree = newDeg;
            }
        }
	}
    //for (TNGraph::TNodeI NI = graph->BegNI(); NI < graph->EndNI(); NI++)
    //{
    //    int node = NI.GetId();
    //    if (colors->GetDat(node) == color)
    //    {
    //        int inDegree = 0;
    //        TNGraph::TNodeI NodeI = graph->GetNI(node);
	//
    //        int v = 0;
    //        for (v = 0; v < NodeI.GetInDeg(); v++)
    //        {
    //            const int outNode = NodeI.GetInNId(v);
	//
    //            if (colors->GetDat(outNode) == color && outNode != node)
    //            {
    //                inDegree += 1;
    //            }
    //        }
	//
    //        int outDegree = 0;
    //        NodeI = graph->GetNI(node);
	//
    //        for (v = 0; v < NodeI.GetOutDeg(); v++)
    //        {
    //            const int outNode = NodeI.GetOutNId(v);
	//
    //            if (colors->GetDat(outNode) == color && outNode != node)
    //            {
    //                outDegree += 1;
    //            }
    //        }
	//
    //        int newDeg = inDegree * outDegree;
    //        if (newDeg > bestDegree)
    //        {
    //            bestNode = NI.GetId();
    //            bestDegree = newDeg;
    //        }
    //    }
    //}
    return bestNode;
};

//Performs a simple random walk on the output of getPivot()
int pivot::getPivotRand(enhancedgraph *g, int color)
{
	int node = getPivot(g, color);

	if (node == -1) {
		return node;
	}

	return randwalk(g, color, node, g->RAND_WALK_ITERATIONS);
};



//--------------------------------
// 		Parallel pivots
//--------------------------------

//Parallel version of getPivot()
int pivot::getParPivot(enhancedgraph *g, int color)
{
    TIntH *colorMap = g->colors;
	PNGraph graph = g->graph;
	TIntV *Ids = g->NIds;

	int retVal = -1;

	#pragma omp parallel for schedule(static)
	for (int i = 0; i < Ids->Len(); i++) {
		if (colorMap->GetDat(Ids->GetVal(i)) == color) {

			#pragma omp cancellation point for	
			#pragma omp critical
			{
				retVal = Ids->GetVal(i);
			}
			#pragma omp cancel for
		}
	}

    return retVal;
};

//Parallel version of getPivotMaxDegree()
int pivot::getParPivotMaxDegree(enhancedgraph *g, int color)
{
	TIntH *colors = g->colors;
    PNGraph graph = g->graph;
	TIntV *Ids = g->NIds;

	struct Compare max; 
	max.val = -1; 
	max.node = -1;
	#pragma omp parallel for reduction(maximum:max) schedule(static)
	for (int i = 0; i < Ids->Len(); i++) {

		const TNGraph::TNodeI NI = graph->GetNI(Ids->GetVal(i));

		if(colors->GetDat(NI.GetId())==color){
            int newDeg = NI.GetInDeg() * NI.GetOutDeg();

			if(newDeg > max.val) { 
				max.val = newDeg;
				max.node = Ids->GetVal(i);
			}
        }
	}

    return max.node;
};

//Parallel version of getPivotMaxDegreeColor()
int pivot::getParPivotMaxDegreeColor(enhancedgraph *g, int color)
{
	TIntH *colors = g->colors;
    PNGraph graph = g->graph;
	TIntV *Ids = g->NIds;

	struct Compare max; 
	max.val = -1; 
	max.node = -1;
	#pragma omp parallel for reduction(maximum:max) schedule(static)
	for (int i = 0; i < Ids->Len(); i++) {
		const TNGraph::TNodeI NI = graph->GetNI(Ids->GetVal(i));
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
            if (newDeg > max.val) { 
				max.val = newDeg;
				max.node = Ids->GetVal(i);
			}
        }
	}

    return max.node;
};

//Performs a simple random walk on the output of getParPivot()
int pivot::getParPivotRand(enhancedgraph *g, int color)
{
    int node = getParPivot(g, color);

	if (node == -1) {
		return node;
	}

	return randwalk(g, color, node, g->RAND_WALK_ITERATIONS);
};









