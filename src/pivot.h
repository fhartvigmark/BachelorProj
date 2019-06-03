#pragma once
#include "core.h"
#include <stdio.h>
#include <time.h>
#include <tuple>

struct Compare { long val; int node; };    
#pragma omp declare reduction(maximum : struct Compare : omp_out = omp_in.val > omp_out.val ? omp_in : omp_out) initializer(omp_priv={-1,-1})

class pivot
{
  public:
	static std::tuple<int, int, int> findPivot(enhancedgraph *g, int color, int method, int low, int high);
	static std::tuple<int, int, int> findParPivot(enhancedgraph *g, int color, int method, int low, int high);
  private:
    static std::tuple<int, int, int> getPivot(enhancedgraph *g, int color, int low, int high);
    static std::tuple<int, int, int> getPivotMaxDegree(enhancedgraph *g, int color, int low, int high);
    static std::tuple<int, int, int> getPivotMaxDegreeColor(enhancedgraph *g, int color, int low, int high);
	static std::tuple<int, int, int> getPivotRand(enhancedgraph *g, int color, int low, int high);
	static std::tuple<int, int, int> getParPivot(enhancedgraph *g, int color, bool parallel, int low, int high);
    static std::tuple<int, int, int> getParPivotMaxDegree(enhancedgraph *g, int color, bool parallel, int low, int high);
    static std::tuple<int, int, int> getParPivotMaxDegreeCalc(enhancedgraph *g, int color, bool parallel, int low, int high);
    static std::tuple<int, int, int> getParPivotMaxDegreeColor(enhancedgraph *g, int color, bool parallel, int low, int high);
	static std::tuple<int, int, int> getParPivotRand(enhancedgraph *g, int color, bool parallel, int low, int high);
};