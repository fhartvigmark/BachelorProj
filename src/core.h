#pragma once
#include "Snap.h"
#include <atomic>

class ColorGenerator
{
    protected:
        int getNext() { return ++lastColor; }
    private:
        std::atomic<int> lastColor;
};

struct enhancedgraph
{
    PNGraph *graph;
    TIntH *colors;
    ColorGenerator *colorGen;
};