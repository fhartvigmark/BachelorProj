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
		callsFWBW++;

		if (depth > depthFWBW) {
			depthFWBW = depth;
		}
	}
}

void enhancedgraph::reportTrim(int color, int amount) {
	if (ANALYSE_ENABLED) {
		trimColor->push_back(color);
		trimAmount->push_back(amount);
	}
}

void enhancedgraph::reportPivot(int color, int node) {
	if (ANALYSE_ENABLED) {
		pivotColor->push_back(color);
		pivotNode->push_back(node);
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
	if (TIMER_ENABLED) {
		tMain = Duration::zero();
		tFirstFWBW = Duration::zero();
		tFWBW = Duration::zero();
		tTrim = Duration::zero();
		tPivot = Duration::zero();
		tSetup = Duration::zero();

		omp_init_lock(&lMain);
		omp_init_lock(&lFirstFWBW);
		omp_init_lock(&lFWBW);
		omp_init_lock(&lTrim);
		omp_init_lock(&lPivot);
		omp_init_lock(&lSetup);
	}

	//Initialize debug information variables and locks
	if (ANALYSE_ENABLED) {
		omp_init_lock(&lDebugFWBW);
		omp_init_lock(&lDebugTrim);
		omp_init_lock(&lDebugPivot);

		callsFWBW = 0;
		depthFWBW = 0;

		trimAmount = {};
		trimColor = {};
		pivotNode = {};
		pivotColor = {};
	}
}

//Basic constructor, only sets constants and initilize timers
enhancedgraph::enhancedgraph() : TIMER_ENABLED(false), ANALYSE_ENABLED(false), RAND_WALK_ITERATIONS(10){
	//Initialize durations and duration locks
	if (TIMER_ENABLED) {
		tMain = Duration::zero();
		tFirstFWBW = Duration::zero();
		tFWBW = Duration::zero();
		tTrim = Duration::zero();
		tPivot = Duration::zero();
		tSetup = Duration::zero();

		omp_init_lock(&lMain);
		omp_init_lock(&lFirstFWBW);
		omp_init_lock(&lFWBW);
		omp_init_lock(&lTrim);
		omp_init_lock(&lPivot);
		omp_init_lock(&lSetup);
	}

	//Initialize debug information variables and locks
	if (ANALYSE_ENABLED) {
		omp_init_lock(&lDebugFWBW);
		omp_init_lock(&lDebugTrim);
		omp_init_lock(&lDebugPivot);

		callsFWBW = 0;
		depthFWBW = 0;

		trimAmount = {};
		trimColor = {};
		pivotNode = {};
		pivotColor = {};
	}
}

enhancedgraph::~enhancedgraph() {
	//Delete graph elements
	//delete *graph; TODO: clean up graph?
	delete colors;
	delete NIds;
    delete colorGen;

	//Delete analysis elements
	if (ANALYSE_ENABLED) {
		omp_destroy_lock(&lDebugFWBW);
		omp_destroy_lock(&lDebugTrim);
		omp_destroy_lock(&lDebugPivot);

		delete trimAmount;
		delete trimColor;
		delete pivotNode;
		delete pivotColor;
	}

	//Delete timing elements
	if (TIMER_ENABLED) {
		omp_destroy_lock(&lMain);
		omp_destroy_lock(&lFirstFWBW);
		omp_destroy_lock(&lFWBW);
		omp_destroy_lock(&lTrim);
		omp_destroy_lock(&lPivot);
		omp_destroy_lock(&lSetup);
	}
}

