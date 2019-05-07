#pragma once
#include "core.h"

class pivot
{
  public:
	static int findPivot(enhancedgraph *g, int color, int method);
	
    static int getPivot(enhancedgraph *g, int color);
    static int getPivotMaxDegree(enhancedgraph *g, int color);
    static int getPivotMaxDegreeColor(enhancedgraph *g, int color);
	static int getParPivot(enhancedgraph *g, int color);
    static int getParPivotMaxDegree(enhancedgraph *g, int color);
    static int getParPivotMaxDegreeColor(enhancedgraph *g, int color);
	//TODO: Add randomwalk pivot
};