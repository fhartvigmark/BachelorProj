#include "pivot.h"
#include "iostream"

//Find a pivot node of the specified color using the given method
//Output is a tuple of pivot node, new low, new high
//Methods are 0=first occurence, 1=maxdegree, 2=color maxdegree, 3=randomwalk, 4=precomputed maxdegree
std::tuple<int, int, int> pivot::findPivot(enhancedgraph *g, int color, int method, int low, int high)
{
	std::tuple<int, int, int> retVal;
	TimePoint start = g->startTimer();

	//If range is empty return
	if (high < low)
	{
		return std::make_tuple(-1, low, high);
	}

	//Find pivot node
	switch (method)
	{
		case 0:
			retVal = pivot::getParPivot(g, color, false, low, high);
			break;
		case 1:
			retVal = pivot::getParPivotMaxDegree(g, color, false, low, high);
			break;
		case 2:
			retVal = pivot::getParPivotMaxDegreeColor(g, color, false, low, high);
			break;
		case 3:
			retVal = pivot::getParPivotRand(g, color, false, low, high);
			break;
		case 4:
			retVal = pivot::getParPivotMaxDegreeCalc(g, color, false, low, high);
			break;
	}

	//Update timers
	g->endTimer(start, eTimer::PIVOT);
	//g->reportPivot(color, std::get<0>(retVal));
	return retVal;
}

//Find a pivot node of the specified color using the given method in parallel
//Output is a tuple of pivot node, new low, new high
//Methods are 0=first occurence, 1=maxdegree, 2=color maxdegree, 3=randomwalk, 4=precomputed maxdegree
std::tuple<int, int, int> pivot::findParPivot(enhancedgraph *g, int color, int method, int low, int high)
{
	std::tuple<int, int, int> retVal;
	TimePoint start = g->startTimer();

	//If range is empty return
	if (high < low)
	{
		return std::make_tuple(-1, low, high);
	}

	//Find pivot node
	switch (method)
	{
		case 0:
			retVal = pivot::getParPivot(g, color, true, low, high);
			break;
		case 1:
			retVal = pivot::getParPivotMaxDegree(g, color, true, low, high);
			break;
		case 2:
			retVal = pivot::getParPivotMaxDegreeColor(g, color, true, low, high);
			break;
		case 3:
			retVal = pivot::getParPivotRand(g, color, true, low, high);
			break;
		case 4:
			retVal = pivot::getParPivotMaxDegreeCalc(g, color, true, low, high);
			break;
	}

	//Update timers
	g->endTimer(start, eTimer::PIVOT);
	//g->reportPivot(color, std::get<0>(retVal));
	return retVal;
}



//--------------------------------
//		Sequential pivots
//--------------------------------

//Returns the first node matching the search color or -1 if no such node exist
std::tuple<int, int, int> pivot::getPivot(enhancedgraph *g, int color, int low, int high)
{
	ColorMap *colorMap = g->colors;

	//Iterate through all nodes in range and return first occurance
	for (int i = low; i < high + 1; i++)
	{
		if (colorMap->GetDat(i) == color)
		{
			return std::make_tuple(i, i, high);
		}
	}

	return std::make_tuple(-1, -1, high);
};

//Returns the node with the highest (in-degree * out-degree)
//matching the search color or -1 if no such node exist
std::tuple<int, int, int> pivot::getPivotMaxDegree(enhancedgraph *g, int color, int low, int high)
{
	int bestNode = -1;
	int bestDegree = -1;
	ColorMap *colors = g->colors;
	PNGraph graph = g->graph;

	//Iterate through all nodes in range
	for (TNGraph::TNodeI NI = graph->BegNI(); NI < graph->EndNI(); NI++)
	{
		//Update best candidate if color matches and degree product is better
		if (colors->GetDat(NI.GetId()) == color)
		{
			int newDeg = NI.GetInDeg() * NI.GetOutDeg();
			if (newDeg > bestDegree)
			{
				bestNode = NI.GetId();
				bestDegree = newDeg;
			}
		}
	}
	return std::make_tuple(bestNode, low, high);
};

