#include "fwbw.h"
#include "iostream"

int fwbw::FWBW(enhancedgraph *g, int trimlevel, int pivotmethod, int startColor, int method, int low, int high)
{
	std::pair<int, int> trimOut = std::make_pair(low, high);
	if (trimlevel > 0)
	{
		TimePoint start = g->startTimer();
		trimOut = trim::doParTrim(1, g, startColor, low, high);
		g->endTimer(start, eTimer::FirstTRIM);
	}

	std::tuple<int, int, int> startNode = pivot::findParPivot(g, startColor, pivotmethod, trimOut.first, trimOut.second);
	if (std::get<0>(startNode) == -1)
	{
		return -1;
	}

	//std::cout << std::get<1>(startNode) << ", " << std::get<2>(startNode) << "\n";

	TimePoint start = g->startTimer();
	std::pair<int, int> newColors = bfs::relaxedSearch(g, startColor, std::get<0>(startNode));
	g->endTimer(start, eTimer::FirstFWBW);

	g->reportFWBW(1);

	switch (method)
	{
	case 0:
		fwbw::basicFWBW(g, trimlevel, pivotmethod, startColor, 1, std::get<1>(startNode), std::get<2>(startNode));
		fwbw::basicFWBW(g, trimlevel, pivotmethod, newColors.first, 1, std::get<1>(startNode), std::get<2>(startNode));
		fwbw::basicFWBW(g, trimlevel, pivotmethod, newColors.second, 1, std::get<1>(startNode), std::get<2>(startNode));
		break;
	case 1:
		fwbw::parFWBW(g, trimlevel, pivotmethod, startColor, 1, std::get<1>(startNode), std::get<2>(startNode));
		fwbw::parFWBW(g, trimlevel, pivotmethod, newColors.first, 1, std::get<1>(startNode), std::get<2>(startNode));
		fwbw::parFWBW(g, trimlevel, pivotmethod, newColors.second, 1, std::get<1>(startNode), std::get<2>(startNode));
		break;
	case 2:
		#pragma omp parallel
		{
			#pragma omp single nowait
			{
				#pragma omp task shared(g)
				{
					recFWBW(g, trimlevel, pivotmethod, startColor, 1, std::get<1>(startNode), std::get<2>(startNode));
				}

				#pragma omp task shared(g)
				{
					recFWBW(g, trimlevel, pivotmethod, newColors.first, 1, std::get<1>(startNode), std::get<2>(startNode));
				}

				#pragma omp task shared(g)
				{
					recFWBW(g, trimlevel, pivotmethod, newColors.second, 1, std::get<1>(startNode), std::get<2>(startNode));
				}
			}
			#pragma omp taskwait
		}
		break;
	}
	return 0;
}

//Coloring based basic fw-bw
int fwbw::basicFWBW(enhancedgraph *g, int trimlevel, int pivotmethod, int startColor, int depth, int low, int high){
    //Find pivot node
    std::tuple<int, int, int> startNode = pivot::findPivot(g, startColor, pivotmethod, low, high);
    if (std::get<0>(startNode) == -1) {
        return -1;
    }

	std::pair<int, int> trimOut = trim::doTrim(trimlevel, g, startColor, std::get<1>(startNode), std::get<2>(startNode));

	//First: fwColor, Second: bwColor
	TimePoint start = g->startTimer();
    std::pair<int,int> newColors = bfs::colorbfs(g, startColor, std::get<0>(startNode));
	g->endTimer(start, eTimer::FWBWs);

	depth++;
	g->reportFWBW(depth);

    basicFWBW(g, trimlevel, pivotmethod, startColor, depth, trimOut.first, trimOut.second);
    basicFWBW(g, trimlevel, pivotmethod, newColors.first, depth, trimOut.first, trimOut.second);
    basicFWBW(g, trimlevel, pivotmethod, newColors.second, depth, trimOut.first, trimOut.second);
    return 0;
};

//Coloring based parallel fw-bw
int fwbw::parFWBW(enhancedgraph *g, int trimlevel, int pivotmethod, int startColor, int depth, int low, int high)
{
	//Find pivot node
	std::tuple<int, int, int> startNode = pivot::findParPivot(g, startColor, pivotmethod, low, high);
	if (std::get<0>(startNode) == -1)
	{
		return -1;
	}

	//std::cout << std::get<1>(startNode) << ", " << std::get<2>(startNode) << "\n";

	std::pair<int, int> trimOut = trim::doParTrim(trimlevel, g, startColor, std::get<1>(startNode), std::get<2>(startNode));

	//First: fwColor, Second: bwColor
	TimePoint start = g->startTimer();
	std::pair<int, int> newColors = bfs::relaxedSearch(g, startColor, std::get<0>(startNode));
	g->endTimer(start, eTimer::FWBWs);

	depth++;
	g->reportFWBW(depth);

	parFWBW(g, trimlevel, pivotmethod, startColor, depth, trimOut.first, trimOut.second);
	parFWBW(g, trimlevel, pivotmethod, newColors.first, depth, trimOut.first, trimOut.second);
	parFWBW(g, trimlevel, pivotmethod, newColors.second, depth, trimOut.first, trimOut.second);
	return 0;
};

//Coloring based recursive fw-bw
int fwbw::recFWBW(enhancedgraph *g, int trimlevel, int pivotmethod, int startColor, int depth, int low, int high)
{
	//Find pivot node
	std::tuple<int, int, int> startNode = pivot::findPivot(g, startColor, pivotmethod, low, high);
	if (std::get<0>(startNode) == -1)
	{
		return -1;
	}

	std::pair<int, int> trimOut = trim::doTrim(trimlevel, g, startColor, std::get<1>(startNode), std::get<2>(startNode));
	
	//First: fwColor, Second: bwColor
	TimePoint start = g->startTimer();
	std::pair<int, int> newColors = bfs::colorbfs(g, startColor, std::get<0>(startNode));
	g->endTimer(start, eTimer::FWBWs);

	depth++;
	g->reportFWBW(depth);

	#pragma omp parallel
	{
		#pragma omp single nowait
		{
			#pragma omp task shared(g)
			{
				recFWBW(g, trimlevel, pivotmethod, startColor, depth, trimOut.first, trimOut.second);
			}

			#pragma omp task shared(g)
			{
				recFWBW(g, trimlevel, pivotmethod, newColors.first, depth, trimOut.first, trimOut.second);
			}

			#pragma omp task shared(g)
			{
				recFWBW(g, trimlevel, pivotmethod, newColors.second, depth, trimOut.first, trimOut.second);
			}
		}
		#pragma omp taskwait
	}


	return 0;
};