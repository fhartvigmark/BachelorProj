#pragma once
#include "core.h"

struct Compare { long val; int node; };    
#pragma omp declare reduction(maximum : struct Compare : omp_out = omp_in.val > omp_out.val ? omp_in : omp_out) initializer(omp_priv={-1,-1})

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