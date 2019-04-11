#pragma once
#include "core.h"
#include "pivot.h"

class bfs
{
  public:
    static std::pair<int, int> colorbfs(enhancedgraph *g, int color, int startNode);
};