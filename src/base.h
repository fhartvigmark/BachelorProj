#include "Snap.h"
#include "hash.h"

struct enhancedgraph
{
    PNGraph *graph;
    TIntH *colors;
};