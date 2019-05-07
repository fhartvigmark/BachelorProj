#pragma once
#include "Snap.h"
#include <atomic>

class ColorGenerator
{
    public:
        int getNext() { return ++lastColor; }
		ColorGenerator();
    private:
        std::atomic<int> lastColor;
};

class enhancedgraph
{
	public:
    	PNGraph graph;
		TIntH *colors;
		TIntV *NIds;
    	ColorGenerator *colorGen;
		enhancedgraph(PNGraph g);
		enhancedgraph();
};