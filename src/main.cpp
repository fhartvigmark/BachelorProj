#include "main.h"

TStr getFileName(TStr path) {
	return path.GetFMid();
}

void printTime(enhancedgraph *enhgraph, TStr path, int operation) {
	if (operation == 1) {
		cout << "\nTime used: \n";
		cout << "  " << "Setup\t\t" << enhgraph->getTime(eTimer::SETUP) << "ms\n";
		cout << "  " << "SCC\t\t" << enhgraph->getTime(eTimer::MAIN) << "ms\n";
		cout << "  " << "First FWBW\t" << enhgraph->getTime(eTimer::FirstFWBW) << "ms\n";
		cout << "  " << "FWBW\t\t" << enhgraph->getTime(eTimer::FWBWs) << "ms\n";
		cout << "  " << "Trim\t\t" << enhgraph->getTime(eTimer::TRIM) << "ms\n";
		cout << "  " << "First Trim\t" << enhgraph->getTime(eTimer::TRIM) << "ms\n";
		cout << "  " << "Pivot\t\t" << enhgraph->getTime(eTimer::PIVOT) << "ms\n";
	} else if (operation == 2) {
		cout << "\nWriting timer output\n";

		TStr fileName = getFileName(path);
		fileName += ".time";

		ofstream file;
		file.open(fileName.GetCStr());

		file << "\nTime used: \n";
		file << "  " << "Setup\t\t" << enhgraph->getTime(eTimer::SETUP) << "ms\n";
		file << "  " << "SCC\t\t" << enhgraph->getTime(eTimer::MAIN) << "ms\n";
		file << "  " << "First FWBW\t" << enhgraph->getTime(eTimer::FirstFWBW) << "ms\n";
		file << "  " << "FWBW\t\t" << enhgraph->getTime(eTimer::FWBWs) << "ms\n";
		file << "  " << "Trim\t\t" << enhgraph->getTime(eTimer::TRIM) << "ms\n";
		file << "  " << "First Trim\t" << enhgraph->getTime(eTimer::TRIM) << "ms\n";
		file << "  " << "Pivot\t\t" << enhgraph->getTime(eTimer::PIVOT) << "ms\n";

		file.close();

		cout << "Done\n";
	}
}

