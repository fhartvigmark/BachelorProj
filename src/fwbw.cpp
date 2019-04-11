#include "fwbw.h"

//Coloring based basic fw-bw
int fwbw::basicFWBW(enhancedgraph *g, int trimlevel, int pivotmethod){

    int startColor = 0; //Temp
    //Find pivot node
    int startNode = pivot::getPivot(g, startColor);
};