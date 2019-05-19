#include "core.h"

ColorGenerator::ColorGenerator() {
	lastColor = 0;
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
}

//TODO: make params for consts, also args in main
enhancedgraph::enhancedgraph() : TIMER_ENABLED(false), RAND_WALK_ITERATIONS(10){
	
}