#include "core.h"
#include "iostream"

void ColorMap::AddDat(int i, int data) {
	array[i] = data;
}
int ColorMap::GetDat(int i) {
	return array[i];
}

int ColorMap::Len() {
	return length;
}

int ColorMap::BegI() {
	return beg;
}

int ColorMap::EndI() {
	return end;
}

ColorMap::ColorMap(int size, bool hasZero) {
	length = size;
	array = new int[size+1];

	//std::cout << "Start is " << hasZero << "\n";

	if (hasZero) {
		beg = 0;
		end = length;
	} else {
		beg = 1;
		end = length + 1;
	}
}

ColorMap::~ColorMap() {
	delete[] array;
}


ColorGenerator::ColorGenerator() {
	lastColor = 0;
}


void enhancedgraph::calculateDegree() {
	bool zero = graph->IsNode(0);

	degree = new ColorMap(graph->GetNodes(), zero);

	for (PNGraph::TObj::TNodeI NI = graph->BegNI(); NI < graph->EndNI(); NI++)
	{
		degree->AddDat(NI.GetId(), NI.GetInDeg() * NI.GetOutDeg());
	}
}


//Get current time
TimePoint enhancedgraph::startTimer() {
	if (TIMER_ENABLED) {
		return Time::now();
	}
}

//Measure end time and add that time to the corresponding duration based on the timer argument
void enhancedgraph::endTimer(TimePoint start, eTimer timer) {
	if (TIMER_ENABLED)
	{
		auto end = Time::now();
		auto dur = end - start;

		switch (timer)
		{
			case eTimer::MAIN:
				omp_set_lock(&lMain);
				tMain += dur;
				omp_unset_lock(&lMain);
				break;
			case eTimer::FirstFWBW:
				omp_set_lock(&lFirstFWBW);
				tFirstFWBW += dur;
				omp_unset_lock(&lFirstFWBW);				
				break;
			case eTimer::FWBWs:
				omp_set_lock(&lFWBW);
				tFWBW += dur;
				omp_unset_lock(&lFWBW);
				break;
			case eTimer::TRIM:
				omp_set_lock(&lTrim);
				tTrim += dur;
				omp_unset_lock(&lTrim);
				break;
			case eTimer::TRIM1:
				omp_set_lock(&lTrim);
				tTrim1 += dur;
				omp_unset_lock(&lTrim);
				break;
			case eTimer::TRIM2:
				omp_set_lock(&lTrim);
				tTrim2 += dur;
				omp_unset_lock(&lTrim);
				break;
			case eTimer::TRIM3:
				omp_set_lock(&lTrim);
				tTrim3 += dur;
				omp_unset_lock(&lTrim);
				break;
			case eTimer::FirstTRIM:
				omp_set_lock(&lFirstTrim);
				tFirstTrim += dur;
				omp_unset_lock(&lFirstTrim);
				break;
			case eTimer::PIVOT:
				omp_set_lock(&lPivot);
				tPivot += dur;
				omp_unset_lock(&lPivot);
				break;
			case eTimer::SETUP:
				omp_set_lock(&lSetup);
				tSetup += dur;
				omp_unset_lock(&lSetup);
				break;
			case eTimer::PREP:
				omp_set_lock(&lSetup);
				tPrep += dur;
				omp_unset_lock(&lSetup);
				break;
			default:
				break;
		}
	}
}

