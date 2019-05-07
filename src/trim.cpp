#include "trim.h"


int trim::trim1(enhancedgraph *g, int color)
{
	TSnapQueue<int> Queue;
	TIntH *colors = g->colors;
	PNGraph graph = g->graph;

	for (TNGraph::TNodeI NI = graph->BegNI(); NI < graph->EndNI(); NI++)
    {
		int node = NI.GetId();
		if (colors->GetDat(node) == color) {
			int inDegree = 0;
			TNGraph::TNodeI NodeI = graph->GetNI(node);

			int v = 0;
			for (v = 0; v < NodeI.GetInDeg(); v++) 
            {
                const int outNode = NodeI.GetInNId(v);

                if (colors->GetDat(outNode) == color && outNode != node) {
                    inDegree = 1;
					break;
                }
            }

			if (inDegree == 0) {
				colors->AddDat(node, g->colorGen->getNext());
				Queue.Push(node);
				continue;
			}

			int outDegree = 0;
			NodeI = graph->GetNI(node);

			for (v = 0; v < NodeI.GetOutDeg(); v++) 
            {
                const int outNode = NodeI.GetOutNId(v);

                if (colors->GetDat(outNode) == color && outNode != node) {
                    outDegree = 1;
					break;
                }
            }

			if (outDegree == 0) {
				colors->AddDat(node, g->colorGen->getNext());
				Queue.Push(node);
				continue;
			}
		}
        
    }
	while (!Queue.Empty())
	{
		int oldnode = Queue.Top();
		Queue.Pop();

		TNGraph::TNodeI OldNodeI = graph->GetNI(oldnode);
		int i;
		for (i = 0; i < OldNodeI.GetInDeg(); i++)
		{
			const int node = OldNodeI.GetInNId(i);
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
						inDegree = 1;
						break;
					}
				}

				if (inDegree == 0)
				{
					colors->AddDat(node, g->colorGen->getNext());
					Queue.Push(node);
					continue;
				}

				int outDegree = 0;
				NodeI = graph->GetNI(node);

				for (v = 0; v < NodeI.GetOutDeg(); v++)
				{
					const int outNode = NodeI.GetOutNId(v);

					if (colors->GetDat(outNode) == color && outNode != node)
					{
						outDegree = 1;
						break;
					}
				}

				if (outDegree == 0)
				{
					colors->AddDat(node, g->colorGen->getNext());
					Queue.Push(node);
					continue;
				}
			}
		}
		for (i = 0; i < OldNodeI.GetOutDeg(); i++)
		{
			const int node = OldNodeI.GetOutNId(i);
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
						inDegree = 1;
						break;
					}
				}

				if (inDegree == 0)
				{
					colors->AddDat(node, g->colorGen->getNext());
					Queue.Push(node);
					continue;
				}

				int outDegree = 0;
				NodeI = graph->GetNI(node);

				for (v = 0; v < NodeI.GetOutDeg(); v++)
				{
					const int outNode = NodeI.GetOutNId(v);

					if (colors->GetDat(outNode) == color && outNode != node)
					{
						outDegree = 1;
						break;
					}
				}

				if (outDegree == 0)
				{
					colors->AddDat(node, g->colorGen->getNext());
					Queue.Push(node);
					continue;
				}
			}
		}
	}
	
    return -1;
};

int trim::partrim1(enhancedgraph *g, int color)
{
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
					inDegree = 1;
					break;
				}
			}

			if (inDegree == 0)
			{
				colors->AddDat(node, g->colorGen->getNext());
				continue;
			}

			int outDegree = 0;
			NodeI = graph->GetNI(node);

			for (v = 0; v < NodeI.GetOutDeg(); v++)
			{
				const int outNode = NodeI.GetOutNId(v);

				if (colors->GetDat(outNode) == color && outNode != node)
				{
					outDegree = 1;
					break;
				}
			}

			if (outDegree == 0)
			{
				colors->AddDat(node, g->colorGen->getNext());
				continue;
			}
		}
	}

	return -1;
};