#pragma once
#include "core.h"

class trim
{
  private:
	static int trim1(enhancedgraph *g, int color);
	static int trim2(enhancedgraph *g, int color);
	static int trim3(enhancedgraph *g, int color);
	static int partrim1(enhancedgraph *g, int color);
	static int partrim2(enhancedgraph *g, int color);
	static int partrim3(enhancedgraph *g, int color);
  public:
	static int doTrim(int trimlevel, enhancedgraph *g, int color);
	static int doParTrim(int trimlevel, enhancedgraph *g, int color);
};