#include "fwbw.h"
#include "iostream"

int fwbw::FWBW(enhancedgraph *g, TNGraph* graph, int trimlevel, int pivotmethod, int startColor, int method, int low, int high) {
	std::tuple<int, int, int, int, int> trimOut = std::make_tuple(low, high, -1, -1, -1);
	if (trimlevel > 0)
	{
		TimePoint start = g->startTimer();
		trimOut = trim::doParTrim(1, graph, g, startColor, low, high);
		g->endTimer(start, eTimer::FirstTRIM);
	}

	std::tuple<int, int, int> startNode = pivot::findParPivot(g, graph, startColor, pivotmethod, std::get<0>(trimOut), std::get<1>(trimOut));
	if (std::get<0>(startNode) == -1)
	{
		g->reportFWBW(startColor, -1, 0, 0, 0, 0, 1, 0, 0);
		return -1;
	}

	//std::cout << std::get<1>(startNode) << ", " << std::get<2>(startNode) << "\n";

	TimePoint start = g->startTimer();
	std::tuple<int, int, int, int, int, int, int> newColors = bfs::relaxedSearch(g, graph, startColor, std::get<0>(startNode));
	//std::cout << "fw " << std::get<0>(newColors) << ", " << std::get<2>(newColors) << ", " << std::get<3>(newColors) << "\n";
	//std::cout << "bw " << std::get<1>(newColors) << ", " << std::get<4>(newColors) << ", " << std::get<5>(newColors) << "\n";

	g->endTimer(start, eTimer::FirstFWBW);

	g->reportFWBW(startColor, std::get<0>(startNode), std::get<2>(trimOut), std::get<3>(trimOut), std::get<4>(trimOut), 
		std::get<6>(newColors), 1, std::get<0>(newColors), std::get<1>(newColors));

	switch (method)
	{
	case 0:
		fwbw::basicFWBW(g, graph, trimlevel, pivotmethod, startColor, 1, std::get<1>(startNode), std::get<2>(startNode));
		fwbw::basicFWBW(g, graph, trimlevel, pivotmethod, std::get<0>(newColors), 1, std::get<2>(newColors), std::get<3>(newColors));
		fwbw::basicFWBW(g, graph, trimlevel, pivotmethod, std::get<1>(newColors), 1, std::get<4>(newColors), std::get<5>(newColors));
		break;
	case 1:
		fwbw::parFWBW(g, graph, trimlevel, pivotmethod, startColor, 1, std::get<1>(startNode), std::get<2>(startNode));
		fwbw::parFWBW(g, graph, trimlevel, pivotmethod, std::get<0>(newColors), 1, std::get<2>(newColors), std::get<3>(newColors));
		fwbw::parFWBW(g, graph, trimlevel, pivotmethod, std::get<1>(newColors), 1, std::get<4>(newColors), std::get<5>(newColors));
		break;
	case 2:
		#pragma omp parallel
		{
			#pragma omp single nowait
			{
				#pragma omp task shared(g, graph)
				{
					recFWBW(g, graph, trimlevel, pivotmethod, startColor, 1, std::get<1>(startNode), std::get<2>(startNode));
				}

				#pragma omp task shared(g, graph)
				{
					recFWBW(g, graph, trimlevel, pivotmethod, std::get<0>(newColors), 1, std::get<2>(newColors), std::get<3>(newColors));
				}

				#pragma omp task shared(g, graph)
				{
					recFWBW(g, graph, trimlevel, pivotmethod, std::get<1>(newColors), 1, std::get<4>(newColors), std::get<5>(newColors));
				}
			}
			#pragma omp taskwait
		}
		break;
	case 3:
		omp_set_dynamic(1);
		#pragma omp parallel
		{
			#pragma omp single nowait
			{
				#pragma omp task shared(g, graph)
				{
					dynamicFWBW(g, graph, trimlevel, pivotmethod, startColor, 1, std::get<1>(startNode), std::get<2>(startNode));
				}

				#pragma omp task shared(g, graph)
				{
					dynamicFWBW(g, graph, trimlevel, pivotmethod, std::get<0>(newColors), 1, std::get<2>(newColors), std::get<3>(newColors));
				}

				#pragma omp task shared(g, graph)
				{
					dynamicFWBW(g, graph, trimlevel, pivotmethod, std::get<1>(newColors), 1, std::get<4>(newColors), std::get<5>(newColors));
				}
			}
			#pragma omp taskwait
		}
		break;
	}
	return 0;
}

