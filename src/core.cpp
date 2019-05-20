#include "core.h"

ColorGenerator::ColorGenerator() {
	lastColor = 0;
}

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

enhancedgraph::enhancedgraph(PNGraph g) : TIMER_ENABLED(false), RAND_WALK_ITERATIONS(10){
	graph = g;
	colorGen = new ColorGenerator();
	colors = new TIntH();
	colors->Gen(g->GetNodes());
	NIds = new TIntV(g->GetNodes());

	int i = 0;

	for (PNGraph::TObj::TNodeI NI = graph->BegNI(); NI < graph->EndNI(); NI++)
	{
		colors->AddDat(NI.GetId(), 0);
		NIds->SetVal(i, NI.GetId());

		i++;
	}

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
enhancedgraph::enhancedgraph() : TIMER_ENABLED(false), RAND_WALK_ITERATIONS(10){
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