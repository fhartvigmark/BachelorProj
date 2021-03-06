#include "main.h"

//Get extract the name of a file from its path
TStr getFileName(TStr path) 
{
	return path.GetFMid();
}

//Print timers
// operation=0		No printing
// operation=1		Print to stdout
// operation=2		Print to file at 'path' with suffix 'suffix'
// operation=3		Print compact version to file
void printTime(enhancedgraph *enhgraph, TStr path, TStr suffix, int operation) 
{
	if (operation == 1) 
	{
		//Print timers to stdout
		cout << "\nTime used: \n";
		cout << "  " << "Setup\t\t\t" << enhgraph->getTime(eTimer::SETUP) << "ms\n";
		cout << "  " << "Prep\t\t\t" << enhgraph->getTime(eTimer::PREP) << "μs\n";
		cout << "  " << "SCC\t\t\t" << enhgraph->getTime(eTimer::MAIN) << "ms\n";
		cout << "    " << "First FWBW\t\t" << enhgraph->getTime(eTimer::FirstFWBW) << "ms\n";
		cout << "    " << "FWBW\t\t" << enhgraph->getTime(eTimer::FWBWs) << "ms\n";
		cout << "    " << "Trim\t\t" << enhgraph->getTime(eTimer::TRIM) << "ms\n";
		cout << "      " << "Trim 1\t\t" << enhgraph->getTime(eTimer::TRIM1) << "ms\n";
		cout << "        " << "First Trim\t" << enhgraph->getTime(eTimer::FirstTRIM) << "ms\n";
		cout << "      " << "Trim 2\t\t" << enhgraph->getTime(eTimer::TRIM2) << "ms\n";
		cout << "      " << "Trim 3\t\t" << enhgraph->getTime(eTimer::TRIM3) << "ms\n";
		cout << "    " << "Pivot\t\t" << enhgraph->getTime(eTimer::PIVOT) << "ms\n";
	} 
	else if (operation == 2) 
	{
		cout << "\nWriting timer output\n";

		//Create output file
		TStr fileName = getFileName(path);
		fileName += suffix;
		fileName += ".time";

		ofstream file;
		file.open(fileName.GetCStr());

		//Print timers to file
		file << "\nTime used: \n";
		file << "  " << "Setup\t\t\t" << enhgraph->getTime(eTimer::SETUP) << "ms\n";
		file << "  " << "Prep\t\t\t" << enhgraph->getTime(eTimer::PREP) << "μs\n";
		file << "  " << "SCC\t\t\t" << enhgraph->getTime(eTimer::MAIN) << "ms\n";
		file << "    " << "First FWBW\t\t" << enhgraph->getTime(eTimer::FirstFWBW) << "ms\n";
		file << "    " << "FWBW\t\t" << enhgraph->getTime(eTimer::FWBWs) << "ms\n";
		file << "    " << "Trim\t\t" << enhgraph->getTime(eTimer::TRIM) << "ms\n";
		file << "      " << "Trim 1\t\t" << enhgraph->getTime(eTimer::TRIM1) << "ms\n";
		file << "        " << "First Trim\t" << enhgraph->getTime(eTimer::FirstTRIM) << "ms\n";
		file << "      " << "Trim 2\t\t" << enhgraph->getTime(eTimer::TRIM2) << "ms\n";
		file << "      " << "Trim 3\t\t" << enhgraph->getTime(eTimer::TRIM3) << "ms\n";
		file << "    " << "Pivot\t\t" << enhgraph->getTime(eTimer::PIVOT) << "ms\n";

		file.close();

		cout << "Done\n";
	} 
	else if (operation == 3) 
	{
		cout << "\nWriting timer output\n";

		//Create output file
		TStr fileName = getFileName(path);
		fileName += suffix;
		fileName += ".time";

		ofstream file;
		file.open(fileName.GetCStr());

		//Write compact timers to file
		file << enhgraph->getTime(eTimer::SETUP) << "\t";
		file << enhgraph->getTime(eTimer::PREP) << "\t";
		file << enhgraph->getTime(eTimer::MAIN) << "\t";
		file << enhgraph->getTime(eTimer::FirstFWBW) << "\t";
		file << enhgraph->getTime(eTimer::FWBWs) << "\t";
		file << enhgraph->getTime(eTimer::TRIM) << "\t";
		file << enhgraph->getTime(eTimer::TRIM1) << "\t";
		file << enhgraph->getTime(eTimer::FirstTRIM) << "\t";
		file << enhgraph->getTime(eTimer::TRIM2) << "\t";
		file << enhgraph->getTime(eTimer::TRIM3) << "\t";
		file << enhgraph->getTime(eTimer::PIVOT) << "\n";

		file.close();
	}
}