//Coloring based basic fw-bw
int fwbw::basicFWBW(enhancedgraph *g, TNGraph* graph, int trimlevel, int pivotmethod, int startColor, int depth, int low, int high) {
	depth++;
	
    //Find pivot node
    std::tuple<int, int, int> startNode = pivot::findPivot(g, graph, startColor, pivotmethod, low, high);
    if (std::get<0>(startNode) == -1) {
		g->reportFWBW(startColor, -1, 0, 0, 0, 0, depth, 0, 0);
        return -1;
    }

	std::tuple<int, int, int, int, int> trimOut = std::make_tuple(std::get<1>(startNode), std::get<2>(startNode), -1, -1, -1);
	if (depth <= g->TRIM_CUTOFF || depth % g->TRIM_STEPS == 0) {
		trimOut = trim::doTrim(trimlevel, graph, g, startColor, std::get<1>(startNode), std::get<2>(startNode));
	}

	//First: fwColor, Second: bwColor
	TimePoint start = g->startTimer();
    std::tuple<int, int, int, int, int, int, int> newColors = bfs::colorbfs(g, graph, startColor, std::get<0>(startNode));
	g->endTimer(start, eTimer::FWBWs);

	//std::cout << "fw " << std::get<0>(newColors) << ", " << std::get<2>(newColors) << ", " << std::get<3>(newColors) << "\n";
	//std::cout << "bw " << std::get<1>(newColors) << ", " << std::get<4>(newColors) << ", " << std::get<5>(newColors) << "\n";

	g->reportFWBW(startColor, std::get<0>(startNode), std::get<2>(trimOut), std::get<3>(trimOut), std::get<4>(trimOut), 
		std::get<6>(newColors), depth, std::get<0>(newColors), std::get<1>(newColors));

    basicFWBW(g, graph, trimlevel, pivotmethod, startColor, depth, std::get<0>(trimOut), std::get<1>(trimOut));
    basicFWBW(g, graph, trimlevel, pivotmethod, std::get<0>(newColors), depth, std::get<2>(newColors), std::get<3>(newColors));
    basicFWBW(g, graph, trimlevel, pivotmethod, std::get<1>(newColors), depth, std::get<4>(newColors), std::get<5>(newColors));
    return 0;
};

//Coloring based parallel fw-bw
int fwbw::parFWBW(enhancedgraph *g, TNGraph* graph, int trimlevel, int pivotmethod, int startColor, int depth, int low, int high) {
	depth++;

	//Find pivot node
	std::tuple<int, int, int> startNode = pivot::findParPivot(g, graph, startColor, pivotmethod, low, high);
	if (std::get<0>(startNode) == -1)
	{
		g->reportFWBW(startColor, -1, 0, 0, 0, 0, depth, 0, 0);		
		return -1;
	}

	//std::cout << std::get<1>(startNode) << ", " << std::get<2>(startNode) << "\n";

	std::tuple<int, int, int, int, int> trimOut = std::make_tuple(std::get<1>(startNode), std::get<2>(startNode), -1, -1, -1);
	if (depth <= g->TRIM_CUTOFF || depth % g->TRIM_STEPS == 0) {
		trimOut = trim::doParTrim(trimlevel, graph, g, startColor, std::get<1>(startNode), std::get<2>(startNode));
	}

	//First: fwColor, Second: bwColor
	TimePoint start = g->startTimer();
	std::tuple<int, int, int, int, int, int, int> newColors = bfs::relaxedSearch(g, graph, startColor, std::get<0>(startNode));
	g->endTimer(start, eTimer::FWBWs);

	//std::cout << "fw " << std::get<0>(newColors) << ", " << std::get<2>(newColors) << ", " << std::get<3>(newColors) << "\n";
	//std::cout << "bw " << std::get<1>(newColors) << ", " << std::get<4>(newColors) << ", " << std::get<5>(newColors) << "\n";

	g->reportFWBW(startColor, std::get<0>(startNode), std::get<2>(trimOut), std::get<3>(trimOut), std::get<4>(trimOut), 
		std::get<6>(newColors), depth, std::get<0>(newColors), std::get<1>(newColors));

	parFWBW(g, graph, trimlevel, pivotmethod, startColor, depth, std::get<0>(trimOut), std::get<1>(trimOut));
	parFWBW(g, graph, trimlevel, pivotmethod, std::get<0>(newColors), depth, std::get<2>(newColors), std::get<3>(newColors));
	parFWBW(g, graph, trimlevel, pivotmethod, std::get<1>(newColors), depth, std::get<4>(newColors), std::get<5>(newColors));
	return 0;
};

