#include "bfs.h"
#include "iostream"

//Maybe include mark
std::pair<int, int> bfs::colorbfs(enhancedgraph *g, int color, int startNode) {
    TSnapQueue<int> Queue;
    TIntH *colors = g->colors;
    PNGraph pgraph = g->graph;
	int count = 0;

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
			count++;

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

	g->reportBFS(color, count);
    return std::make_pair(fwColor, bwColor);
}

std::pair<int, int> bfs::parbfs(enhancedgraph *g, int color, int startNode) {
	TSnapQueue<int> Queue;
	TIntH *colors = g->colors;
	PNGraph pgraph = g->graph;
	int totalCount = 0;

	Queue.Push(startNode);

	const int sccColor = g->colorGen->getNext();
	const int fwColor = g->colorGen->getNext();
	const int bwColor = g->colorGen->getNext();


	while (!Queue.Empty())
	{
		int qsize = Queue.Len();

		#pragma omp parallel for schedule(static)
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
				for (int v = 0; v < NodeI.GetOutDeg(); v++)
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
		int count = 0;

		#pragma omp parallel for reduction(+:count) schedule(static)
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
				for (int v = 0; v < NodeI.GetInDeg(); v++)
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
				count++;

				//Get node iterator for the current node
				const TNGraph::TNodeI NodeI = pgraph->GetNI(node);

				//Add all out edges that have not already been visited to the queue
				for (int v = 0; v < NodeI.GetInDeg(); v++)
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

		totalCount += count;
	}

	g->reportBFS(color, totalCount);
	return std::make_pair(fwColor, bwColor);
}

std::pair<int, int> bfs::relaxedSearch(enhancedgraph *g, int color, int startNode) {
	TIntH *colors = g->colors;
	PNGraph pgraph = g->graph;
	int totalCount = 0;
	int count = 0;
	int threads = omp_get_max_threads();
	//std::cout << "Threads " << threads << "\n";

	const int sccColor = g->colorGen->getNext();
	const int fwColor = g->colorGen->getNext();
	const int bwColor = g->colorGen->getNext();

	TSnapQueue<int> Queue;
	Queue.Push(startNode);
	int v = 0;

	//Find FW start nodes
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

		if (Queue.Len() >= threads) {
			break;
		}
    }

	//std::cout << "Queue " << Queue.Len() << "\n";

	//Find FW closure
	if (!Queue.Empty()) {
		#pragma omp parallel
		{
			TSnapQueue<int> *myQueue;
			int threadNode;
			int id = omp_get_thread_num();

			if (id == 0) {
				myQueue = &Queue;
			} else {
				myQueue = new TSnapQueue<int>();
			}

			#pragma omp critical
			{
				threadNode = Queue.Top();
				Queue.Pop();
			}
			
			
			#pragma omp barrier
			myQueue->Push(threadNode);


			int v = 0;
			while(!myQueue->Empty())
			{
				int node = myQueue->Top();
				myQueue->Pop();

				if (colors->GetDat(node) == color) {
					colors->AddDat(node, fwColor);

					//Get node iterator for the current node
					const TNGraph::TNodeI NodeI = pgraph->GetNI(node);

					//Add all out edges that have not already been visited to the queue
					for (v = 0; v < NodeI.GetOutDeg(); v++) 
					{
						const int outNode = NodeI.GetOutNId(v);

						if (colors->GetDat(outNode) == color)
						{
							myQueue->Push(outNode);
						}
					}
				}
			}

			if (id != 0) {
				delete myQueue;
			}
		}
	}
	

	//Something wrong happened
	if (!Queue.Empty()) {
		throw "relaxedSearch: FW queue not emptied";
	}

	//Find BW start nodes
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
			totalCount++;

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

		if (Queue.Len() >= threads) {
			break;
		}
    }
	
	//Find BW closure
	if (!Queue.Empty()) {
		#pragma omp parallel reduction(+:count)
		{
			TSnapQueue<int> *myQueue;
			int threadNode;
			int id = omp_get_thread_num();

			if (id == 0) {
				myQueue = &Queue;
			} else {
				myQueue = new TSnapQueue<int>();
			}

			#pragma omp critical
			{
				threadNode = Queue.Top();
				Queue.Pop();
			}
			
			
			#pragma omp barrier
			myQueue->Push(threadNode);


			int v = 0;
			while (!myQueue->Empty())
			{
				int node = myQueue->Top();
				myQueue->Pop();
				int nodeColor = colors->GetDat(node);

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
							myQueue->Push(inNode);
						}
					}
				}else if (nodeColor == fwColor)
				{
					colors->AddDat(node, sccColor);
					count++;

					//Get node iterator for the current node
					const TNGraph::TNodeI NodeI = pgraph->GetNI(node);

					//Add all out edges that have not already been visited to the queue
					for (v = 0; v < NodeI.GetInDeg(); v++)
					{
						const int inNode = NodeI.GetInNId(v);
						const int inNodeColor = colors->GetDat(inNode);

						if ((inNodeColor == color) || (inNodeColor == fwColor))
						{
							myQueue->Push(inNode);
						}
					}
				}
			}

			if (id != 0) {
				delete myQueue;
			}
		}
	}
	

	//Something wrong happened
	if (!Queue.Empty()) {
		throw "relaxedSearch: BW queue not emptied";
	}

	g->reportBFS(color, totalCount + count);
	return std::make_pair(fwColor, bwColor);
}