//Print SCC output
// operation=0		No printing
// operation=1		Print to file at 'path' with suffix 'suffix'
void printFile(enhancedgraph *enhgraph, TStr path, TStr suffix, bool operation) 
{
	if (!operation) 
	{
		return;
	}

	cout << "\nWriting SCC output\n";

	//Create output file
	TStr fileName = getFileName(path);
	fileName += suffix;
	fileName += ".scc";

	ofstream file;
	file.open(fileName.GetCStr());

	ColorMap *colors = enhgraph->colors;

	//Print list of nodes and their color
	for (int i = enhgraph->colors->BegI(); i < enhgraph->colors->EndI(); i++)
    {
		file << i << " " << colors->GetDat(i) << "\n";
	}

	file.close();

	cout << "Done\n";
}

//Print debug information about program execution
// operation=0		No printing
// operation=1		Print to stdout
// operation=2		Print to file at 'path' with suffix 'suffix'
// operation=3		Print compact version to file
void printInfo(enhancedgraph *enhgraph, TStr path, TStr suffix, int operation) 
{
	if (operation == 0) 
	{
		return;
	}

	//Get basic information
	int fwbwCalls = enhgraph->getCallsFWBW();
	int fwbwDepth = enhgraph->getDepth();
	int sccs = 0;
	int gsize = enhgraph->graph->GetNodes();

	std::list<int> *lColor = enhgraph->getReports(eDebug::dCOLOR);
	std::list<int> *lNode = enhgraph->getReports(eDebug::dNODE);
	std::list<int> *lTrim1 = enhgraph->getReports(eDebug::dTRIM1);
	std::list<int> *lTrim2 = enhgraph->getReports(eDebug::dTRIM2);
	std::list<int> *lTrim3 = enhgraph->getReports(eDebug::dTRIM3);
	std::list<int> *lBfs = enhgraph->getReports(eDebug::dBFS);
	std::list<int> *lDepth = enhgraph->getReports(eDebug::dDEPTH);
	std::list<int> *lFw = enhgraph->getReports(eDebug::dFW);
	std::list<int> *lBw = enhgraph->getReports(eDebug::dBW);

	//Count number of SCC's
	for (auto it = lTrim1->cbegin(); it != lTrim1->cend(); it++) 
	{
		if (*it > -1) 
		{
			sccs += *it;
		}
	}

	for (auto it = lTrim2->cbegin(); it != lTrim2->cend(); it++) 
	{
		if (*it > -1) 
		{
			sccs += *it;
		}
	}

	for (auto it = lTrim3->cbegin(); it != lTrim3->cend(); it++) 
	{
		if (*it > -1) 
		{
			sccs += *it;
		}
	}

	for (auto it = lBfs->cbegin(); it != lBfs->cend(); it++) 
	{
		if (*it > 0) 
		{
			sccs++;
		}
	}


	if (operation == 1) 
	{
		//Write basic info to stdout
		cout << "\nDebug information: \n";
		cout << "  " << "#SCCs: " << sccs << "\n";
		cout << "  " << "Graph size: " << gsize << "\n";
		cout << "  " << "#FWBW calls: " << fwbwCalls << "\n";
		cout << "  " << "#FWBW depth: " << fwbwDepth << "\n";
		
		cout << "  " << "BFS calls: " << lBfs->size() << "\n";
		cout << "  " << "trim calls: " << enhgraph->getCallsTrim() << "\n";
		cout << "  " << "pivots calls: " << enhgraph->getCallsPivot() << "\n";
		cout << "  " << "successful pivot calls: " << lNode->size() << "\n\n";

		//Write FWBW recurion tree
		cout << "FWBW: " << "\n";
		while (!lColor->empty()) 
		{
			int node = lNode->front();
			lNode->pop_front();
			int color = lColor->front();
			lColor->pop_front();
			int trim1 = lTrim1->front();
			lTrim1->pop_front();
			int trim2 = lTrim2->front();
			lTrim2->pop_front();
			int trim3 = lTrim3->front();
			lTrim3->pop_front();
			int bfs = lBfs->front();
			lBfs->pop_front();
			int depth = lDepth->front();
			lDepth->pop_front();
			int fw = lFw->front();
			lFw->pop_front();
			int bw = lBw->front();
			lBw->pop_front();

			cout << "  " << "FWBW at depth " << depth << " for color " << color << " with pivot " << node << "\n";
			cout << "    " << "BFS found " << bfs << "\n";
			cout << "    " << "Trim found " << trim1 << ", " << trim2 << ", " << trim3 << "\n";
			cout << "    " << "Children " << color << ", " << fw << ", " << bw << "\n";
		}

	} 
	else if (operation == 2) 
	{
		cout << "\nWriting debug output\n";

		//Create output file
		TStr fileName = getFileName(path);
		fileName += suffix;
		fileName += ".debug";

		ofstream file;
		file.open(fileName.GetCStr());

		//Write basic info to file
		file << "\nDebug information: \n";
		file << "  " << "#SCCs: " << sccs << "\n";
		file << "  " << "Graph size: " << gsize << "\n";
		file << "  " << "#FWBW calls: " << fwbwCalls << "\n";
		file << "  " << "#FWBW depth: " << fwbwDepth << "\n";
		
		file << "  " << "BFS calls: " << lBfs->size() << "\n";
		file << "  " << "trim calls: " << enhgraph->getCallsTrim() << "\n";
		file << "  " << "pivots calls: " << enhgraph->getCallsPivot() << "\n";
		file << "  " << "successful pivot calls: " << lNode->size() << "\n\n";

		//Write FWBW recursion tree
		file << "FWBW: " << "\n";
		while (!lColor->empty()) 
		{
			int node = lNode->front();
			lNode->pop_front();
			int color = lColor->front();
			lColor->pop_front();
			int trim1 = lTrim1->front();
			lTrim1->pop_front();
			int trim2 = lTrim2->front();
			lTrim2->pop_front();
			int trim3 = lTrim3->front();
			lTrim3->pop_front();
			int bfs = lBfs->front();
			lBfs->pop_front();
			int depth = lDepth->front();
			lDepth->pop_front();
			int fw = lFw->front();
			lFw->pop_front();
			int bw = lBw->front();
			lBw->pop_front();

			file << "  " << "FWBW at depth " << depth << " for color " << color << " with pivot " << node << "\n";
			file << "    " << "BFS found " << bfs << "\n";
			file << "    " << "Trim found " << trim1 << ", " << trim2 << ", " << trim3 << "\n";
			file << "    " << "Children " << color << ", " << fw << ", " << bw << "\n";
		}

		file.close();

		cout << "Done\n";
	} 
	else if (operation == 3) 
	{
		cout << "\nWriting debug output\n";

		//Create output file
		TStr fileName = getFileName(path);
		fileName += suffix;
		fileName += ".debug";

		ofstream file;
		file.open(fileName.GetCStr());

		//Write compact info to file
		file << sccs << "\t";
		file << gsize << "\t";
		file << fwbwCalls << "\t";
		file << fwbwDepth << "\t";
		file << lBfs->size() << "\t";
		file << enhgraph->getCallsTrim() << "\t";
		file << enhgraph->getCallsPivot() << "\t";
		file << lNode->size() << "\n\n";

		//Write recursion tree
		while (!lColor->empty()) 
		{
			int node = lNode->front();
			lNode->pop_front();
			int color = lColor->front();
			lColor->pop_front();
			int trim1 = lTrim1->front();
			lTrim1->pop_front();
			int trim2 = lTrim2->front();
			lTrim2->pop_front();
			int trim3 = lTrim3->front();
			lTrim3->pop_front();
			int bfs = lBfs->front();
			lBfs->pop_front();
			int depth = lDepth->front();
			lDepth->pop_front();
			int fw = lFw->front();
			lFw->pop_front();
			int bw = lBw->front();
			lBw->pop_front();

			file << depth << "\t";
			file << color << "\t";
			file << node << "\t";
			file << bfs << "\t";
			file << trim1 << "\t";
			file << trim2 << "\t";
			file << trim3 << "\t";
			file << fw << "\t";
			file << bw << "\n";
		}

		file.close();

		cout << "Done\n";
	}
}

