#pragma once

#include "Snap.h"
#include "fwbw.h"

class scc
{
    public:
      static int FindSCCs(PNGraph g, int trimlevel, int pivotmethod, int fwbwmethod);
};
