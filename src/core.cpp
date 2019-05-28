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

void enhancedgraph::reportFWBW(int depth) {
	if (ANALYSE_ENABLED) {
		omp_set_lock(&lDebugFWBW);
		callsFWBW++;

		if (depth > depthFWBW) {
			depthFWBW = depth;
		}
		omp_unset_lock(&lDebugFWBW);
	}
}

void enhancedgraph::reportBFS(int color, int amount) {
	if (ANALYSE_ENABLED) {
		omp_set_lock(&lDebugBFS);
		bfsColor->push_back(color);
		bfsAmount->push_back(amount);
		omp_unset_lock(&lDebugBFS);
	}
}

void enhancedgraph::reportTrim(int color, int amount, int type) {
	if (ANALYSE_ENABLED) {
		omp_set_lock(&lDebugTrim);
		trimColor->push_back(color);
		trimAmount->push_back(amount);
		trimType->push_back(type);

		callsTrim++;
		omp_unset_lock(&lDebugTrim);
	}
}

void enhancedgraph::reportPivot(int color, int node) {
	if (ANALYSE_ENABLED) {
		omp_set_lock(&lDebugPivot);
		if (node > -1) {
			pivotColor->push_back(color);
			pivotNode->push_back(node);
		}
		
		callsPivot++;
		omp_unset_lock(&lDebugPivot);
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
		case eDebug::tAmount:
			return trimAmount;
		case eDebug::tColor:
			return trimColor;
		case eDebug::tType:
			return trimType;
		case eDebug::pColor:
			return pivotColor;
		case eDebug::pNode:
			return pivotNode;
		case eDebug::bAmount:
			return bfsAmount;
		case eDebug::bColor:
			return bfsColor;
		default:
			return {};
	}
}


enhancedgraph::enhancedgraph(PNGraph g, bool timer, bool analyse, int randwalk_iterations) : TIMER_ENABLED(timer), ANALYSE_ENABLED(analyse), RAND_WALK_ITERATIONS(randwalk_iterations){
	graph = g;
	colorGen = new ColorGenerator();
	//colors = new TIntH();
	//colors->Gen(g->GetNodes());
	bool zero = g->IsNode(0);
	//std::cout << "Is 12 a node: " << g->IsNode(12) << "\n";
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
		omp_init_lock(&lDebugBFS);
		omp_init_lock(&lDebugTrim);
		omp_init_lock(&lDebugPivot);

		callsFWBW = 0;
		callsTrim = 0;
		callsPivot = 0;
		depthFWBW = 0;

		trimAmount = new std::list<int>;
		trimColor = new std::list<int>;
		trimType = new std::list<int>;
		pivotNode = new std::list<int>;
		pivotColor = new std::list<int>;
		bfsColor = new std::list<int>;
		bfsAmount = new std::list<int>;
	}
}

//Basic constructor, only sets constants and initilize timers
enhancedgraph::enhancedgraph() : TIMER_ENABLED(false), ANALYSE_ENABLED(false), RAND_WALK_ITERATIONS(10){
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
		omp_init_lock(&lDebugBFS);
		omp_init_lock(&lDebugTrim);
		omp_init_lock(&lDebugPivot);

		callsFWBW = 0;
		callsTrim = 0;
		callsPivot = 0;
		depthFWBW = 0;

		trimAmount = {};
		trimColor = {};
		trimType = {};
		pivotNode = {};
		pivotColor = {};
		bfsColor = {};
		bfsAmount = {};
	}
}

enhancedgraph::~enhancedgraph() {
	//Delete graph elements
	//delete *graph; TODO: clean up graph?
	//TODO: clean up degree array
	delete colors;
    delete colorGen;

	//Delete analysis elements
	if (true) {
		omp_destroy_lock(&lDebugFWBW);
		omp_destroy_lock(&lDebugBFS);
		omp_destroy_lock(&lDebugTrim);
		omp_destroy_lock(&lDebugPivot);

		delete trimAmount;
		delete trimColor;
		delete trimType;
		delete pivotNode;
		delete pivotColor;
		delete bfsColor;
		delete bfsAmount;
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

int Random::myRand(unsigned int seed, int limit) {
	return (rand_r(&seed)%limit);
}

int Random::randstep(enhancedgraph *g, int color, int node, unsigned int seed) {
	ColorMap *colors = g->colors;
    PNGraph graph = g->graph;

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
	int index = myRand(seed, edges);
	edges = 0;

	//Find the edges with chosen index
	NodeI = graph->GetNI(node);
	for (v = 0; v < NodeI.GetOutDeg(); v++)
	{
		const int outNode = NodeI.GetOutNId(v);

		if (colors->GetDat(outNode) == color)
		{
			if (edges == index) {
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
				return outNode;
			}
			edges += 1;
		}
	}

	return node;
}

int Random::randstepIn(enhancedgraph *g, int color, const int node, unsigned int seed) {
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
	int index = myRand(seed, edges);
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

int Random::randstepOut(enhancedgraph *g, int color, const int node, unsigned int seed) {
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
	int index = myRand(seed, edges);
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
	unsigned int seed = time(NULL) * (r+1);
	int currentNode = node;

	for (int i = 0; i < k; i++) {
		if (direction) {
			currentNode = randstepOut(g, color, currentNode, seed);
		} else {
			currentNode = randstepIn(g, color, currentNode, seed);
		}
	}

	return currentNode;
}

//Performs a simple random walk for k iterations starting from the given node
//only looks at in/out edges of same color as start node
int Random::randwalk(enhancedgraph *g, int color, int node, const int k) {
	unsigned int seed = time(NULL);
	int currentNode = node;

	for (int i = 0; i < k; i++) {
		currentNode = randstep(g, color, currentNode, seed);
	}

	return currentNode;
}