//Returns the node with the highest (in * out-degree)
//only counting edges going to nodes of the same color
//matching the search color or -1 if no such node exist
std::tuple<int, int, int> pivot::getPivotMaxDegreeColor(enhancedgraph *g, int color, int low, int high)
{
	int bestNode = -1;
	int bestDegree = -1;
	ColorMap *colors = g->colors;
	PNGraph graph = g->graph;

	//Iterate through all nodes in range
	for (TNGraph::TNodeI NI = graph->BegNI(); NI < graph->EndNI(); NI++)
	{
		//Check if the node color match the search criteria
		int node = NI.GetId();
		if (colors->GetDat(node) == color)
		{
			int inDegree = 0;
			TNGraph::TNodeI NodeI = graph->GetNI(node);

			//Count in degree
			int v = 0;
			for (v = 0; v < NodeI.GetInDeg(); v++)
			{
				const int outNode = NodeI.GetInNId(v);

				if (colors->GetDat(outNode) == color && outNode != node)
				{
					inDegree += 1;
				}
			}

			//Count out degree
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

			//Update best candidate if degree product is higher
			int newDeg = inDegree * outDegree;
			if (newDeg > bestDegree)
			{
				bestNode = NI.GetId();
				bestDegree = newDeg;
			}
		}
	}
	return std::make_tuple(bestNode, low, high);
};

//Performs a simple random walk on the output of getPivot()
std::tuple<int, int, int> pivot::getPivotRand(enhancedgraph *g, int color, int low, int high)
{
	//Find starting point for random-walk
	std::tuple<int, int, int> node = getPivot(g, color, low, high);

	//Return if no starting point could be found
	if (std::get<0>(node) == -1)
	{
		return node;
	}

	//Perform random-walk on start node
	int bestNode = Random::randwalk(g, color, std::get<0>(node), g->RAND_WALK_ITERATIONS);
	return std::make_tuple(bestNode, std::get<1>(node), std::get<2>(node));
};



//--------------------------------
// 		Parallel pivots
//--------------------------------

//Returns the first node matching the search color or -1 if no such node exist
std::tuple<int, int, int> pivot::getParPivot(enhancedgraph *g, int color, bool parallel, int low, int high)
{
	ColorMap *colorMap = g->colors;
	int retVal = -1;

	//Calculate number of threads worth using
	int max_threads = std::min((high - low) / (50 * omp_get_max_threads()), omp_get_max_threads());
	//std::cout << (high-low) << ", " << max_threads << "\n";

	//Iterate through all nodes in range in parallel and return first occurance
	#pragma omp parallel for schedule(static) num_threads(max_threads) reduction(max: retVal) firstprivate(color) if (parallel && max_threads > 0)
	for (int i = low; i < high + 1; i++)
	{
		if (colorMap->GetDat(i) == color)
		{
			retVal = i;
			#pragma omp cancel for
		}
	}

	return std::make_tuple(retVal, low, high);
};

//Returns the node with the highest (in-degree * out-degree)
//matching the search color or -1 if no such node exist
std::tuple<int, int, int> pivot::getParPivotMaxDegree(enhancedgraph *g, int color, bool parallel, int low, int high)
{
	ColorMap *colors = g->colors;
	TNGraph *graph = g->graph;
	int min_i = high;
	int max_i = low;

	//Calculate number of threads worth using
	int max_threads = std::min((high - low) / 10, omp_get_max_threads());

	struct Compare max;
	max.val = -1;
	max.node = -1;

	//Iterate through all nodes in range in parallel
	#pragma omp parallel for reduction(maximum: max) reduction(min: min_i) reduction(max: max_i) schedule(guided) if (parallel) num_threads(max_threads)
	for (int i = low; i < high + 1; i++)
	{
		const TNGraph::TNodeI NI = graph->GetNI(i);

		//Update best candidate if color matches and degree product is better
		if (colors->GetDat(i) == color)
		{
			int newDeg = NI.GetInDeg() * NI.GetOutDeg();

			if (newDeg > max.val)
			{
				max.val = newDeg;
				max.node = i;
			}

			//Update range
			if (i < min_i)
			{
				min_i = i;
			}
			if (i > max_i)
			{
				max_i = i;
			}
		}
	}

	return std::make_tuple(max.node, min_i, max_i);
};

