#include "core.h"

ColorGenerator::ColorGenerator() {
	lastColor = 0;
}

//Measure end time and add that time to the corresponding duration based on the timer argument
void enhancedgraph::endTimer(std::chrono::high_resolution_clock::time_point start, eTimer timer) {
	if (TIMER_ENABLED)
	{
		auto end = std::chrono::high_resolution_clock::now();
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

int64_t enhancedgraph::getTime(eTimer timer) {
	if (!TIMER_ENABLED) {
		return -1;
	}

	switch (timer)
	{
		case eTimer::MAIN:
			return std::chrono::duration_cast<std::chrono::milliseconds>(tMain).count();
		case eTimer::FirstFWBW:
			return std::chrono::duration_cast<std::chrono::milliseconds>(tFirstFWBW).count();
		case eTimer::FWBWs:
			return std::chrono::duration_cast<std::chrono::milliseconds>(tFWBW).count();
		case eTimer::TRIM:
			return std::chrono::duration_cast<std::chrono::milliseconds>(tTrim).count();
		case eTimer::PIVOT:
			return std::chrono::duration_cast<std::chrono::milliseconds>(tPivot).count();
		case eTimer::SETUP:
			return std::chrono::duration_cast<std::chrono::milliseconds>(tSetup).count();
		default:
			return -1;
	}
}

enhancedgraph::enhancedgraph(PNGraph g) : TIMER_ENABLED(false), RAND_WALK_ITERATIONS(10){
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
		tMain = std::chrono::high_resolution_clock::duration::zero();
		tFirstFWBW = std::chrono::high_resolution_clock::duration::zero();
		tFWBW = std::chrono::high_resolution_clock::duration::zero();
		tTrim = std::chrono::high_resolution_clock::duration::zero();
		tPivot = std::chrono::high_resolution_clock::duration::zero();
		tSetup = std::chrono::high_resolution_clock::duration::zero();

		omp_init_lock(&lMain);
		omp_init_lock(&lFirstFWBW);
		omp_init_lock(&lFWBW);
		omp_init_lock(&lTrim);
		omp_init_lock(&lPivot);
		omp_init_lock(&lSetup);
	}
}

//TODO: make params for consts, also args in main
//Basic constructor, only sets constants and initilize timers
enhancedgraph::enhancedgraph() : TIMER_ENABLED(false), RAND_WALK_ITERATIONS(10){
	//Initialize durations and duration locks
	if (TIMER_ENABLED) {
		tMain = std::chrono::high_resolution_clock::duration::zero();
		tFirstFWBW = std::chrono::high_resolution_clock::duration::zero();
		tFWBW = std::chrono::high_resolution_clock::duration::zero();
		tTrim = std::chrono::high_resolution_clock::duration::zero();
		tPivot = std::chrono::high_resolution_clock::duration::zero();
		tSetup = std::chrono::high_resolution_clock::duration::zero();

		omp_init_lock(&lMain);
		omp_init_lock(&lFirstFWBW);
		omp_init_lock(&lFWBW);
		omp_init_lock(&lTrim);
		omp_init_lock(&lPivot);
		omp_init_lock(&lSetup);
	}
}