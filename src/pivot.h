#pragma once
#include "core.h"

class pivot
{
  public:
    static int getPivot(enhancedgraph *g, int color);
    static int getPivotMaxDegree(enhancedgraph *g, int color);
    static int getPivotMaxDegreeColor(enhancedgraph *g, int color);
};