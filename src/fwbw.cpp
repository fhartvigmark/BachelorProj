#include "fwbw.h"

int fwbw::FWBW(enhancedgraph *g, int trimlevel, int pivotmethod, int startColor, int method)
{

	if (trimlevel > 0)
	{
		TimePoint start = g->startTimer();
		trim::doParTrim(1, g, startColor);
		g->endTimer(start, eTimer::FirstTRIM);
	}

	int startNode = pivot::findPivot(g, startColor, pivotmethod);
	if (startNode == -1)
	{
		return -1;
	}

	TimePoint start = g->startTimer();
	std::pair<int, int> newColors = bfs::parbfs(g, startColor, startNode);
	g->endTimer(start, eTimer::FirstFWBW);

	switch (method)
	{
	case 0:
		fwbw::basicFWBW(g, trimlevel, pivotmethod, startColor);
		fwbw::basicFWBW(g, trimlevel, pivotmethod, newColors.first);
		fwbw::basicFWBW(g, trimlevel, pivotmethod, newColors.second);
		break;
	case 1:
		fwbw::parFWBW(g, trimlevel, pivotmethod, startColor);
		fwbw::parFWBW(g, trimlevel, pivotmethod, newColors.first);
		fwbw::parFWBW(g, trimlevel, pivotmethod, newColors.second);
		break;
	case 2:
		#pragma omp parallel
		{
			#pragma omp single nowait
			{
				#pragma omp task shared(g)
				{
					recFWBW(g, trimlevel, pivotmethod, startColor);
				}

				#pragma omp task shared(g)
				{
					recFWBW(g, trimlevel, pivotmethod, newColors.first);
				}

				#pragma omp task shared(g)
				{
					recFWBW(g, trimlevel, pivotmethod, newColors.second);
				}
			}
			#pragma omp taskwait
		}
		break;
	}
	return 0;
}

//Coloring based basic fw-bw
int fwbw::basicFWBW(enhancedgraph *g, int trimlevel, int pivotmethod, int startColor){
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

    basicFWBW(g, trimlevel, pivotmethod, startColor);
    basicFWBW(g, trimlevel, pivotmethod, newColors.first);
    basicFWBW(g, trimlevel, pivotmethod, newColors.second);
    return 0;
};

//Coloring based parallel fw-bw
int fwbw::parFWBW(enhancedgraph *g, int trimlevel, int pivotmethod, int startColor)
{
	//Find pivot node
	int startNode = pivot::findPivot(g, startColor, pivotmethod);
	if (startNode == -1)
	{
		return -1;
	}

	trim::doParTrim(trimlevel, g, startColor);
	//First: fwColor, Second: bwColor
	TimePoint start = g->startTimer();
	std::pair<int, int> newColors = bfs::parbfs(g, startColor, startNode);
	g->endTimer(start, eTimer::FWBWs);

	parFWBW(g, trimlevel, pivotmethod, startColor);
	parFWBW(g, trimlevel, pivotmethod, newColors.first);
	parFWBW(g, trimlevel, pivotmethod, newColors.second);
	return 0;
};

//Coloring based recursive fw-bw
int fwbw::recFWBW(enhancedgraph *g, int trimlevel, int pivotmethod, int startColor)
{
	//Find pivot node
	int startNode = pivot::findPivot(g, startColor, pivotmethod);
	if (startNode == -1)
	{
		return -1;
	}

	trim::doParTrim(trimlevel, g, startColor);
	//First: fwColor, Second: bwColor
	TimePoint start = g->startTimer();
	std::pair<int, int> newColors = bfs::colorbfs(g, startColor, startNode);
	g->endTimer(start, eTimer::FWBWs);

	#pragma omp parallel
	{
		#pragma omp single nowait
		{
			#pragma omp task shared(g)
			{
				recFWBW(g, trimlevel, pivotmethod, startColor);
			}

			#pragma omp task shared(g)
			{
				recFWBW(g, trimlevel, pivotmethod, newColors.first);
			}

			#pragma omp task shared(g)
			{
				recFWBW(g, trimlevel, pivotmethod, newColors.second);
			}
		}
		#pragma omp taskwait
	}


	return 0;
};