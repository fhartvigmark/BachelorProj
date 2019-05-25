#include "fwbw.h"

int fwbw::FWBW(enhancedgraph *g, int trimlevel, int pivotmethod, int startColor, int method)
{

	if (trimlevel > 0)
	{
		TimePoint start = g->startTimer();
		trim::doParTrim(1, g, startColor);
		g->endTimer(start, eTimer::FirstTRIM);
	}

	int startNode = pivot::findParPivot(g, startColor, pivotmethod);
	if (startNode == -1)
	{
		return -1;
	}

	TimePoint start = g->startTimer();
	std::pair<int, int> newColors = bfs::parbfs(g, startColor, startNode);
	g->endTimer(start, eTimer::FirstFWBW);

	g->reportFWBW(1);

	switch (method)
	{
	case 0:
		fwbw::basicFWBW(g, trimlevel, pivotmethod, startColor, 1);
		fwbw::basicFWBW(g, trimlevel, pivotmethod, newColors.first, 1);
		fwbw::basicFWBW(g, trimlevel, pivotmethod, newColors.second, 1);
		break;
	case 1:
		fwbw::parFWBW(g, trimlevel, pivotmethod, startColor, 1);
		fwbw::parFWBW(g, trimlevel, pivotmethod, newColors.first, 1);
		fwbw::parFWBW(g, trimlevel, pivotmethod, newColors.second, 1);
		break;
	case 2:
		#pragma omp parallel
		{
			#pragma omp single nowait
			{
				#pragma omp task shared(g)
				{
					recFWBW(g, trimlevel, pivotmethod, startColor, 1);
				}

				#pragma omp task shared(g)
				{
					recFWBW(g, trimlevel, pivotmethod, newColors.first, 1);
				}

				#pragma omp task shared(g)
				{
					recFWBW(g, trimlevel, pivotmethod, newColors.second, 1);
				}
			}
			#pragma omp taskwait
		}
		break;
	}
	return 0;
}

//Coloring based basic fw-bw
int fwbw::basicFWBW(enhancedgraph *g, int trimlevel, int pivotmethod, int startColor, int depth){
    //Find pivot node
    int startNode = pivot::findPivot(g, startColor, pivotmethod);
    if (startNode==-1){
        return -1;
    }

	trim::doTrim(trimlevel, g, startColor);
	//First: fwColor, Second: bwColor
	TimePoint start = g->startTimer();
    std::pair<int,int> newColors = bfs::colorbfs(g, startColor, startNode);
	g->endTimer(start, eTimer::FWBWs);

	depth++;
	g->reportFWBW(depth);

    basicFWBW(g, trimlevel, pivotmethod, startColor, depth);
    basicFWBW(g, trimlevel, pivotmethod, newColors.first, depth);
    basicFWBW(g, trimlevel, pivotmethod, newColors.second, depth);
    return 0;
};

//Coloring based parallel fw-bw
int fwbw::parFWBW(enhancedgraph *g, int trimlevel, int pivotmethod, int startColor, int depth)
{
	//Find pivot node
	int startNode = pivot::findParPivot(g, startColor, pivotmethod);
	if (startNode == -1)
	{
		return -1;
	}

	trim::doParTrim(trimlevel, g, startColor);
	//First: fwColor, Second: bwColor
	TimePoint start = g->startTimer();
	std::pair<int, int> newColors = bfs::parbfs(g, startColor, startNode);
	g->endTimer(start, eTimer::FWBWs);

	depth++;
	g->reportFWBW(depth);

	parFWBW(g, trimlevel, pivotmethod, startColor, depth);
	parFWBW(g, trimlevel, pivotmethod, newColors.first, depth);
	parFWBW(g, trimlevel, pivotmethod, newColors.second, depth);
	return 0;
};

//Coloring based recursive fw-bw
int fwbw::recFWBW(enhancedgraph *g, int trimlevel, int pivotmethod, int startColor, int depth)
{
	//Find pivot node
	int startNode = pivot::findPivot(g, startColor, pivotmethod);
	if (startNode == -1)
	{
		return -1;
	}

	trim::doTrim(trimlevel, g, startColor);
	//First: fwColor, Second: bwColor
	TimePoint start = g->startTimer();
	std::pair<int, int> newColors = bfs::colorbfs(g, startColor, startNode);
	g->endTimer(start, eTimer::FWBWs);

	depth++;
	g->reportFWBW(depth);

	#pragma omp parallel
	{
		#pragma omp single nowait
		{
			#pragma omp task shared(g)
			{
				recFWBW(g, trimlevel, pivotmethod, startColor, depth);
			}

			#pragma omp task shared(g)
			{
				recFWBW(g, trimlevel, pivotmethod, newColors.first, depth);
			}

			#pragma omp task shared(g)
			{
				recFWBW(g, trimlevel, pivotmethod, newColors.second, depth);
			}
		}
		#pragma omp taskwait
	}


	return 0;
};