#include "iostream"
#include "trim.h"

int trim::doTrim(int trimlevel, enhancedgraph *g, int color) {
	int retVal = -1;
	TimePoint start = g->startTimer();
	TimePoint start2;

	switch (trimlevel)
	{
		case 1:
			retVal = trim::partrim1(g, color, false);
			g->endTimer(start, eTimer::TRIM1);
			break;
		case 2:
			trim::partrim1(g, color, false);
			g->endTimer(start, eTimer::TRIM1);
			start2 = g->startTimer();
			retVal = trim::partrim2(g, color, false);
			g->endTimer(start2, eTimer::TRIM2);
			break;
		case 3:
			trim::partrim1(g, color, false);
			g->endTimer(start, eTimer::TRIM1);
			start2 = g->startTimer();
			trim::partrim2(g, color, false);
			g->endTimer(start2, eTimer::TRIM2);
			start2 = g->startTimer();
			retVal = trim::partrim3(g, color, false);
			g->endTimer(start2, eTimer::TRIM3);
			break;
		default:
			retVal = -1;
			break;
	}

	g->endTimer(start, eTimer::TRIM);
	return retVal;
}

int trim::doParTrim(int trimlevel, enhancedgraph *g, int color) {
	int retVal = -1;
	TimePoint start = g->startTimer();
	TimePoint start2;

	switch (trimlevel)
	{
		case 1:
			retVal = trim::partrim1(g, color, true);
			g->endTimer(start, eTimer::TRIM1);
			break;
		case 2:
			trim::partrim1(g, color, true);
			g->endTimer(start, eTimer::TRIM1);
			start2 = g->startTimer();
			retVal = trim::partrim2(g, color, true);
			g->endTimer(start2, eTimer::TRIM2);
			break;
		case 3:
			trim::partrim1(g, color, true);
			g->endTimer(start, eTimer::TRIM1);
			start2 = g->startTimer();
			trim::partrim2(g, color, true);
			g->endTimer(start2, eTimer::TRIM2);
			start2 = g->startTimer();
			retVal = trim::partrim3(g, color, true);
			g->endTimer(start2, eTimer::TRIM3);
			break;
		default:
			retVal = -1;
			break;
	}

	g->endTimer(start, eTimer::TRIM);
	return retVal;
}