//Get total runtime of a component
int64_t enhancedgraph::getTime(eTimer timer) {
	if (!TIMER_ENABLED) {
		return -1;
	}

	switch (timer)
	{
		case eTimer::MAIN:
			return std::chrono::duration_cast<Ms>(tMain).count();
		case eTimer::FirstFWBW:
			return std::chrono::duration_cast<Ms>(tFirstFWBW).count();
		case eTimer::FWBWs:
			return std::chrono::duration_cast<Ms>(tFWBW).count();
		case eTimer::TRIM:
			return std::chrono::duration_cast<Ms>(tTrim).count();
		case eTimer::TRIM1:
			return std::chrono::duration_cast<Ms>(tTrim1).count();
		case eTimer::TRIM2:
			return std::chrono::duration_cast<Ms>(tTrim2).count();
		case eTimer::TRIM3:
			return std::chrono::duration_cast<Ms>(tTrim3).count();
		case eTimer::FirstTRIM:
			return std::chrono::duration_cast<Ms>(tFirstTrim).count();
		case eTimer::PIVOT:
			return std::chrono::duration_cast<Ms>(tPivot).count();
		case eTimer::SETUP:
			return std::chrono::duration_cast<Ms>(tSetup).count();
		case eTimer::PREP:
			return std::chrono::duration_cast<Us>(tPrep).count();
		default:
			return -1;
	}
}

void enhancedgraph::reportFWBW(int color, int node, int trim1, int trim2, int trim3, int bfs, int depth, int fw, int bw) {
	if (ANALYSE_ENABLED) {
		omp_set_lock(&lDebugFWBW);
		if (node == -1) {
			callsPivot++;
			omp_unset_lock(&lDebugFWBW);
			return;
		}

		callsFWBW++;
		callsTrim++;
		callsPivot++;

		if (depth > depthFWBW) {
			depthFWBW = depth;
		}

		dColor->push_back(color);
		dNode->push_back(node);
		dTrim1->push_back(trim1);
		dTrim2->push_back(trim2);
		dTrim3->push_back(trim3);
		dBFS->push_back(bfs);
		dDepth->push_back(depth);
		dFW->push_back(fw);
		dBW->push_back(bw);

		omp_unset_lock(&lDebugFWBW);
	}
}

int64_t enhancedgraph::getCallsFWBW() {
	return callsFWBW;
}

int64_t enhancedgraph::getCallsTrim() {
	return callsTrim;
}

int64_t enhancedgraph::getCallsPivot() {
	return callsPivot;
}

int64_t enhancedgraph::getDepth() {
	return depthFWBW;
}

std::list<int>* enhancedgraph::getReports(eDebug data) {
	switch (data)
	{
		case eDebug::dCOLOR:
			return dColor;
		case eDebug::dNODE:
			return dNode;
		case eDebug::dTRIM1:
			return dTrim1;
		case eDebug::dTRIM2:
			return dTrim2;
		case eDebug::dTRIM3:
			return dTrim3;
		case eDebug::dBFS:
			return dBFS;
		case eDebug::dDEPTH:
			return dDepth;
		case eDebug::dFW:
			return dFW;
		case eDebug::dBW:
			return dBW;
		default:
			return {};
	}
}


enhancedgraph::enhancedgraph(const PNGraph& g, bool timer, bool analyse, int randwalk_iterations, int cutoff, int steps) : TIMER_ENABLED(timer), ANALYSE_ENABLED(analyse), RAND_WALK_ITERATIONS(randwalk_iterations), TRIM_CUTOFF(cutoff), TRIM_STEPS(steps) {
	graph = g;
	colorGen = new ColorGenerator();
	//colors = new TIntH();
	//colors->Gen(g->GetNodes());
	bool zero = g->IsNode(0);
	//std::cout << "Is a node: " << zero << "\n";
	//const TNGraph::TNodeI NodeI = g->GetNI(12);
	//std::cout << "In degree of node 12 is: " << NodeI.GetInDeg() << "\n";

	colors = new ColorMap(g->GetNodes(), zero);


	//Add all colors and node ids to colormap and node vector
	for (PNGraph::TObj::TNodeI NI = graph->BegNI(); NI < graph->EndNI(); NI++)
	{
		colors->AddDat(NI.GetId(), 0);
	}

	//Initialize durations and duration locks
	if (true) {
		tMain = Duration::zero();
		tFirstFWBW = Duration::zero();
		tFWBW = Duration::zero();
		tTrim = Duration::zero();
		tTrim1 = Duration::zero();
		tTrim2 = Duration::zero();
		tTrim3 = Duration::zero();
		tFirstTrim = Duration::zero();
		tPivot = Duration::zero();
		tSetup = Duration::zero();
		tPrep = Duration::zero();

		omp_init_lock(&lMain);
		omp_init_lock(&lFirstFWBW);
		omp_init_lock(&lFWBW);
		omp_init_lock(&lTrim);
		omp_init_lock(&lFirstTrim);
		omp_init_lock(&lPivot);
		omp_init_lock(&lSetup);
	}

	//Initialize debug information variables and locks
	if (true) {
		omp_init_lock(&lDebugFWBW);

		callsFWBW = 0;
		callsTrim = 0;
		callsPivot = 0;
		depthFWBW = 0;

		dColor = new std::list<int>;
		dNode = new std::list<int>;
		dTrim1 = new std::list<int>;
		dTrim2 = new std::list<int>;
		dTrim3 = new std::list<int>;
		dBFS = new std::list<int>;
		dDepth = new std::list<int>;
		dFW = new std::list<int>;
		dBW = new std::list<int>;
	}
}

