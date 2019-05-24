#include "core.h"

ColorGenerator::ColorGenerator() {
	lastColor = 0;
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
	colors = new TIntH();
	colors->Gen(g->GetNodes());
	NIds = new TIntV(g->GetNodes());

	//Add all colors and node ids to colormap and node vector
	int i = 0;

	for (PNGraph::TObj::TNodeI NI = graph->BegNI(); NI < graph->EndNI(); NI++)
	{
		colors->AddDat(NI.GetId(), 0);
		NIds->SetVal(i, NI.GetId());

		i++;
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
	delete colors;
	delete NIds;
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

