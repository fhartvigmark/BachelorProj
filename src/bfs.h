#pragma once
#include "core.h"


class bfs
{
  public:
    static std::pair<int, int> colorbfs(enhancedgraph *g, int color, int startNode);
	static std::pair<int, int> parbfs(enhancedgraph *g, int color, int startNode);
};
