#include "bfs.h"

//Maybe include mark
std::pair<int, int> bfs::colorbfs(enhancedgraph *g, int color, int startNode) {
    TSnapQueue<int> Queue;
    TIntH *colors = g->colors;
    PNGraph pgraph = g->graph;

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
            const TNGraph::TNodeI NodeI = pgraph->GetNI(node);

			//Add all out edges that have not already been visited to the queue
            for (v = 0; v < NodeI.GetOutDeg(); v++) 
            {
                const int outNode = NodeI.GetOutNId(v);

                if (colors->GetDat(outNode) == color) {
                    Queue.Push(outNode);
                }
            }
        }
    }
    Queue.Push(startNode);
    while (!Queue.Empty())
    {
        const int node = Queue.Top();
        Queue.Pop();
        const int nodeColor = colors->GetDat(node);

        if (nodeColor == color)
        {
            colors->AddDat(node, bwColor);

            //Get node iterator for the current node
            const TNGraph::TNodeI NodeI = pgraph->GetNI(node);

            //Add all out edges that have not already been visited to the queue
            for (v = 0; v < NodeI.GetInDeg(); v++)
            {
                const int inNode = NodeI.GetInNId(v);
                const int inNodeColor = colors->GetDat(inNode);

                if ((inNodeColor == color)||(inNodeColor == fwColor))
                {
                    Queue.Push(inNode);
                }
            }
        }else if (nodeColor == fwColor)
        {
            colors->AddDat(node, sccColor);

            //Get node iterator for the current node
            const TNGraph::TNodeI NodeI = pgraph->GetNI(node);

            //Add all out edges that have not already been visited to the queue
            for (v = 0; v < NodeI.GetInDeg(); v++)
            {
                const int inNode = NodeI.GetInNId(v);
                const int inNodeColor = colors->GetDat(inNode);

                if ((inNodeColor == color) || (inNodeColor == fwColor))
                {
                    Queue.Push(inNode);
                }
            }
        }
    }
    return std::make_pair(fwColor, bwColor);
}

std::pair<int, int> bfs::parbfs(enhancedgraph *g, int color, int startNode) {
	TSnapQueue<int> Queue;
	TIntH *colors = g->colors;
	PNGraph pgraph = g->graph;

	Queue.Push(startNode);

	const int sccColor = g->colorGen->getNext();
	const int fwColor = g->colorGen->getNext();
	const int bwColor = g->colorGen->getNext();

	int v = 0;

	while (!Queue.Empty())
	{
		int qsize = Queue.Len();

		#pragma omp parallel for 
		for (int i = 0; i<qsize; i++)
		{
			int node = 0;

			#pragma omp critical
			{
			node = Queue.Top();
			Queue.Pop();
			}

			if (colors->GetDat(node) == color)
			{
				colors->AddDat(node, fwColor);

				//Get node iterator for the current node
				const TNGraph::TNodeI NodeI = pgraph->GetNI(node);

				//Add all out edges that have not already been visited to the queue
				for (v = 0; v < NodeI.GetOutDeg(); v++)
				{
					const int outNode = NodeI.GetOutNId(v);

					if (colors->GetDat(outNode) == color)
					{
						#pragma omp critical
						Queue.Push(outNode);
					}
				}
			}
		}
	}

	Queue.Push(startNode);

	while (!Queue.Empty())
	{
		int qsize = Queue.Len();

		#pragma omp parallel for 
		for (int i = 0; i < qsize; i++)
		{
			int node = 0;

			#pragma omp critical
			{
			node = Queue.Top();
			Queue.Pop();
			}

			const int nodeColor = colors->GetDat(node);

			if (nodeColor == color)
			{
				colors->AddDat(node, bwColor);

				//Get node iterator for the current node
				const TNGraph::TNodeI NodeI = pgraph->GetNI(node);

				//Add all out edges that have not already been visited to the queue
				for (v = 0; v < NodeI.GetInDeg(); v++)
				{
					const int inNode = NodeI.GetInNId(v);
					const int inNodeColor = colors->GetDat(inNode);

					if ((inNodeColor == color) || (inNodeColor == fwColor))
					{
						#pragma omp critical
						Queue.Push(inNode);
					}
				}
			}
			else if (nodeColor == fwColor)
			{
				colors->AddDat(node, sccColor);

				//Get node iterator for the current node
				const TNGraph::TNodeI NodeI = pgraph->GetNI(node);

				//Add all out edges that have not already been visited to the queue
				for (v = 0; v < NodeI.GetInDeg(); v++)
				{
					const int inNode = NodeI.GetInNId(v);
					const int inNodeColor = colors->GetDat(inNode);

					if ((inNodeColor == color) || (inNodeColor == fwColor))
					{
						#pragma omp critical
						Queue.Push(inNode);
					}
				}
			}
		}
	}
	return std::make_pair(fwColor, bwColor);
}

