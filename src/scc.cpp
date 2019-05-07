#include "scc.h"
#include <iostream>
#include <fstream>
using namespace std;

int scc::FindSCCs(PNGraph g, int trimlevel, int pivotmethod, int fwbwmethod){
    enhancedgraph *enhgraph;
    enhgraph = new enhancedgraph(g);
    
	cout << "Starting\n";

    fwbw::FWBW(enhgraph, trimlevel, pivotmethod, 0, fwbwmethod);
	
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
