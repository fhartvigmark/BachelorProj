#include "fwbw.h"

int fwbw::FWBW(enhancedgraph *g, int trimlevel, int pivotmethod, int startColor, int method) {
	switch (method){
        case 0:
            return fwbw::basicFWBW(g, trimlevel, pivotmethod, startColor);
		case 1:
            return fwbw::parFWBW(g, trimlevel, pivotmethod, startColor);	
		case 2:
			return fwbw::recFWBW(g, trimlevel, pivotmethod, startColor);		
    }
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
	std::pair<int, int> newColors = bfs::parbfs(g, startColor, startNode);
	g->endTimer(start, eTimer::FWBWs);

	#pragma omp parallel 
	{
		#pragma omp task
		{
			recFWBW(g, trimlevel, pivotmethod, startColor);
		}

		#pragma omp task
		{
			recFWBW(g, trimlevel, pivotmethod, newColors.first);
		}

		#pragma omp task
		{
			recFWBW(g, trimlevel, pivotmethod, newColors.second);
		}
		
	}
	return 0;
};