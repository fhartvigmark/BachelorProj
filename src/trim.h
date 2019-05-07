#pragma once
#include "core.h"

class trim
{
  private:
	static void trimInner(enhancedgraph *g, int color, PNGraph graph, TIntH *colors, TSnapQueue<int> *Queue);
  public:
    static int trim1(enhancedgraph *g, int color);
    static int trim2(enhancedgraph *g, int color);
    static int trim3(enhancedgraph *g, int color);
	static int partrim1(enhancedgraph *g, int color);
	static int partrim2(enhancedgraph *g, int color);
	static int partrim3(enhancedgraph *g, int color);
};