//Basic constructor, only sets constants and initilize timers
enhancedgraph::enhancedgraph() : TIMER_ENABLED(false), ANALYSE_ENABLED(false), RAND_WALK_ITERATIONS(10), TRIM_CUTOFF(0), TRIM_STEPS(1) {
	//Initialize durations and duration locks
	if (true) {
		tMain = Duration::zero();
		tFirstFWBW = Duration::zero();
		tFWBW = Duration::zero();
		tTrim = Duration::zero();
		tTrim1 = Duration::zero();
		tTrim2 = Duration::zero();
		tTrim3 = Duration::zero();
		tFirstTrim = Duration::zero();
		tPivot = Duration::zero();
		tSetup = Duration::zero();
		tPrep = Duration::zero();

		omp_init_lock(&lMain);
		omp_init_lock(&lFirstFWBW);
		omp_init_lock(&lFWBW);
		omp_init_lock(&lTrim);
		omp_init_lock(&lFirstTrim);
		omp_init_lock(&lPivot);
		omp_init_lock(&lSetup);
	}

	//Initialize debug information variables and locks
	if (true) {
		omp_init_lock(&lDebugFWBW);

		callsFWBW = 0;
		callsTrim = 0;
		callsPivot = 0;
		depthFWBW = 0;

		dColor = {};
		dNode = {};
		dTrim1 = {};
		dTrim2 = {};
		dTrim3 = {};
		dBFS = {};
		dDepth = {};
		dFW = {};
		dBW = {};
	}
}

enhancedgraph::~enhancedgraph() {
	//Delete graph elements
	//delete *graph; TODO: clean up graph?
	//TODO: clean up degree array
	delete colors;
    delete colorGen;
	//graph.~TPt();

	//Delete analysis elements
	if (true) {
		omp_destroy_lock(&lDebugFWBW);
		
		delete dColor;
		delete dNode;
		delete dTrim1;
		delete dTrim2;
		delete dTrim3;
		delete dBFS;
		delete dDepth;
		delete dFW;
		delete dBW;
	}

	//Delete timing elements
	if (true) {
		omp_destroy_lock(&lMain);
		omp_destroy_lock(&lFirstFWBW);
		omp_destroy_lock(&lFWBW);
		omp_destroy_lock(&lTrim);
		omp_destroy_lock(&lFirstTrim);
		omp_destroy_lock(&lPivot);
		omp_destroy_lock(&lSetup);
	}
}

int Random::myRand(const int low, const int high) {
    static thread_local std::mt19937 randGenerator;

    std::uniform_int_distribution<int> distribution(low,high);
    return distribution(randGenerator);
}