int trim::trim1(enhancedgraph *g, int color)
{
	TSnapQueue<int> Queue;
	ColorMap *colors = g->colors;
	PNGraph graph = g->graph;
	int count = 0;

	for (TNGraph::TNodeI NI = graph->BegNI(); NI < graph->EndNI(); NI++)
    {
		int node = NI.GetId();
		if (colors->GetDat(node) == color) {
			int inDegree = 0;
			TNGraph::TNodeI NodeI = graph->GetNI(node);

			int v = 0;
			for (v = 0; v < NodeI.GetInDeg(); v++) 
            {
                int outNode = NodeI.GetInNId(v);

                if (colors->GetDat(outNode) == color && outNode != node) {
                    inDegree = 1;
					break;
                }
            }

			if (inDegree == 0) {
				colors->AddDat(node, g->colorGen->getNext());
				count++;
				Queue.Push(node);
				continue;
			}

			int outDegree = 0;
			NodeI = graph->GetNI(node);

			for (v = 0; v < NodeI.GetOutDeg(); v++) 
            {
                int outNode = NodeI.GetOutNId(v);

                if (colors->GetDat(outNode) == color && outNode != node) {
                    outDegree = 1;
					break;
                }
            }

			if (outDegree == 0) {
				colors->AddDat(node, g->colorGen->getNext());
				count++;
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
			int node = OldNodeI.GetInNId(i);
			if (colors->GetDat(node) == color)
			{
				int inDegree = 0;
				TNGraph::TNodeI NodeI = graph->GetNI(node);

				int v = 0;
				for (v = 0; v < NodeI.GetInDeg(); v++)
				{
					int outNode = NodeI.GetInNId(v);

					if (colors->GetDat(outNode) == color && outNode != node)
					{
						inDegree = 1;
						break;
					}
				}

				if (inDegree == 0)
				{
					colors->AddDat(node, g->colorGen->getNext());
					count++;
					Queue.Push(node);
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
					count++;
					Queue.Push(node);
					continue;
				}
			}
		}
		for (i = 0; i < OldNodeI.GetOutDeg(); i++)
		{
			int node = OldNodeI.GetOutNId(i);
			if (colors->GetDat(node) == color)
			{
				int inDegree = 0;
				TNGraph::TNodeI NodeI = graph->GetNI(node);

				int v = 0;
				for (v = 0; v < NodeI.GetInDeg(); v++)
				{
					int outNode = NodeI.GetInNId(v);

					if (colors->GetDat(outNode) == color && outNode != node)
					{
						inDegree = 1;
						break;
					}
				}

				if (inDegree == 0)
				{
					colors->AddDat(node, g->colorGen->getNext());
					count++;
					Queue.Push(node);
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
					count++;
					Queue.Push(node);
					continue;
				}
			}
		}
	}

	g->reportTrim(color, count, 1);
    return -1;
};

int trim::partrim1(enhancedgraph *g, int color, bool parallel)
{
	TSnapQueue<int> Queue;
	ColorMap *colors = g->colors;
	PNGraph graph = g->graph;
	TIntV *Ids = g->NIds;
	int count = 0;
	int count2 = 0;

	#pragma omp parallel for schedule(static) reduction(+:count) if(parallel)
	for (int i = 0; i < Ids->Len(); i++)
	{
		int node = Ids->GetVal(i);
		int nodeColor;
		//std::cout << "Error on node " << node << "\n";
		nodeColor = colors->GetDat(node);

		if (nodeColor == color)
		{
			int inDegree = 0;
			TNGraph::TNodeI NodeI = graph->GetNI(node);

			for (int v = 0; v < NodeI.GetInDeg(); v++)
			{
				int outNode = NodeI.GetInNId(v);
				//std::cout << "Error on node " << outNode << "\n";
				int outColor;
				outColor = colors->GetDat(outNode);


				if (outColor == color && outNode != node)
				{
					inDegree = 1;
					break;
				}
			}

			if (inDegree == 0)
			{
				#pragma omp critical
				{
					colors->AddDat(node, g->colorGen->getNext());
					count++;
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
				#pragma omp critical
				{
					colors->AddDat(node, g->colorGen->getNext());
					count++;
					Queue.Push(node);
				}
				continue;
			}
		}
	}
	
	while (!Queue.Empty())
	{
		int qsize = Queue.Len();
		int count2 = 0;

		#pragma omp parallel for schedule(static) reduction(+:count2) if(parallel)
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

				int nodeColor;
				#pragma omp critical 
				{
					nodeColor = colors->GetDat(node);
				}
				if (nodeColor == color)
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
						#pragma omp critical 
						{
							colors->AddDat(node, g->colorGen->getNext());
							count2++;
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
						#pragma omp critical 
						{
							colors->AddDat(node, g->colorGen->getNext());
							count2++;
							Queue.Push(node);
						}
						continue;
					}
				}
			}
			for (int i = 0; i < OldNodeI.GetOutDeg(); i++)
			{
				int node = OldNodeI.GetOutNId(i);
				int nodeColor;
				nodeColor = colors->GetDat(node);
			
				if (nodeColor == color)
				{
					int inDegree = 0;
					TNGraph::TNodeI NodeI = graph->GetNI(node);

					for (int v = 0; v < NodeI.GetInDeg(); v++)
					{
						int outNode = NodeI.GetInNId(v);

						if (colors->GetDat(outNode) == color && outNode != node)
						{
							inDegree = 1;
							break;
						}
					}

					if (inDegree == 0)
					{
						#pragma omp critical
						{
							colors->AddDat(node, g->colorGen->getNext());
							count2++;
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
						#pragma omp critical
						{
							colors->AddDat(node, g->colorGen->getNext());
							count2++;
							Queue.Push(node);
						}
						continue;
					}
				}
			}
		}
		
		count += count2;
	}

	g->reportTrim(color, count, 1);
	return -1;
};

int trim::trim2(enhancedgraph *g, int color)
{
	ColorMap *colors = g->colors;
	PNGraph graph = g->graph;
	TIntV *Ids = g->NIds;
	int count = 0;

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
				int inNode = NodeI.GetInNId(v);

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
					int outoutNode = LastNodeI.GetInNId(v);

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
					count++;
					continue;
				}
			}

			int outDegree = 0;
			NodeI = graph->GetNI(node);

			for (int v = 0; v < NodeI.GetOutDeg(); v++)
			{
				int outNode = NodeI.GetOutNId(v);

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
					int outoutNode = LastNodeI.GetOutNId(v);

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
					count++;
					continue;
				}
			}
		}
	}

	g->reportTrim(color, count, 2);
	return -1;
};

