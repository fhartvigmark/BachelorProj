#pragma once

#include "core.h"
#include "fwbw.h"
#include <iostream>
using namespace std;

class scc
{
    public:
      static int FindSCCs(enhancedgraph *enhgraph, const PNGraph& g, int trimlevel, int pivotmethod, int fwbwmethod);
};
