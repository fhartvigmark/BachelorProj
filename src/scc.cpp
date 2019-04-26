#include "scc.h"

int scc::FindSCCs(PNGraph g, int trimlevel, int pivotmethod, int fwbwmethod){
    struct enhancedgraph enhgraph;
    enhgraph.graph = g;
    TIntH colors;
    colors(g -> GetNodes());
    enhgraph.colors = &colors;
    enhgraph.colorGen = new ColorGenerator();
    
    switch (fwbwmethod){
        case 0:
            return fwbw::basicFWBW(&enhgraph, trimlevel, pivotmethod, 0);
    }
};