int trim::partrim2(enhancedgraph *g, int color, bool parallel)
{
	ColorMap *colors = g->colors;
	PNGraph graph = g->graph;
	TIntV *Ids = g->NIds;
	int count = 0;

	#pragma omp parallel for schedule(static) reduction(+:count) if(parallel)
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
				int inNode = NodeI.GetInNId(v);

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
					int outoutNode = LastNodeI.GetInNId(v);

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
						count++;
					}
					continue;
				}
			}

			int outDegree = 0;
			NodeI = graph->GetNI(node);

			for (int v = 0; v < NodeI.GetOutDeg(); v++)
			{
				int outNode = NodeI.GetOutNId(v);

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
					int outoutNode = LastNodeI.GetOutNId(v);

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
						count++;
					}
					continue;
				}
			}
		}
	}

	g->reportTrim(color, count, 2);
	return -1;
};

int trim::trim3(enhancedgraph *g, int color){
	ColorMap *colors = g->colors;
	PNGraph graph = g->graph;
	TIntV *Ids = g->NIds;
	int count = 0;

	for (int i = 0; i < Ids->Len(); i++)
	{
		int node = Ids->GetVal(i);
		if (colors->GetDat(node) == color)
		{
			int inDegree = 0;
			int nodeB = -1;
			int nodeC = -2;
			TNGraph::TNodeI NodeI = graph->GetNI(node);

			for (int v = 0; v < NodeI.GetInDeg(); v++)
			{
				int inNode = NodeI.GetInNId(v);

				if (colors->GetDat(inNode) == color && inNode != node)
				{
					if(inDegree == 0){
						inDegree++;
						nodeB = inNode;
					}else if (inDegree == 1)
					{
						inDegree++;
						nodeC = inNode;
					}else{
						//If out-deg is now 3, break
						inDegree++;
						break;
					}
				}
			}

			if (inDegree == 1 && colors->GetDat(nodeB) == color)
			{
				int inDegree_B = 0;
				TNGraph::TNodeI NodeBI = graph->GetNI(nodeB);
				for (int v = 0; v < NodeBI.GetInDeg(); v++)
				{
					nodeC = NodeBI.GetInNId(v);

					if (colors->GetDat(nodeC) == color && nodeC != nodeB)
					{	
						inDegree_B++;
						if(inDegree_B==2){
							break;
						}
					}
				}
				if (inDegree_B == 1)
				{
					int inDegree_C = 0;
					TNGraph::TNodeI NodeCI = graph->GetNI(nodeC);
					for (int v = 0; v < NodeCI.GetInDeg(); v++)
					{
						int out_node_c = NodeCI.GetInNId(v);

						if (colors->GetDat(out_node_c) == color && nodeC != out_node_c)
						{
							inDegree_C++;
							if (inDegree_C == 2)
							{
								break;
							}
						}
					}
					if(inDegree_C == 1 && NodeCI.IsInNId(node)){
						int newSCC = g->colorGen->getNext();
						colors->AddDat(node, newSCC);
						colors->AddDat(nodeB, newSCC);
						colors->AddDat(nodeC, newSCC);
						count++;
					}
					continue;
				}
			}

			if (inDegree == 2 && colors->GetDat(nodeB) == color)
			{
				//std::cout << "Found potential pattern 2 on in-degrees, node: " << node << "\n";
				int inDegree_B = 0;
				TNGraph::TNodeI NodeBI = graph->GetNI(nodeB);
				for (int v = 0; v < NodeBI.GetInDeg(); v++)
				{
					int out_node_B = NodeBI.GetInNId(v);

					if (colors->GetDat(out_node_B) == color && out_node_B != nodeB)
					{
						inDegree_B++;
						if (inDegree_B == 2)
						{
							//std::cout << "B's in-degree over 1, breaking \n";
							break;
						}
					}
				}
				if (inDegree_B == 1 && NodeBI.IsInNId(node))
				{
					//std::cout << "B's in-degree is 1, proceeding \n";
					int inDegree_C = 0;
					TNGraph::TNodeI NodeCI = graph->GetNI(nodeC);
					for (int v = 0; v < NodeCI.GetInDeg(); v++)
					{
						int in_node_c = NodeCI.GetInNId(v);

						if (colors->GetDat(in_node_c) == color && nodeC != in_node_c)
						{
							inDegree_C++;
							if (inDegree_C == 2)
							{
								break;
							}
						}
					}
					if (inDegree_C == 1 && NodeCI.IsInNId(node))
					{
						//std::cout << "Found pattern 2 SCC \n";
						int newSCC = g->colorGen->getNext();
						colors->AddDat(node, newSCC);
						colors->AddDat(nodeB, newSCC);
						colors->AddDat(nodeC, newSCC);
						count++;
					}
					continue;
				}
			}

			int outDegree = 0;
			NodeI = graph->GetNI(node);

			for (int v = 0; v < NodeI.GetOutDeg(); v++)
			{
				int outNode = NodeI.GetOutNId(v);

				if (colors->GetDat(outNode) == color && outNode != node)
				{
					if (outDegree == 0)
					{
						outDegree++;
						nodeB = outNode;
					}
					else if (outDegree == 1)
					{
						outDegree++;
						nodeC = outNode;
					}
					else
					{
						outDegree++;
						//If out-deg is now 3, break
						break;
					}
				}
			}

			if (outDegree == 1 && colors->GetDat(nodeB) == color)
			{
				int outDegree_B = 0;
				TNGraph::TNodeI NodeBI = graph->GetNI(nodeB);
				for (int v = 0; v < NodeBI.GetInDeg(); v++)
				{
					nodeC = NodeBI.GetOutNId(v);

					if (colors->GetDat(nodeC) == color && nodeC != nodeB)
					{
						outDegree_B++;
						if (outDegree_B == 2)
						{
							break;
						}
					}
				}
				if (outDegree_B == 1)
				{
					int outDegree_C = 0;
					TNGraph::TNodeI NodeCI = graph->GetNI(nodeC);
					for (int v = 0; v < NodeCI.GetOutDeg(); v++)
					{
						int out_node_c = NodeCI.GetOutNId(v);

						if (colors->GetDat(out_node_c) == color && nodeC != out_node_c)
						{
							outDegree_C++;
							if (outDegree_C == 2)
							{
								break;
							}
						}
					}
					if (outDegree_C == 1 && NodeCI.IsOutNId(node))
					{
						int newSCC = g->colorGen->getNext();
						colors->AddDat(node, newSCC);
						colors->AddDat(nodeB, newSCC);
						colors->AddDat(nodeC, newSCC);
						count++;
					}
					continue;
				}
			}

			if (outDegree == 2 && colors->GetDat(nodeB) == color)
			{
				//std::cout << "Found potential pattern 2 on out-degrees, node: " << node << "\n";
				int outDegree_B = 0;
				TNGraph::TNodeI NodeBI = graph->GetNI(nodeB);
				for (int v = 0; v < NodeBI.GetOutDeg(); v++)
				{
					int out_node_B = NodeBI.GetOutNId(v);

					if (colors->GetDat(out_node_B) == color && out_node_B != nodeB)
					{
						outDegree_B++;
						if (outDegree_B == 2)
						{
							//std::cout << "B's out-degree over 1, breaking \n";
							break;
						}
					}
				}
				if (outDegree_B == 1 && NodeBI.IsOutNId(node))
				{
					//std::cout << "B's out-degree is 1, proceeding \n";
					int outDegree_C = 0;
					TNGraph::TNodeI NodeCI = graph->GetNI(nodeC);
					for (int v = 0; v < NodeCI.GetOutDeg(); v++)
					{
						int out_node_c = NodeCI.GetOutNId(v);

						if (colors->GetDat(out_node_c) == color && nodeC != out_node_c)
						{
							outDegree_C++;
							if (outDegree_C == 2)
							{
								break;
							}
						}
					}
					if (outDegree_C == 1 && NodeCI.IsOutNId(node))
					{
						//std::cout << "Found pattern 2 SCC \n";
						int newSCC = g->colorGen->getNext();
						colors->AddDat(node, newSCC);
						colors->AddDat(nodeB, newSCC);
						colors->AddDat(nodeC, newSCC);
						count++;
					}
					continue;
				}
			}
		}
	}

	g->reportTrim(color, count, 3);
	return -1;
};

