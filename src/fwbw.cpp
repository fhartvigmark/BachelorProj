#include "fwbw.h"

int fwbw::FWBW(enhancedgraph *g, int trimlevel, int pivotmethod, int startColor, int method) {
	switch (method){
        case 0:
            return fwbw::basicFWBW(g, trimlevel, pivotmethod, startColor);
		case 1:
            return fwbw::parFWBW(g, trimlevel, pivotmethod, startColor);			
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
    std::pair<int,int> newColors = bfs::colorbfs(g, startColor, startNode);
    basicFWBW(g, trimlevel, pivotmethod, startColor);
    basicFWBW(g, trimlevel, pivotmethod, newColors.first);
    basicFWBW(g, trimlevel, pivotmethod, newColors.second);
    return 0;
};

//Coloring based basic fw-bw
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
	std::pair<int, int> newColors = bfs::parbfs(g, startColor, startNode);
	parFWBW(g, trimlevel, pivotmethod, startColor);
	parFWBW(g, trimlevel, pivotmethod, newColors.first);
	parFWBW(g, trimlevel, pivotmethod, newColors.second);
	return 0;
};