//TODO: test fwbfs and parfwbfs
int bfs::fwbfs(enhancedgraph *g, int color, int startNode) {
    TSnapQueue<int> Queue;
    TIntH *colors = g->colors;
    PNGraph pgraph = g->graph;

    Queue.Push(startNode);

    const int sccColor = g->colorGen->getNext();
    const int fwColor = g->colorGen->getNext();

    int v = 0;

    while(!Queue.Empty())
    {
        const int node = Queue.Top();
        Queue.Pop();

        if (colors->GetDat(node) == color) {
            colors->AddDat(node, fwColor);


            //Get node iterator for the current node
            const TNGraph::TNodeI NodeI = pgraph->GetNI(node);

			//Add all out edges that have not already been visited to the queue
            for (v = 0; v < NodeI.GetOutDeg(); v++) 
            {
                const int outNode = NodeI.GetOutNId(v);

                if (colors->GetDat(outNode) == color) {
                    Queue.Push(outNode);
                }
            }
        }
    }
    Queue.Push(startNode);
    while (!Queue.Empty())
    {
        const int node = Queue.Top();
        Queue.Pop();
        const int nodeColor = colors->GetDat(node);

		if (nodeColor == fwColor)
        {
            colors->AddDat(node, sccColor);

            //Get node iterator for the current node
            const TNGraph::TNodeI NodeI = pgraph->GetNI(node);

            //Add all out edges that have not already been visited to the queue
            for (v = 0; v < NodeI.GetInDeg(); v++)
            {
                const int inNode = NodeI.GetInNId(v);
                const int inNodeColor = colors->GetDat(inNode);

                if (inNodeColor == fwColor)
                {
                    Queue.Push(inNode);
                }
            }
        }
    }
    return fwColor;
}

int bfs::parfwbfs(enhancedgraph *g, int color, int startNode) {
	TSnapQueue<int> Queue;
	TIntH *colors = g->colors;
	PNGraph pgraph = g->graph;

	Queue.Push(startNode);

	const int sccColor = g->colorGen->getNext();
	const int fwColor = g->colorGen->getNext();

	int v = 0;

	while (!Queue.Empty())
	{
		int qsize = Queue.Len();

		#pragma omp parallel for 
		for (int i = 0; i<qsize; i++)
		{
			int node = 0;

			#pragma omp critical
			{
			node = Queue.Top();
			Queue.Pop();
			}

			if (colors->GetDat(node) == color)
			{
				colors->AddDat(node, fwColor);

				//Get node iterator for the current node
				const TNGraph::TNodeI NodeI = pgraph->GetNI(node);

				//Add all out edges that have not already been visited to the queue
				for (v = 0; v < NodeI.GetOutDeg(); v++)
				{
					const int outNode = NodeI.GetOutNId(v);

					if (colors->GetDat(outNode) == color)
					{
						#pragma omp critical
						Queue.Push(outNode);
					}
				}
			}
		}
	}

	Queue.Push(startNode);

	while (!Queue.Empty())
	{
		int qsize = Queue.Len();

		#pragma omp parallel for 
		for (int i = 0; i < qsize; i++)
		{
			int node = 0;

			#pragma omp critical
			{
			node = Queue.Top();
			Queue.Pop();
			}

			const int nodeColor = colors->GetDat(node);

			if (nodeColor == fwColor)
			{
				colors->AddDat(node, sccColor);

				//Get node iterator for the current node
				const TNGraph::TNodeI NodeI = pgraph->GetNI(node);

				//Add all out edges that have not already been visited to the queue
				for (v = 0; v < NodeI.GetInDeg(); v++)
				{
					const int inNode = NodeI.GetInNId(v);
					const int inNodeColor = colors->GetDat(inNode);

					if (inNodeColor == fwColor)
					{
						#pragma omp critical
						Queue.Push(inNode);
					}
				}
			}
		}
	}
	return fwColor;
}