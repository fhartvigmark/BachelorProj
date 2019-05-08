#include "main.h"
#include "scc.h"
#include <iostream>
using namespace std;

int main(int argc, char **argv)
{
	//https://stackoverflow.com/questions/46482468/enable-cancellation-of-openmp-threads-from-inside-program
	char *hasCancel = getenv("OMP_CANCELLATION");
	if (hasCancel == nullptr) {
		printf("Bootstrapping...");
		setenv("OMP_CANCELLATION", "true", 1);
		// Restart the program here
		int output = execvp(argv[0], argv);
		// Execution should not continue past here
		printf("Bootstrapping failed with code %d\n",output);
		exit(1);
	} else {
		puts("Bootstrapping complete");
	}

    // Input Parameters
    Env = TEnv(argc, argv, TNotify::StdNotify);
    Env.PrepArgs(TStr::Fmt("SCC. build: %s, %s. Time: %s",
                           __TIME__, __DATE__, TExeTm::GetCurTm()));
    const TStr InEdges =
        Env.GetIfArgPrefixStr("-g=", "fb1.edges", "Input graph (directed)");
    const TInt Trimlevels =
        Env.GetIfArgPrefixInt("-t=", 0, "Input number of trim levels");
    const TInt PivotMethod =
        Env.GetIfArgPrefixInt("-p=", 0, "Pivot selection (0 for random)");
    const TInt FwBwMethod =
        Env.GetIfArgPrefixInt("-m=", 0, "Specify FW-BW variant");
	const TInt Timer =
        Env.GetIfArgPrefixInt("-time=", 0, "Time execution");
	const TInt Output =
        Env.GetIfArgPrefixInt("-out=", 0, "Print SCC output to file");
	const TInt Help =
        Env.GetIfArgPrefixInt("-h=", 0, "Print help section");

	if (Help > 0)
	{
		cout << "\n" << "parameters:" << "\n";
		cout << "  " << "Trim levels:" << "\n";
		cout << "    " << "0 = no trim" << "\n";
		cout << "    " << "1 = trim-1" << "\n";
		cout << "    " << "2 = trim-1,2" << "\n";
		cout << "    " << "3 = trim-1,2,3" << "\n";
		//TODO: partrim???
		cout << "  " << "Pivot methods:" << "\n";
		cout << "    " << "0 = first occurrence" << "\n";
		cout << "    " << "1 = max degree product" << "\n";
		cout << "    " << "2 = max degree product with colors" << "\n";
		cout << "    " << "3 = parallel version of 0" << "\n";
		cout << "    " << "4 = parallel version of 1" << "\n";
		cout << "    " << "5 = parallel version of 2" << "\n";

		cout << "  " << "FWBW method:" << "\n";
		cout << "    " << "0 = basic FWBW" << "\n";
		cout << "    " << "1 = FWBW with parallel bfs" << "\n";
		cout << "    " << "2 = Recursive FWBW" << "\n";

		return 0;
	}
	
	cout << "\n";

    // Load the graph 
	cout << "Loading graph\n";
    PNGraph Graph = TSnap::LoadEdgeList<PNGraph>(InEdges);
	cout << "Graph loaded\n";


	scc::FindSCCs(Graph, Trimlevels, PivotMethod, FwBwMethod);

	//TODO add printing and timing handler here

    return 0;
}