//Coloring based recursive fw-bw
int fwbw::recFWBW(enhancedgraph *g, TNGraph* graph, int trimlevel, int pivotmethod, int startColor, int depth, int low, int high) {
	depth++;

	//Find pivot node
	std::tuple<int, int, int> startNode = pivot::findPivot(g, graph, startColor, pivotmethod, low, high);
	if (std::get<0>(startNode) == -1)
	{
		g->reportFWBW(startColor, -1, 0, 0, 0, 0, depth, 0, 0);
		return -1;
	}

	std::tuple<int, int, int, int, int> trimOut = std::make_tuple(std::get<1>(startNode), std::get<2>(startNode), -1, -1, -1);
	if (depth <= g->TRIM_CUTOFF || depth % g->TRIM_STEPS == 0) {
		trimOut = trim::doTrim(trimlevel, graph, g, startColor, std::get<1>(startNode), std::get<2>(startNode));
	}
	
	//First: fwColor, Second: bwColor
	TimePoint start = g->startTimer();
	std::tuple<int, int, int, int, int, int, int> newColors = bfs::colorbfs(g, graph, startColor, std::get<0>(startNode));
	g->endTimer(start, eTimer::FWBWs);

	g->reportFWBW(startColor, std::get<0>(startNode), std::get<2>(trimOut), std::get<3>(trimOut), std::get<4>(trimOut), 
		std::get<6>(newColors), depth, std::get<0>(newColors), std::get<1>(newColors));

	#pragma omp parallel
	{
		#pragma omp single nowait
		{
			#pragma omp task shared(g, graph)
			{
				recFWBW(g, graph, trimlevel, pivotmethod, startColor, depth, std::get<0>(trimOut), std::get<1>(trimOut));
			}

			#pragma omp task shared(g, graph)
			{
				recFWBW(g, graph, trimlevel, pivotmethod, std::get<0>(newColors), depth, std::get<2>(newColors), std::get<3>(newColors));
			}

			#pragma omp task shared(g, graph)
			{
				recFWBW(g, graph, trimlevel, pivotmethod, std::get<1>(newColors), depth, std::get<4>(newColors), std::get<5>(newColors));
			}
		}
		#pragma omp taskwait
	}


	return 0;
};

int fwbw::dynamicFWBW(enhancedgraph *g, TNGraph* graph, int trimlevel, int pivotmethod, int startColor, int depth, int low, int high) {
	depth++;

	//Find pivot node
	std::tuple<int, int, int> startNode = pivot::findParPivot(g, graph, startColor, pivotmethod, low, high);
	if (std::get<0>(startNode) == -1)
	{
		g->reportFWBW(startColor, -1, 0, 0, 0, 0, depth, 0, 0);
		return -1;
	}

	std::tuple<int, int, int, int, int> trimOut = std::make_tuple(std::get<1>(startNode), std::get<2>(startNode), -1, -1, -1);
	if (depth <= g->TRIM_CUTOFF || depth % g->TRIM_STEPS == 0) {
		trimOut = trim::doParTrim(trimlevel, graph, g, startColor, std::get<1>(startNode), std::get<2>(startNode));
	}
	
	//First: fwColor, Second: bwColor
	TimePoint start = g->startTimer();
	std::tuple<int, int, int, int, int, int, int> newColors = bfs::relaxedSearch(g, graph, startColor, std::get<0>(startNode));
	g->endTimer(start, eTimer::FWBWs);

	g->reportFWBW(startColor, std::get<0>(startNode), std::get<2>(trimOut), std::get<3>(trimOut), std::get<4>(trimOut), 
		std::get<6>(newColors), depth, std::get<0>(newColors), std::get<1>(newColors));

	#pragma omp parallel
	{
		#pragma omp single nowait
		{
			#pragma omp task shared(g, graph)
			{
				dynamicFWBW(g, graph, trimlevel, pivotmethod, startColor, depth, std::get<0>(trimOut), std::get<1>(trimOut));
			}

			#pragma omp task shared(g, graph)
			{
				dynamicFWBW(g, graph, trimlevel, pivotmethod, std::get<0>(newColors), depth, std::get<2>(newColors), std::get<3>(newColors));
			}

			#pragma omp task shared(g, graph)
			{
				dynamicFWBW(g, graph, trimlevel, pivotmethod, std::get<1>(newColors), depth, std::get<4>(newColors), std::get<5>(newColors));
			}
		}
		#pragma omp taskwait
	}


	return 0;
};