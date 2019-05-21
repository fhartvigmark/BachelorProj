#include "scc.h"

int scc::FindSCCs(enhancedgraph *enhgraph, int trimlevel, int pivotmethod, int fwbwmethod){
	cout << "Starting\n";

    fwbw::FWBW(enhgraph, trimlevel, pivotmethod, 0, fwbwmethod);

	cout << "Done\n";
};
