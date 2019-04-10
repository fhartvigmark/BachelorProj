#pragma once

#include "base.h"
#include "fwbw.h"

class scc
{
    public:
      static int FindSCCs(PNGraph g, int trimlevel, int pivotmethod, int fwbwmethod);
};
