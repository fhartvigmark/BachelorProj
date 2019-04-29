#include "fwbw.h"

//Coloring based basic fw-bw
int fwbw::basicFWBW(enhancedgraph *g, int trimlevel, int pivotmethod, int startColor){
    //Find pivot node
    int startNode = pivot::getPivot(g, startColor);
    if (startNode==-1){
        return -1;
    }
    //First: fwColor, Second: bwColor
    std::pair<int,int> newColors = bfs::colorbfs(g, startColor, startNode);
    basicFWBW(g, trimlevel,pivotmethod, startColor);
    basicFWBW(g, trimlevel, pivotmethod, newColors.first);
    basicFWBW(g, trimlevel, pivotmethod, newColors.second);
    return 0;
};