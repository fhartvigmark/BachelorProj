#pragma once
#include "core.h"
#include <tuple>


class bfs
{
	public:
		static std::tuple<int, int, int, int, int, int, int> colorbfs(enhancedgraph *g, int color, int startNode);
		static std::tuple<int, int, int, int, int, int, int> parbfs(enhancedgraph *g, int color, int startNode);
		static std::tuple<int, int, int, int, int, int, int> relaxedSearch(enhancedgraph *g, int color, int startNode);
		static std::pair<int, int> randomRelaxedSearch(enhancedgraph *g, int color, int startNode);
		static int fwbfs(enhancedgraph *g, int color, int startNode);
		static int parfwbfs(enhancedgraph *g, int color, int startNode);
		//Try fully async "bfs"?
		//Try hybrid bfs?
};
