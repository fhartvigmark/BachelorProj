#include "pivot.h"

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
    int bestDegree = 0;
    TIntH *colorMap = g->colors;
    PNGraph graph = g->graph;
    for (TNGraph::TNodeI NI = graph->BegNI(); NI < graph->EndNI(); NI++)
    {
        if(colorMap->GetDat(NI.GetId())==color){
            int newDeg = NI.GetInDeg() * NI.GetOutDeg();
            if (newDeg>bestDegree){
                bestNode = NI.GetId();
                bestDegree = newDeg;
            }
        }
    }
    return bestNode;
};