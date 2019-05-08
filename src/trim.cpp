#include "trim.h"

int trim::doTrim(int trimlevel, enhancedgraph *g, int color){
	switch (trimlevel)
	{
	case 1:
		return trim::trim1(g, color);
	default:
		return -1;
	}
};

int trim::doParTrim(int trimlevel, enhancedgraph *g, int color)
{
	switch (trimlevel)
	{
	case 1:
		return trim::partrim1(g, color);
	default:
		return -1;
	}
};

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
	TSnapQueue<int> Queue;
	TIntH *colors = g->colors;
	PNGraph graph = g->graph;
	TIntV *Ids = g->NIds;

	#pragma omp parallel for schedule(dynamic)
	for (int i = 0; i < Ids->Len(); i++)
	{
		int node = Ids->GetVal(i);
		if (colors->GetDat(node) == color)
		{
			int inDegree = 0;
			TNGraph::TNodeI NodeI = graph->GetNI(node);

			for (int v = 0; v < NodeI.GetInDeg(); v++)
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
				#pragma omp critical
				{
					Queue.Push(node);
				}
				continue;
			}

			int outDegree = 0;
			NodeI = graph->GetNI(node);

			for (int v = 0; v < NodeI.GetOutDeg(); v++)
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
				#pragma omp critical
				{
					Queue.Push(node);
				}
				continue;
			}
		}
	}

	while (!Queue.Empty())
	{
		int qsize = Queue.Len();
		#pragma omp parallel for
		for (int q = 0; q < qsize; q++)
		{
			int oldnode;
			#pragma omp critical 
			{
				oldnode = Queue.Top();
				Queue.Pop();
			}

			TNGraph::TNodeI OldNodeI = graph->GetNI(oldnode);
			for (int i = 0; i < OldNodeI.GetInDeg(); i++)
			{
				int node = OldNodeI.GetInNId(i);
				if (colors->GetDat(node) == color)
				{
					int inDegree = 0;
					TNGraph::TNodeI NodeI = graph->GetNI(node);

					int v = 0;
					for (v = 0; v < NodeI.GetInDeg(); v++)
					{
						int inNode = NodeI.GetInNId(v);

						if (colors->GetDat(inNode) == color && inNode != node)
						{
							inDegree = 1;
							break;
						}
					}

					if (inDegree == 0)
					{
						colors->AddDat(node, g->colorGen->getNext());
						#pragma omp critical 
						{
							Queue.Push(node);
						}
						continue;
					}

					int outDegree = 0;
					NodeI = graph->GetNI(node);

					for (v = 0; v < NodeI.GetOutDeg(); v++)
					{
						int outNode = NodeI.GetOutNId(v);

						if (colors->GetDat(outNode) == color && outNode != node)
						{
							outDegree = 1;
							break;
						}
					}

					if (outDegree == 0)
					{
						colors->AddDat(node, g->colorGen->getNext());
						#pragma omp critical 
						{
							Queue.Push(node);
						}
						continue;
					}
				}
			}
			for (int i = 0; i < OldNodeI.GetOutDeg(); i++)
			{
				int node = OldNodeI.GetOutNId(i);
				if (colors->GetDat(node) == color)
				{
					int inDegree = 0;
					TNGraph::TNodeI NodeI = graph->GetNI(node);

					for (int v = 0; v < NodeI.GetInDeg(); v++)
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
						#pragma omp critical
						{
							Queue.Push(node);
						}
						continue;
					}

					int outDegree = 0;
					NodeI = graph->GetNI(node);

					for (int v = 0; v < NodeI.GetOutDeg(); v++)
					{
						int outNode = NodeI.GetOutNId(v);

						if (colors->GetDat(outNode) == color && outNode != node)
						{
							outDegree = 1;
							break;
						}
					}

					if (outDegree == 0)
					{
						colors->AddDat(node, g->colorGen->getNext());
						#pragma omp critical
						{
							Queue.Push(node);
						}
						continue;
					}
				}
			}
		}
		
	}

	return -1;
};