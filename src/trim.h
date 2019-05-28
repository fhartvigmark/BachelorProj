#pragma once
#include "core.h"
#include <tuple>

class trim
{
  private:
	static int trim1(enhancedgraph *g, int color);
	static int trim2(enhancedgraph *g, int color);
	static int trim3(enhancedgraph *g, int color);
	static std::pair<int, float> partrim1(enhancedgraph *g, int color, bool parallel, int low, int high);
	static std::pair<int, float> partrim2(enhancedgraph *g, int color, bool parallel, int low, int high);
	static std::pair<int, float> partrim3(enhancedgraph *g, int color, bool parallel, int low, int high);
  public:
	static std::pair<int, float> doTrim(int trimlevel, enhancedgraph *g, int color, int low, int high);
	static std::pair<int, float> doParTrim(int trimlevel, enhancedgraph *g, int color, int low, int high);
};