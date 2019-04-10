#pragma once

#include "core.h"
#include "fwbw.h"

class scc
{
    public:
      static int FindSCCs(PNGraph g, int trimlevel, int pivotmethod, int fwbwmethod);
};