std::pair<int, int> bfs::randomRelaxedSearch(enhancedgraph *g, int color, int startNode) {
	TIntH *colors = g->colors;
	PNGraph pgraph = g->graph;
	int totalCount = 0;

	const int sccColor = g->colorGen->getNext();
	const int fwColor = g->colorGen->getNext();
	const int bwColor = g->colorGen->getNext();

	

	#pragma omp parallel reduction(+:totalCount)
	{
		TSnapQueue<int> *Queue = new TSnapQueue<int>();
		int id = omp_get_thread_num();
		int threadNode;
		if (id == 0) {
			threadNode = startNode;
		} else {
			threadNode = Random::randwalk(g, color, startNode, 5, id, true);
		}
		
		
		//#pragma omp critical
		//std::cout << threadNode << "\n";
		#pragma omp barrier
		Queue->Push(threadNode);
		//#pragma omp critical
		//std::cout << "Done " << id << "\n";

		

		//#pragma omp critical
		//std::cout << "Past barrier " << id << "\n";

		int v = 0;
		while(!Queue->Empty())
		{
			int node = Queue->Top();
			Queue->Pop();

			if (colors->GetDat(node) == color) {
				//#pragma omp critical
				colors->AddDat(node, fwColor);


				//Get node iterator for the current node
				const TNGraph::TNodeI NodeI = pgraph->GetNI(node);

				//Add all out edges that have not already been visited to the queue
				for (v = 0; v < NodeI.GetOutDeg(); v++) 
				{
					const int outNode = NodeI.GetOutNId(v);

					if (colors->GetDat(outNode) == color)
					{
						//#pragma omp critical
						Queue->Push(outNode);
					}
				}
			}
		}

		if (id == 0) {
			threadNode = startNode;
		} else {
			threadNode = Random::randwalk(g, color, startNode, 5, id, false);
		}
		
		//#pragma omp critical
		//std::cout << threadNode << "\n";
		#pragma omp barrier
		Queue->Push(threadNode);

		
		//#pragma omp critical
		//std::cout << "Past barrier2 " << id << "\n";
		
		while (!Queue->Empty())
		{
			int node = Queue->Top();
			Queue->Pop();
			int nodeColor = colors->GetDat(node);

			if (nodeColor == color)
			{
				//#pragma omp critical
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
						//#pragma omp critical
						Queue->Push(inNode);
					}
				}
			}else if (nodeColor == fwColor)
			{
				//#pragma omp critical
				colors->AddDat(node, sccColor);
				totalCount++;

				//Get node iterator for the current node
				const TNGraph::TNodeI NodeI = pgraph->GetNI(node);

				//Add all out edges that have not already been visited to the queue
				for (v = 0; v < NodeI.GetInDeg(); v++)
				{
					const int inNode = NodeI.GetInNId(v);
					const int inNodeColor = colors->GetDat(inNode);

					if ((inNodeColor == color) || (inNodeColor == fwColor))
					{
						//#pragma omp critical
						Queue->Push(inNode);
					}
				}
			}

			//delete Queue;
		}

		//#pragma omp critical
		//std::cout << "Complete " << id << "\n";
	}

	//for (TNGraph::TNodeI NI = pgraph->BegNI(); NI < pgraph->EndNI(); NI++)
   //{
	//	int node = NI.GetId();
	//	std::cout << node << " " << colors->GetDat(node) << "\n";
	//}

	g->reportBFS(color, totalCount);
	return std::make_pair(fwColor, bwColor);
}

//TODO: test fwbfs and parfwbfs
int bfs::fwbfs(enhancedgraph *g, int color, int startNode) {
    TSnapQueue<int> Queue;
    TIntH *colors = g->colors;
    PNGraph pgraph = g->graph;
	int count = 0;

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
			count++;

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
	g->reportBFS(color, count);
    return fwColor;
}

int bfs::parfwbfs(enhancedgraph *g, int color, int startNode) {
	TSnapQueue<int> Queue;
	TIntH *colors = g->colors;
	PNGraph pgraph = g->graph;
	int totalCount = 0;

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
		int count = 0;

		#pragma omp parallel for reduction(+:count)
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

		totalCount += count;
	}

	g->reportBFS(color, totalCount);
	return fwColor;
}