void printHelp()
{
	cout << "\n" << "parameters:" << "\n";
	cout << "  " << "Trim levels:" << "\n";
	cout << "    " << "0 = no trim" << "\n";
	cout << "    " << "1 = trim-1" << "\n";
	cout << "    " << "2 = trim-1,2" << "\n";
	cout << "    " << "3 = trim-1,2,3" << "\n";
	
	cout << "  " << "Pivot methods:" << "\n";
	cout << "    " << "0 = first occurrence" << "\n";
	cout << "    " << "1 = max degree product" << "\n";
	cout << "    " << "2 = max degree product with colors" << "\n";
	cout << "    " << "3 = randomwalk" << "\n";
	cout << "    " << "3 = max degree with precomputed product" << "\n";

	cout << "  " << "FWBW method:" << "\n";
	cout << "    " << "0 = basic FWBW" << "\n";
	cout << "    " << "1 = FWBW with parallel bfs" << "\n";
	cout << "    " << "2 = Recursive FWBW" << "\n";


	cout << "\n" << "Print:" << "\n";
	cout << "  " << "Timers:" << "\n";
	cout << "    " << "0 = no printing" << "\n";
	cout << "    " << "1 = print to stdout" << "\n";
	cout << "    " << "2 = print to file" << "\n";
	cout << "    " << "3 = print compact version to file" << "\n";

	cout << "  " << "Debug:" << "\n";
	cout << "    " << "0 = no printing" << "\n";
	cout << "    " << "1 = print to stdout" << "\n";
	cout << "    " << "2 = print to file" << "\n";
	cout << "    " << "3 = print compact version to file" << "\n";
}

