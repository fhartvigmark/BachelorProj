#include "pivot.h"

int pivot::findPivot(enhancedgraph *g, int color, int method) {
	switch (method)
	{
		case 0:
			return pivot::getPivot(g, color);
		case 1:
			return pivot::getPivotMaxDegree(g, color);
		case 2:
			return pivot::getPivotMaxDegreeColor(g, color);
		case 3:
			return pivot::getParPivot(g, color);
		case 4:
			return pivot::getParPivotMaxDegree(g, color);
		case 5:
			return pivot::getParPivotMaxDegreeColor(g, color);
	}
}

int pivot::getPivot(enhancedgraph *g, int color)
{
    TIntH *colorMap = g->colors;
    for (THashKeyDatI<TInt, TInt> i = colorMap->BegI(); i < colorMap->EndI(); i++)
    {
        if (i.GetDat()==color)
        {
            return i.GetKey();
        }
        
    }
    return -1;
};

int pivot::getPivotMaxDegree(enhancedgraph *g, int color)
{
    int bestNode = -1;
    int bestDegree = -1;
    TIntH *colors = g->colors;
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

int pivot::getPivotMaxDegreeColor(enhancedgraph *g, int color)
{
    int bestNode = -1;
    int bestDegree = -1;
    TIntH *colors = g->colors;
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

int pivot::getParPivot(enhancedgraph *g, int color)
{
    TIntH *colorMap = g->colors;
	PNGraph graph = g->graph;
	TIntV *Ids = g->NIds;

	int retVal = -1;

	#pragma omp parallel for schedule(dynamic)
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

int pivot::getParPivotMaxDegree(enhancedgraph *g, int color)
{
	TIntH *colors = g->colors;
    PNGraph graph = g->graph;
	TIntV *Ids = g->NIds;

	struct Compare max; 
	max.val = -1; 
	max.node = -1;
	#pragma omp parallel for reduction(maximum:max)
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

int pivot::getParPivotMaxDegreeColor(enhancedgraph *g, int color)
{
	TIntH *colors = g->colors;
    PNGraph graph = g->graph;
	TIntV *Ids = g->NIds;

	struct Compare max; 
	max.val = -1; 
	max.node = -1;
	#pragma omp parallel for reduction(maximum:max)
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