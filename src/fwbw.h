#pragma once

#include "core.h"
#include "bfs.h"
#include "trim.h"
#include "pivot.h"

class fwbw{
	public:
		static int FWBW(enhancedgraph *g, int trimlevel, int pivotmethod, int startColor, int method);
    private:
     	static int basicFWBW(enhancedgraph *g, int trimlevel, int pivotmethod, int startColor, int depth);
	 	static int parFWBW(enhancedgraph *g, int trimlevel, int pivotmethod, int startColor, int depth);
		static int recFWBW(enhancedgraph *g, int trimlevel, int pivotmethod, int startColor, int depth);
};