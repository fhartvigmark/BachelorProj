#include "scc.h"

int scc::FindSCCs(enhancedgraph *enhgraph, int trimlevel, int pivotmethod, int fwbwmethod){
	cout << "Starting\n";

    fwbw::FWBW(enhgraph, trimlevel, pivotmethod, 0, fwbwmethod, enhgraph->colors->BegI(), enhgraph->colors->EndI()-1);

	cout << "Done\n";
};