void printFile(enhancedgraph *enhgraph, TStr path, bool operation) {
	if (!operation) {
		return;
	}

	cout << "\nWriting SCC output\n";

	TStr fileName = getFileName(path);
	fileName += ".scc";

	ofstream file;
	file.open(fileName.GetCStr());

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

void printInfo(enhancedgraph *enhgraph, TStr path, int operation) {
	if (operation == 0) {
		return;
	}
	int fwbwCalls = enhgraph->getCalls();
	int fwbwDepth = enhgraph->getDepth();
	int sccs = fwbwCalls;
	int gsize = enhgraph->graph->GetNodes();

	std::list<int> *trimAmount = enhgraph->getReports(eDebug::tAmount);
	std::list<int> *trimColor = enhgraph->getReports(eDebug::tColor);
	std::list<int> *trimType = enhgraph->getReports(eDebug::tType);
	std::list<int> *pivotNode = enhgraph->getReports(eDebug::pNode);
	std::list<int> *pivotColor = enhgraph->getReports(eDebug::pColor);

	for (auto it = trimAmount->cbegin(); it != trimAmount->cend(); it++) {
		sccs += *it;
	}

	if (operation == 1) {
		cout << "\nDebug information: \n";
		cout << "  " << "#SCCs: " << sccs << "\n";
		cout << "  " << "Graph size: " << gsize << "\n";
		cout << "  " << "#FWBW calls: " << fwbwCalls << "\n";
		cout << "  " << "#FWBW depth: " << fwbwDepth << "\n";
		
		cout << "  " << "pivots: " << "\n";
		while (!pivotNode->empty()) {
			int node = pivotNode->front();
			pivotNode->pop_front();
			int color = pivotColor->front();
			pivotColor->pop_front();

			cout << "    " << "Chosen node " << node << " for color " << color << "\n";
		}

		cout << "  " << "trims: " << "\n";
		while (!trimAmount->empty()) {
			int amount = trimAmount->front();
			trimAmount->pop_front();
			int color = trimColor->front();
			trimColor->pop_front();
			int type = trimType->front();
			trimType->pop_front();

			cout << "    " << "Trim " << type << " trimmed " << amount << " for color " << color << "\n";
		}

	} else if (operation == 2) {
		cout << "\nWriting debug output\n";

		TStr fileName = getFileName(path);
		fileName += ".debug";

		ofstream file;
		file.open(fileName.GetCStr());

		file << "\nDebug information: \n";
		file << "  " << "#SCCs: " << sccs << "\n";
		file << "  " << "#FWBW calls: " << fwbwCalls << "\n";
		file << "  " << "FWBW depth: " << fwbwDepth << "\n";
		
		file << "  " << "pivots: " << "\n";
		while (!pivotNode->empty()) {
			int node = pivotNode->front();
			pivotNode->pop_front();
			int color = pivotColor->front();
			pivotColor->pop_front();

			file << "    " << "Chosen node " << node << " for color " << color << "\n";
		}

		file << "  " << "trims: " << "\n";
		while (!trimAmount->empty()) {
			int amount = trimAmount->front();
			trimAmount->pop_front();
			int color = trimColor->front();
			trimColor->pop_front();
			int type = trimType->front();
			trimType->pop_front();

			file << "    " << "Trim " << type << " trimmed " << amount << " for color " << color << "\n";
		}

		file.close();

		cout << "Done\n";
	}
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
	const bool Help =
        Env.GetIfArgPrefixBool("-h=", false, "Print help section\t\t");
	const bool Output =
        Env.GetIfArgPrefixBool("-out=", false, "Print SCC output to file\t");
	const int Timer =
        Env.GetIfArgPrefixInt("-time=", 0, "Print timers\t\t\t");
	const int Analyse =
        Env.GetIfArgPrefixInt("-a=", 0, "Print debug information\t\t");

	if (Help)
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
		cout << "    " << "3 = parallel version of 0" << "\n";
		cout << "    " << "4 = parallel version of 1" << "\n";
		cout << "    " << "5 = parallel version of 2" << "\n";
		cout << "    " << "6 = randomwalk" << "\n";
		cout << "    " << "7 = parallel randomwalk" << "\n";

		cout << "  " << "FWBW method:" << "\n";
		cout << "    " << "0 = basic FWBW" << "\n";
		cout << "    " << "1 = FWBW with parallel bfs" << "\n";
		cout << "    " << "2 = Recursive FWBW" << "\n";


		cout << "\n" << "Print:" << "\n";
		cout << "  " << "Timers:" << "\n";
		cout << "    " << "0 = no printing" << "\n";
		cout << "    " << "1 = print to stdout" << "\n";
		cout << "    " << "2 = print to file" << "\n";

		cout << "  " << "Debug:" << "\n";
		cout << "    " << "0 = no printing" << "\n";
		cout << "    " << "1 = print to stdout" << "\n";
		cout << "    " << "2 = print to file" << "\n";


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
    enhgraph = new enhancedgraph(Graph, Timer, Analyse, 10);
	
	enhgraph->endTimer(start, eTimer::SETUP);
	cout << "Graph loaded\n";

	
	start = enhgraph->startTimer();
	scc::FindSCCs(enhgraph, Trimlevels, PivotMethod, FwBwMethod);
	enhgraph->endTimer(start, eTimer::MAIN);


	//int color = 100;
	//TIntH *colorMap = enhgraph->colors;
	//PNGraph graph = enhgraph->graph;
	//TIntV *Ids = enhgraph->NIds;
	//int retVal = -1;

/*
	start = enhgraph->startTimer();
	for (int j = 0; j < 100; j++) {
		for (THashKeyDatI<TInt, TInt> i = colorMap->BegI(); i < colorMap->EndI(); i++)
		{
			if (i.GetDat()==color)
			{
				retVal = i.GetKey();
			}
			
		}
	}
	enhgraph->endTimer(start, eTimer::FirstFWBW);


	start = enhgraph->startTimer();
	for (int j = 0; j < 100; j++) {
		for (int i = 0; i < Ids->Len(); i++) {
			if (colorMap->GetDat(Ids->GetVal(i)) == color) {
				retVal = Ids->GetVal(i);
			}
		}
	}
	enhgraph->endTimer(start, eTimer::FWBWs);


	start = enhgraph->startTimer();
	for (int j = 0; j < 100; j++) {
		#pragma omp parallel for schedule(dynamic)
		for (int i = 0; i < Ids->Len(); i++) {
			if (colorMap->GetDat(Ids->GetVal(i)) == color) {

				#pragma omp cancellation point for	
				#pragma omp critical
				{
					retVal = Ids->GetVal(i);
				}
				#pragma omp cancel for
			}
		}
	}
	enhgraph->endTimer(start, eTimer::TRIM);


	start = enhgraph->startTimer();
	for (int j = 0; j < 100; j++) {
		#pragma omp parallel for schedule(static)
		for (int i = 0; i < Ids->Len(); i++) {
			if (colorMap->GetDat(Ids->GetVal(i)) == color) {

				#pragma omp cancellation point for	
				#pragma omp critical
				{
					retVal = Ids->GetVal(i);
				}
				#pragma omp cancel for
			}
		}
	}
	enhgraph->endTimer(start, eTimer::PIVOT);
*/

/*
	start = enhgraph->startTimer();
	for (int j = 0; j < 100; j++)
	{
		TSnapQueue<int> Queue;
		int color=0;
		TIntH *colors = enhgraph->colors;
		PNGraph graph = enhgraph->graph;
		for (TNGraph::TNodeI NI = graph->BegNI(); NI < graph->EndNI(); NI++)
		{
			int node = NI.GetId();
			if (colors->GetDat(node) == color) {
				int inDegree = 0;
				TNGraph::TNodeI NodeI = graph->GetNI(node);

				int v = 0;
				for (v = 0; v < NodeI.GetInDeg(); v++) 
				{
					const int outNode = NodeI.GetInNId(v);

					if (colors->GetDat(outNode) == color && outNode != node) {
						inDegree = 1;
						break;
					}
				}

				if (inDegree == 0) {
					//colors->AddDat(node, g->colorGen->getNext());
					Queue.Push(node);
					continue;
				}

				int outDegree = 0;
				NodeI = graph->GetNI(node);

				for (v = 0; v < NodeI.GetOutDeg(); v++) 
				{
					const int outNode = NodeI.GetOutNId(v);

					if (colors->GetDat(outNode) == color && outNode != node) {
						outDegree = 1;
						break;
					}
				}

				if (outDegree == 0) {
					//colors->AddDat(node, g->colorGen->getNext());
					Queue.Push(node);
					continue;
				}
			}
		}
	}
	enhgraph->endTimer(start, eTimer::FirstFWBW);


	start = enhgraph->startTimer();
	for (int j = 0; j < 100; j++)
	{
		int color=0;
		TSnapQueue<int> Queue;
		TIntH *colors = enhgraph->colors;
		PNGraph graph = enhgraph->graph;
		TIntV *Ids = enhgraph->NIds;

		#pragma omp parallel for schedule(dynamic)
		for (int i = 0; i < Ids->Len(); i++)
		{
			int node = Ids->GetVal(i);
			if (colors->GetDat(node) == color)
			{
				int inDegree = 0;
				TNGraph::TNodeI NodeI = graph->GetNI(node);

				for (int v = 0; v < NodeI.GetInDeg(); v++)
				{
					const int outNode = NodeI.GetInNId(v);

					if (colors->GetDat(outNode) == color && outNode != node)
						{
						inDegree = 1;
						break;
					}
				}

				if (inDegree == 0)
				{
					//colors->AddDat(node, g->colorGen->getNext());
					#pragma omp critical
					{
						Queue.Push(node);
					}
					continue;
				}

				int outDegree = 0;
				NodeI = graph->GetNI(node);

				for (int v = 0; v < NodeI.GetOutDeg(); v++)
				{
					const int outNode = NodeI.GetOutNId(v);

					if (colors->GetDat(outNode) == color && outNode != node)
					{
						outDegree = 1;
						break;
					}
				}

				if (outDegree == 0)
				{
					//colors->AddDat(node, g->colorGen->getNext());
					#pragma omp critical
					{
						Queue.Push(node);
					}
					continue;
				}
			}
		}
	}
	enhgraph->endTimer(start, eTimer::FWBWs);


	start = enhgraph->startTimer();
	for (int j = 0; j < 100; j++)
	{
		int color=0;
		TSnapQueue<int> Queue;
		TIntH *colors = enhgraph->colors;
		PNGraph graph = enhgraph->graph;
		TIntV *Ids = enhgraph->NIds;

		#pragma omp parallel for schedule(static)
		for (int i = 0; i < Ids->Len(); i++)
		{
			int node = Ids->GetVal(i);
			if (colors->GetDat(node) == color)
			{
				int inDegree = 0;
				TNGraph::TNodeI NodeI = graph->GetNI(node);

				for (int v = 0; v < NodeI.GetInDeg(); v++)
				{
					const int outNode = NodeI.GetInNId(v);

					if (colors->GetDat(outNode) == color && outNode != node)
						{
						inDegree = 1;
						break;
					}
				}

				if (inDegree == 0)
				{
					//colors->AddDat(node, g->colorGen->getNext());
					#pragma omp critical
					{
						Queue.Push(node);
					}
					continue;
				}

				int outDegree = 0;
				NodeI = graph->GetNI(node);

				for (int v = 0; v < NodeI.GetOutDeg(); v++)
				{
					const int outNode = NodeI.GetOutNId(v);

					if (colors->GetDat(outNode) == color && outNode != node)
					{
						outDegree = 1;
						break;
					}
				}

				if (outDegree == 0)
				{
					//colors->AddDat(node, g->colorGen->getNext());
					#pragma omp critical
					{
						Queue.Push(node);
					}
					continue;
				}
			}
		}
	}
	enhgraph->endTimer(start, eTimer::TRIM);
*/


	printTime(enhgraph, InEdges, Timer);
	printFile(enhgraph, InEdges, Output);
	printInfo(enhgraph, InEdges, Analyse);

    return 0;
}