#pragma once

#include "core.h"
#include "bfs.h"
#include "trim.h"
#include "pivot.h"
#include <tuple>


class fwbw{
	public:
		static int FWBW(enhancedgraph *g, TNGraph* graph, int trimlevel, int pivotmethod, int startColor, int method, int low, int high);
    private:
     	static int basicFWBW(enhancedgraph *g, TNGraph* graph, int trimlevel, int pivotmethod, int startColor, int depth, int low, int high);
	 	static int parFWBW(enhancedgraph *g, TNGraph* graph, int trimlevel, int pivotmethod, int startColor, int depth, int low, int high);
		static int recFWBW(enhancedgraph *g, TNGraph* graph, int trimlevel, int pivotmethod, int startColor, int depth, int low, int high);
		static int dynamicFWBW(enhancedgraph *g, TNGraph* graph, int trimlevel, int pivotmethod, int startColor, int depth, int low, int high);
};