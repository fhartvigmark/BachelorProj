#include "main.h"

void printTime(enhancedgraph *enhgraph) {
	cout << "\nTime used: \n";
	cout << "  " << "Setup\t\t" << enhgraph->getTime(eTimer::SETUP) << "ms\n";
	cout << "  " << "SCC\t\t" << enhgraph->getTime(eTimer::MAIN) << "ms\n";
	cout << "  " << "First FWBW\t" << enhgraph->getTime(eTimer::FirstFWBW) << "ms\n";
	cout << "  " << "FWBW\t\t" << enhgraph->getTime(eTimer::FWBWs) << "ms\n";
	cout << "  " << "Trim\t\t" << enhgraph->getTime(eTimer::TRIM) << "ms\n";
	cout << "  " << "Pivot\t\t" << enhgraph->getTime(eTimer::PIVOT) << "ms\n";
}

void printFile(enhancedgraph *enhgraph) {
	cout << "\nWriting output\n";

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
}

void bootstrap(char **argv) {
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
}

int main(int argc, char **argv)
{
	bootstrap(argv);

    // Input Parameters
    Env = TEnv(argc, argv, TNotify::StdNotify);
    Env.PrepArgs(TStr::Fmt("SCC. build: %s, %s. Time: %s",
                           __TIME__, __DATE__, TExeTm::GetCurTm()));
    const TStr InEdges =
        Env.GetIfArgPrefixStr("-g=", "fb1.edges", "Input graph (directed)\t\t");
    const int Trimlevels =
        Env.GetIfArgPrefixInt("-t=", 0, "Input number of trim levels\t");
    const int PivotMethod =
        Env.GetIfArgPrefixInt("-p=", 0, "Pivot selection (0 for random)\t");
    const int FwBwMethod =
        Env.GetIfArgPrefixInt("-m=", 0, "Specify FW-BW variant\t\t");
	const bool Timer =
        Env.GetIfArgPrefixInt("-time=", false, "Time execution\t\t\t");
	const bool Output =
        Env.GetIfArgPrefixInt("-out=", false, "Print SCC output to file\t");
	const bool Help =
        Env.GetIfArgPrefixInt("-h=", false, "Print help section\t\t");

	if (Help)
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
		cout << "    " << "6 = randomwalk" << "\n";
		cout << "    " << "7 = parallel randomwalk" << "\n";

		cout << "  " << "FWBW method:" << "\n";
		cout << "    " << "0 = basic FWBW" << "\n";
		cout << "    " << "1 = FWBW with parallel bfs" << "\n";
		cout << "    " << "2 = Recursive FWBW" << "\n";

		return 0;
	}
	
	cout << "\n";

    // Load the graph 
	cout << "Loading graph\n";
	TimePoint start;
	if (Timer) {
		start = Time::now();
	}
    PNGraph Graph = TSnap::LoadEdgeList<PNGraph>(InEdges);
	

	enhancedgraph *enhgraph;
    enhgraph = new enhancedgraph(Graph, Timer, 10);
	
	enhgraph->endTimer(start, eTimer::SETUP);
	cout << "Graph loaded\n";

	
	start = enhgraph->startTimer();
	scc::FindSCCs(enhgraph, Trimlevels, PivotMethod, FwBwMethod);
	enhgraph->endTimer(start, eTimer::MAIN);

	//TODO add printing and timing handler here
	if (Timer){
		printTime(enhgraph);
	}

	if (Output) {
		printFile(enhgraph);
	}

    return 0;
}