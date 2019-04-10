#pragma once

#include "base.h"
#include "bfs.h"
#include "trim.h"
#include "pivot.h"

class fwbw{
    public:
      static int basicFWBW(enhancedgraph *g, int trimlevel, int pivotmethod);
};