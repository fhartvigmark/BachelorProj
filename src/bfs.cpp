#include "bfs.h"

//Maybe include mark
//The passed parameters should be new graph struct.
int colorbfs(enhancedgraph *g, int color){
    TSnapQueue<int> Queue;
    TIntH *colors = g->colors;

    //Find pivot node
    int startNode = pivot::getPivot(g, color);
    Queue.Push(startNode);

    const int sccColor = g->colorGen->getNext();
    const int fwColor = g->colorGen->getNext();
    const int bwColor = g->colorGen->getNext();

    int v = 0;

    while(!Queue.Empty())
    {
        const int node = Queue.Top();
        Queue.Pop();

        if (colors->GetDat(node) == color) {
            colors->AddDat(node, fwColor);

            //Get node iterator for the current node
            const TNGraph::TNodeI NodeI = g->graph->GetNI(node);

            //Add all out edges that have not already been visited to the queue
            for (v = 0; v < NodeI.GetOutDeg(); v++) 
            {
                const int outNode = NodeI.GetOutNId(v);

                if (colors->GetDat(outNode) != fwColor) {
                    Queue.Push(outNode);
                }
            }
        }
    }


    return 0;
}   