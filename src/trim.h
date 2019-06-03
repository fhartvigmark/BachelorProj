#pragma once
#include "core.h"
#include <tuple>

class trim
{
  private:
	static int trim1(enhancedgraph *g, int color);
	static int trim2(enhancedgraph *g, int color);
	static int trim3(enhancedgraph *g, int color);
	static std::tuple<int, int, int> partrim1(enhancedgraph *g, const PNGraph& graph, int color, bool parallel, int low, int high);
	static std::tuple<int, int, int> partrim2(enhancedgraph *g, const PNGraph& graph, int color, bool parallel, int low, int high);
	static std::tuple<int, int, int> partrim3(enhancedgraph *g, const PNGraph& graph, int color, bool parallel, int low, int high);
  public:
	static std::tuple<int, int, int, int, int> doTrim(int trimlevel, const PNGraph& graph, enhancedgraph *g, int color, int low, int high);
	static std::tuple<int, int, int, int, int> doParTrim(int trimlevel, const PNGraph& graph, enhancedgraph *g, int color, int low, int high);
};