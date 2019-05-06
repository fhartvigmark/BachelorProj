#include "scc.h"
#include <iostream>
#include <fstream>
using namespace std;

int scc::FindSCCs(PNGraph g, int trimlevel, int pivotmethod, int fwbwmethod){
    enhancedgraph *enhgraph;
    enhgraph = new enhancedgraph(g);
    
	cout << "Starting\n";

    switch (fwbwmethod){
        case 0:
            fwbw::basicFWBW(enhgraph, trimlevel, pivotmethod, 0);
    }

	cout << "Writing output\n";

	ofstream file;
	file.open("output.txt");

	TIntH *colors = enhgraph->colors;
	PNGraph graph = enhgraph->graph;

	for (TNGraph::TNodeI NI = graph->BegNI(); NI < graph->EndNI(); NI++)
    {
		int node = NI.GetId();
		file << node << " " << colors->GetDat(node) << "\n";
	}

	file.close();

	cout << "Done\n";
};
