//#include "iostream"
#include "trim.h"

int trim::doTrim(int trimlevel, enhancedgraph *g, int color){
	switch (trimlevel)
	{
	case 1:
		return trim::trim1(g, color);
	case 2:
		return trim::trim2(g, color);
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
	case 2:
		return trim::partrim2(g, color);
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

int trim::trim2(enhancedgraph *g, int color)
{
	TIntH *colors = g->colors;
	PNGraph graph = g->graph;
	TIntV *Ids = g->NIds;

	for (int i = 0; i < Ids->Len(); i++)
	{	
		int node = Ids->GetVal(i);
		//std::cout << "looking at node " << node << "\n"; 
		if (colors->GetDat(node) == color)
		{
			int inDegree = 0;
			int lastNode;
			TNGraph::TNodeI NodeI = graph->GetNI(node);

			for (int v = 0; v < NodeI.GetInDeg(); v++)
			{
				const int inNode = NodeI.GetInNId(v);

				if (colors->GetDat(inNode) == color && inNode != node)
				{
					inDegree++;
					lastNode = inNode;
				}
			}

			if (inDegree == 1)
			{
				//std::cout << "Found node with in-deg 1: " << node << "\n";
				int newInDegree = 0;
				TNGraph::TNodeI LastNodeI = graph->GetNI(lastNode);
				for (int v = 0; v < LastNodeI.GetInDeg(); v++)
				{
					const int outoutNode = LastNodeI.GetInNId(v);

					if (colors->GetDat(outoutNode) == color && outoutNode != lastNode)
					{
						newInDegree++;
					}
				}
				//std::cout << "Found potential SCC containing: " << node << " and " << lastNode << ". With in-deg " << newInDegree << ", " << LastNodeI.IsInNId(node) << "\n";
				if (newInDegree == 1 && LastNodeI.IsInNId(node))
				{
					//std::cout << "Found SCC containing: " << node << " and " << lastNode << "\n";
					int newSCC = g->colorGen->getNext();
					colors->AddDat(node, newSCC);
					colors->AddDat(lastNode, newSCC);
					continue;
				}
			}

			int outDegree = 0;
			NodeI = graph->GetNI(node);

			for (int v = 0; v < NodeI.GetOutDeg(); v++)
			{
				const int outNode = NodeI.GetOutNId(v);

				if (colors->GetDat(outNode) == color && outNode != node)
				{
					outDegree++;
					lastNode = outNode;
				}
			}

			if (outDegree == 1)
			{
				//std::cout << "Found node with out-deg 1: " << node << "\n";
				int newOutDegree = 0;
				TNGraph::TNodeI LastNodeI = graph->GetNI(lastNode);
				for (int v = 0; v < LastNodeI.GetOutDeg(); v++)
				{
					const int outoutNode = LastNodeI.GetOutNId(v);

					if (colors->GetDat(outoutNode) == color && outoutNode != lastNode)
					{
						newOutDegree++;
					}
				}
				//std::cout << "Found potential SCC containing: " << node << " and " << lastNode << ". With out-deg " << newOutDegree << ", " << LastNodeI.IsInNId(node) << "\n";
				if (newOutDegree == 1 && LastNodeI.IsOutNId(node))
				{
					//std::cout << "Found SCC containing: " << node << " and " << lastNode <<"\n";
					int newSCC = g->colorGen->getNext();
					colors->AddDat(node, newSCC);
					colors->AddDat(lastNode, newSCC);
					continue;
				}
			}
		}
	}
	return -1;
};

int trim::partrim2(enhancedgraph *g, int color)
{
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
			int lastNode;
			TNGraph::TNodeI NodeI = graph->GetNI(node);

			for (int v = 0; v < NodeI.GetInDeg(); v++)
			{
				const int inNode = NodeI.GetInNId(v);

				if (colors->GetDat(inNode) == color && inNode != node)
				{
					inDegree++;
					lastNode = inNode;
				}
			}

			if (inDegree == 1 && colors->GetDat(lastNode) == color)
			{
				int newInDegree = 0;
				TNGraph::TNodeI LastNodeI = graph->GetNI(lastNode);
				for (int v = 0; v < LastNodeI.GetInDeg(); v++)
				{
					const int outoutNode = LastNodeI.GetInNId(v);

					if (colors->GetDat(outoutNode) == color && outoutNode != lastNode)
					{
						newInDegree++;
					}
				}
				if (newInDegree == 1 && LastNodeI.IsInNId(node))
				{
					#pragma omp critical
					{
						int newSCC = g->colorGen->getNext();
						colors->AddDat(node, newSCC);
						colors->AddDat(lastNode, newSCC);
					}
					continue;
				}
			}

			int outDegree = 0;
			NodeI = graph->GetNI(node);

			for (int v = 0; v < NodeI.GetOutDeg(); v++)
			{
				const int outNode = NodeI.GetOutNId(v);

				if (colors->GetDat(outNode) == color && outNode != node)
				{
					outDegree++;
					lastNode = outNode;
				}
			}

			if (outDegree == 1 && colors->GetDat(lastNode) == color)
			{
				int newOutDegree = 0;
				TNGraph::TNodeI LastNodeI = graph->GetNI(lastNode);
				for (int v = 0; v < LastNodeI.GetOutDeg(); v++)
				{
					const int outoutNode = LastNodeI.GetOutNId(v);

					if (colors->GetDat(outoutNode) == color && outoutNode != lastNode)
					{
						newOutDegree++;
					}
				}
				if (newOutDegree == 1 && LastNodeI.IsOutNId(node)){
					#pragma omp critical 
					{
						int newSCC = g->colorGen->getNext();
						colors->AddDat(node, newSCC);
						colors->AddDat(lastNode, newSCC);
					}
					continue;
				}
			}
		}
	}
	return -1;
};