#pragma once

#include "core.h"
#include "fwbw.h"
#include <iostream>
using namespace std;

class scc
{
    public:
      static int FindSCCs(enhancedgraph *enhgraph, int trimlevel, int pivotmethod, int fwbwmethod);
};