int Random::randstep(enhancedgraph *g, PNGraph graph, int color, int node) {
	ColorMap *colors = g->colors;

	//Find number of edges of same color
	int edges = 0;

	TNGraph::TNodeI NodeI = graph->GetNI(node);
	int v;
	for (v = 0; v < NodeI.GetOutDeg(); v++)
	{
		const int outNode = NodeI.GetOutNId(v);

		if (colors->GetDat(outNode) == color)
		{
			edges += 1;
		}
	}
	
	NodeI = graph->GetNI(node);
	for (v = 0; v < NodeI.GetInDeg(); v++)
	{
		const int outNode = NodeI.GetInNId(v);

		if (colors->GetDat(outNode) == color)
		{
			edges += 1;
		}
	}

	if (edges <= 0) {
		return node;
	}
	int index = myRand(0, edges-1);
	//std::cout << ", " << index << "/" << edges;
	edges = 0;

	//Find the edges with chosen index
	NodeI = graph->GetNI(node);
	for (v = 0; v < NodeI.GetOutDeg(); v++)
	{
		const int outNode = NodeI.GetOutNId(v);

		if (colors->GetDat(outNode) == color)
		{
			if (edges == index) {
				//std::cout << "-" << outNode;
				return outNode;
			}
			edges += 1;
		}
	}
	
	NodeI = graph->GetNI(node);
	for (v = 0; v < NodeI.GetInDeg(); v++)
	{
		const int outNode = NodeI.GetInNId(v);

		if (colors->GetDat(outNode) == color)
		{
			if (edges == index) {
				//std::cout << "-" << outNode;
				return outNode;
			}
			edges += 1;
		}
	}

	return node;
}

int Random::randstepIn(enhancedgraph *g, int color, const int node) {
	ColorMap *colors = g->colors;
    PNGraph graph = g->graph;

	//Find number of edges of same color
	int edges = 0;

	//#pragma omp critical
	//std::cout << "NodeI " << node << "\n";
	TNGraph::TNodeI NodeI = graph->GetNI(node);
	for (int v = 0; v < NodeI.GetInDeg(); v++)
	{
		int outNode = NodeI.GetInNId(v);

		if (colors->GetDat(outNode) == color)
		{
			edges += 1;
		}
	}
	
	if (edges <= 0) {
		return node;
	}
	int index = myRand(0, edges-1);
	edges = 0;

	//Find the edges with chosen index
	for (int v = 0; v < NodeI.GetInDeg(); v++)
	{
		int outNode = NodeI.GetInNId(v);

		if (colors->GetDat(outNode) == color)
		{
			if (edges == index) {
				return outNode;
			}
			edges += 1;
		}
	}

	return node;
}

int Random::randstepOut(enhancedgraph *g, int color, const int node) {
	ColorMap *colors = g->colors;
    PNGraph graph = g->graph;

	//Find number of edges of same color
	int edges = 0;

	//#pragma omp critical
	//std::cout << "NodeO " << node << "\n";
	TNGraph::TNodeI NodeI = graph->GetNI(node);

	for (int v = 0; v < NodeI.GetOutDeg(); v++)
	{
		int outNode = NodeI.GetOutNId(v);

		if (colors->GetDat(outNode) == color)
		{
			edges += 1;
		}
	}
	
	
	if (edges == 0) {
		return node;
	}
	int index = myRand(0, edges-1);
	edges = 0;

	//Find the edges with chosen index
	for (int v = 0; v < NodeI.GetOutDeg(); v++)
	{
		int outNode = NodeI.GetOutNId(v);

		if (colors->GetDat(outNode) == color)
		{
			if (edges == index) {
				return outNode;
			}
			edges += 1;
		}
	}

	return node;
}

//Performs a simple random walk for k iterations starting from the given node
//only looks at in/out edges of same color as start node
int Random::randwalk(enhancedgraph *g, int color, int node, const int k, int r, bool direction) {
	int currentNode = node;

	for (int i = 0; i < k; i++) {
		if (direction) {
			currentNode = randstepOut(g, color, currentNode);
		} else {
			currentNode = randstepIn(g, color, currentNode);
		}
	}

	return currentNode;
}

//Performs a simple random walk for k iterations starting from the given node
//only looks at in/out edges of same color as start node
int Random::randwalk(enhancedgraph *g, PNGraph graph, int color, int node, const int k) {
	int currentNode = node;

	for (int i = 0; i < k; i++) {
		currentNode = randstep(g, graph, color, currentNode);
	}

	return currentNode;
}