int main(int argc, char **argv)
{
    // Setup input arguments handler
    Env = TEnv(argc, argv, TNotify::StdNotify);
    Env.PrepArgs(TStr::Fmt("SCC. build: %s, %s. Time: %s",
                           __TIME__, __DATE__, TExeTm::GetCurTm()));
    const TStr InEdges =
        Env.GetIfArgPrefixStr("-g=", "fb1.edges", "Input graph (directed)\t\t");
    const int Trimlevels =
        Env.GetIfArgPrefixInt("-t=", 0, "Input number of trim levels\t");
    const int PivotMethod =
        Env.GetIfArgPrefixInt("-p=", 0, "Pivot selection\t\t\t");
    const int FwBwMethod =
        Env.GetIfArgPrefixInt("-m=", 0, "Specify FW-BW variant\t\t");
	const int MaxThreads =
        Env.GetIfArgPrefixInt("-w=", omp_get_max_threads(), "Specify maximum workers\t\t");
	const int RandIterations =
        Env.GetIfArgPrefixInt("-rand=", 10, "Specify randwalk iterations\t");
	const int TrimCutoff =
        Env.GetIfArgPrefixInt("-cutoff=", 0, "Specify trim cutoff point\t");
	const int TrimSteps =
        Env.GetIfArgPrefixInt("-steps=", 1, "Specify how often to trim\t");
	const bool Help =
        Env.GetIfArgPrefixBool("-h=", false, "Print help section\t\t");
	const bool Output =
        Env.GetIfArgPrefixBool("-out=", false, "Print SCC output to file\t");
	const TStr Suffix =
        Env.GetIfArgPrefixStr("-suffix=", "", "Name to append output file\t");
	const int Timer =
        Env.GetIfArgPrefixInt("-time=", 0, "Print timers\t\t\t");
	const int Analyse =
        Env.GetIfArgPrefixInt("-a=", 0, "Print debug information\t\t");

	//Print help section if help argument was included
	if (Help)
	{
		printHelp();
		return 0;
	}
	
	cout << "\n";

	//Set max number of OpenMP workers
	omp_set_num_threads(MaxThreads);

    // Load the graph and setup enhanched graph object
	cout << "Loading graph\n";
	TimePoint start;
	if (Timer) 
	{
		start = Time::now();
	}
	PNGraph Graph = TSnap::LoadEdgeList<PNGraph>(InEdges);

	enhancedgraph *enhgraph;
    enhgraph = new enhancedgraph(&(*Graph), Timer, Analyse, RandIterations, TrimCutoff, TrimSteps);
	
	enhgraph->endTimer(start, eTimer::SETUP);
	cout << "Graph loaded\n";

	start = enhgraph->startTimer();
	if (PivotMethod == 4) 
	{
		enhgraph->calculateDegree();
	}
	enhgraph->endTimer(start, eTimer::PREP);

	
	//Perform SCC decomposition of the graph
	start = enhgraph->startTimer();
	scc::FindSCCs(enhgraph, Trimlevels, PivotMethod, FwBwMethod);
	enhgraph->endTimer(start, eTimer::MAIN);

	cout << "Graph " << Graph->GetNodes() << ", " << Graph->GetEdges() << "\n";
	
	
	//Output data, timers and debug information if specified in the arguments
	printTime(enhgraph, InEdges, Suffix, Timer);
	printFile(enhgraph, InEdges, Suffix, Output);
	printInfo(enhgraph, InEdges, Suffix, Analyse);

    return 0;
}