int trim::partrim3(enhancedgraph *g, int color, bool parallel)
{
	ColorMap *colors = g->colors;
	PNGraph graph = g->graph;
	TIntV *Ids = g->NIds;
	int count = 0;

	#pragma omp parallel for schedule(static) reduction(+:count) if(parallel)
	for (int i = 0; i < Ids->Len(); i++)
	{
		int node = Ids->GetVal(i);
		if (colors->GetDat(node) == color)
		{
			int inDegree = 0;
			int nodeB = -1;
			int nodeC = -2;
			TNGraph::TNodeI NodeI = graph->GetNI(node);

			for (int v = 0; v < NodeI.GetInDeg(); v++)
			{
				int inNode = NodeI.GetInNId(v);

				if (colors->GetDat(inNode) == color && inNode != node)
				{
					if (inDegree == 0)
					{
						inDegree++;
						nodeB = inNode;
					}
					else if (inDegree == 1)
					{
						inDegree++;
						nodeC = inNode;
					}
					else
					{
						//If out-deg is now 3, break
						inDegree++;
						break;
					}
				}
			}

			if (inDegree == 1 && colors->GetDat(nodeB) == color)
			{
				//#pragma omp critical
				//{
				//	std::cout << "Found potential pattern 1 on in-degrees, node: " << node << "\n";
				//}
				int inDegree_B = 0;
				int inNode_B = 0;
				TNGraph::TNodeI NodeBI = graph->GetNI(nodeB);
				for (int v = 0; v < NodeBI.GetInDeg(); v++)
				{
					inNode_B = NodeBI.GetInNId(v);

					if (colors->GetDat(inNode_B) == color && inNode_B != nodeB)
					{
						if (inDegree_B == 0)
						{
							inDegree_B++;
							nodeC = inNode_B;
						}
						else
						{
							//If out-deg is now 3, break
							inDegree_B++;
							break;
						}
					}
				}
				if (inDegree_B == 1 && colors->GetDat(nodeB) == color)
				{
					int inDegree_C = 0;
					TNGraph::TNodeI NodeCI = graph->GetNI(nodeC);
					for (int v = 0; v < NodeCI.GetInDeg(); v++)
					{
						int out_node_c = NodeCI.GetInNId(v);

						if (colors->GetDat(out_node_c) == color && nodeC != out_node_c)
						{
							inDegree_C++;
							if (inDegree_C == 2)
							{
								break;
							}
						}
					}
					if (inDegree_C == 1 && NodeCI.IsInNId(node) && colors->GetDat(nodeC) == color)
					{
						#pragma omp critical
						{
							if (colors->GetDat(node) == color && colors->GetDat(nodeB) == color && colors->GetDat(nodeC) == color)
							{
								int newSCC = g->colorGen->getNext();
								colors->AddDat(node, newSCC);
								colors->AddDat(nodeB, newSCC);
								colors->AddDat(nodeC, newSCC);
								count++;
							}
						}
					}
					continue;
				}
			}

			if (inDegree == 2 && colors->GetDat(nodeB) == color)
			{
				//std::cout << "Found potential pattern 2 on in-degrees, node: " << node << "\n";
				int inDegree_B = 0;
				TNGraph::TNodeI NodeBI = graph->GetNI(nodeB);
				for (int v = 0; v < NodeBI.GetInDeg(); v++)
				{
					int out_node_B = NodeBI.GetInNId(v);

					if (colors->GetDat(out_node_B) == color && out_node_B != nodeB)
					{
						inDegree_B++;
						if (inDegree_B == 2)
						{
							//std::cout << "B's in-degree over 1, breaking \n";
							break;
						}
					}
				}
				if (inDegree_B == 1 && NodeBI.IsInNId(node) && colors->GetDat(nodeB) == color)
				{
					//std::cout << "B's in-degree is 1, proceeding \n";
					int inDegree_C = 0;
					TNGraph::TNodeI NodeCI = graph->GetNI(nodeC);
					for (int v = 0; v < NodeCI.GetInDeg(); v++)
					{
						int in_node_c = NodeCI.GetInNId(v);

						if (colors->GetDat(in_node_c) == color && nodeC != in_node_c)
						{
							inDegree_C++;
							if (inDegree_C == 2)
							{
								break;
							}
						}
					}
					if (inDegree_C == 1 && NodeCI.IsInNId(node) && colors->GetDat(nodeC) == color)
					{
						//std::cout << "Found pattern 2 SCC \n";
						#pragma omp critical
						{
							if (colors->GetDat(node) == color && colors->GetDat(nodeB) == color && colors->GetDat(nodeC) == color)
							{
								int newSCC = g->colorGen->getNext();
								colors->AddDat(node, newSCC);
								colors->AddDat(nodeB, newSCC);
								colors->AddDat(nodeC, newSCC);
								count++;
							}
						}
					}
					continue;
				}
			}

			int outDegree = 0;
			NodeI = graph->GetNI(node);

			for (int v = 0; v < NodeI.GetOutDeg(); v++)
			{
				int outNode = NodeI.GetOutNId(v);

				if (colors->GetDat(outNode) == color && outNode != node)
				{
					if (outDegree == 0)
					{
						outDegree++;
						nodeB = outNode;
					}
					else if (outDegree == 1)
					{
						outDegree++;
						nodeC = outNode;
					}
					else
					{
						outDegree++;
						//If out-deg is now 3, break
						break;
					}
				}
			}

			if (outDegree == 1 && colors->GetDat(nodeB) == color)
			{
				//#pragma omp critical 
				//{
					//std::cout << "Found potential pattern 1 on out-degrees, node: " << node << "\n";
				//}
				int outDegree_B = 0;
				int outNode_B = 0;
				TNGraph::TNodeI NodeBI = graph->GetNI(nodeB);
				for (int v = 0; v < NodeBI.GetInDeg(); v++)
				{
					outNode_B = NodeBI.GetOutNId(v);

					if (colors->GetDat(outNode_B) == color && outNode_B != nodeB)
					{
						if (outDegree_B == 0)
						{
							outDegree_B++;
							nodeC = outNode_B;
						}
						else
						{
							outDegree_B++;
							//If out-deg is now 3, break
							break;
						}
					}
				}
				if (outDegree_B == 1 && colors->GetDat(nodeB) == color)
				{
					int outDegree_C = 0;
					TNGraph::TNodeI NodeCI = graph->GetNI(nodeC);
					for (int v = 0; v < NodeCI.GetOutDeg(); v++)
					{
						int out_node_c = NodeCI.GetOutNId(v);

						if (colors->GetDat(out_node_c) == color && nodeC != out_node_c)
						{
							outDegree_C++;
							if (outDegree_C == 2)
							{
								break;
							}
						}
					}
					if (outDegree_C == 1 && NodeCI.IsOutNId(node) && colors->GetDat(nodeC) == color)
					{
						#pragma omp critical
						{
							if (colors->GetDat(node) == color && colors->GetDat(nodeB) == color && colors->GetDat(nodeC) == color)
							{
								int newSCC = g->colorGen->getNext();
								colors->AddDat(node, newSCC);
								colors->AddDat(nodeB, newSCC);
								colors->AddDat(nodeC, newSCC);
								count++;
							}
						}
					}
					continue;
				}
			}

			if (outDegree == 2 && colors->GetDat(nodeB) == color)
			{
				//std::cout << "Found potential pattern 2 on out-degrees, node: " << node << "\n";
				int outDegree_B = 0;
				TNGraph::TNodeI NodeBI = graph->GetNI(nodeB);
				for (int v = 0; v < NodeBI.GetOutDeg(); v++)
				{
					int out_node_B = NodeBI.GetOutNId(v);

					if (colors->GetDat(out_node_B) == color && out_node_B != nodeB)
					{
						outDegree_B++;
						if (outDegree_B == 2)
						{
							//std::cout << "B's out-degree over 1, breaking \n";
							break;
						}
					}
				}
				if (outDegree_B == 1 && NodeBI.IsOutNId(node) && colors->GetDat(nodeB) == color)
				{
					//std::cout << "B's out-degree is 1, proceeding \n";
					int outDegree_C = 0;
					TNGraph::TNodeI NodeCI = graph->GetNI(nodeC);
					for (int v = 0; v < NodeCI.GetOutDeg(); v++)
					{
						int out_node_c = NodeCI.GetOutNId(v);

						if (colors->GetDat(out_node_c) == color && nodeC != out_node_c)
						{
							outDegree_C++;
							if (outDegree_C == 2)
							{
								break;
							}
						}
					}
					if (outDegree_C == 1 && NodeCI.IsOutNId(node) && colors->GetDat(nodeC) == color)
					{
						//std::cout << "Found pattern 2 SCC \n";
						#pragma omp critical
						{
							if (colors->GetDat(node) == color && colors->GetDat(nodeB) == color && colors->GetDat(nodeC) == color)
							{
								int newSCC = g->colorGen->getNext();
								colors->AddDat(node, newSCC);
								colors->AddDat(nodeB, newSCC);
								colors->AddDat(nodeC, newSCC);
								count++;
							}
						}
					}
					continue;
				}
			}
		}
	}

	g->reportTrim(color, count, 3);
	return -1;
};