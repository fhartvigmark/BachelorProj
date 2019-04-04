#pragma once

#include "Snap.h"
#include "bfs.h"
#include "trim.h"
#include "pivot.h"

class fwbw{
    public:
      static int basicFWBW(PNGraph g, int trimlevel, int pivotmethod);
};