//Returns the node with the highest (in-degree * out-degree)
//matching the search color or -1 if no such node exist, using precomputed degree numbers
std::tuple<int, int, int> pivot::getParPivotMaxDegreeCalc(enhancedgraph *g, int color, bool parallel, int low, int high)
{
	ColorMap *colors = g->colors;
	ColorMap *degree = g->degree;
	int min_i = high;
	int max_i = low;

	//Calculate number of threads worth using
	int max_threads = std::min((high - low) / 10, omp_get_max_threads());

	struct Compare max;
	max.val = -1;
	max.node = -1;

	//Iterate through all nodes in range in parallel
	#pragma omp parallel for reduction(maximum: max) reduction(min: min_i) reduction(max: max_i) schedule(guided) if (parallel) num_threads(max_threads)
	for (int i = low; i < high + 1; i++)
	{
		//Update best candidate if color matches and degree product is better
		if (colors->GetDat(i) == color)
		{
			int newDeg = degree->GetDat(i);

			if (newDeg > max.val)
			{
				max.val = newDeg;
				max.node = i;
			}

			//Update range
			if (i < min_i)
			{
				min_i = i;
			}
			if (i > max_i)
			{
				max_i = i;
			}
		}
	}

	return std::make_tuple(max.node, min_i, max_i);
};

//Returns the node with the highest (in * out-degree)
//only counting edges going to nodes of the same color
//matching the search color or -1 if no such node exist
std::tuple<int, int, int> pivot::getParPivotMaxDegreeColor(enhancedgraph *g, int color, bool parallel, int low, int high)
{
	ColorMap *colors = g->colors;
	TNGraph *graph = g->graph;
	int min_i = high;
	int max_i = low;

	//Calculate number of threads worth using
	int max_threads = std::min((high - low) / 10, omp_get_max_threads());

	struct Compare max;
	max.val = -1;
	max.node = -1;

	//Iterate through all nodes in range in parallel
	#pragma omp parallel for reduction(maximum: max) reduction(min: min_i) reduction(max: max_i) schedule(guided) if (parallel) num_threads(max_threads)
	for (int i = low; i < high + 1; i++)
	{
		const TNGraph::TNodeI NI = graph->GetNI(i);

		//Check if the node color match the search criteria
		if (colors->GetDat(i) == color)
		{
			int inDegree = 0;
			TNGraph::TNodeI NodeI = graph->GetNI(i);

			//Count in degree
			int v = 0;
			for (v = 0; v < NodeI.GetInDeg(); v++)
			{
				const int outNode = NodeI.GetInNId(v);

				if (colors->GetDat(outNode) == color && outNode != i)
				{
					inDegree += 1;
				}
			}

			//Count out degree
			int outDegree = 0;
			NodeI = graph->GetNI(i);

			for (v = 0; v < NodeI.GetOutDeg(); v++)
			{
				const int outNode = NodeI.GetOutNId(v);

				if (colors->GetDat(outNode) == color && outNode != i)
				{
					outDegree += 1;
				}
			}

			//Update best candidate if degree product is higher
			int newDeg = inDegree * outDegree;
			if (newDeg > max.val)
			{
				max.val = newDeg;
				max.node = i;
			}

			//Update range
			if (i < min_i)
			{
				min_i = i;
			}
			if (i > max_i)
			{
				max_i = i;
			}
		}
	}

	return std::make_tuple(max.node, min_i, max_i);
};

//Performs a simple random walk on the output of getParPivot()
std::tuple<int, int, int> pivot::getParPivotRand(enhancedgraph *g, int color, bool parallel, int low, int high)
{
	//Find starting point for random-walk
	std::tuple<int, int, int> node = getParPivot(g, color, parallel, low, high);
	//std::cout << "Rand found " << std::get<0>(node);

	//Return if no starting point could be found
	if (std::get<0>(node) == -1)
	{
		//std::cout << "\n";
		return node;
	}

	//Perform random-walk on start node
	int bestNode = Random::randwalk(g, color, std::get<0>(node), g->RAND_WALK_ITERATIONS);
	//std::cout << ", " << bestNode << "\n";

	return std::make_tuple(bestNode, std::get<1>(node), std::get<2>(node));
};
