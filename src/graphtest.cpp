#include "graphtest.h"

int main(int argc, char** argv) {
	// Input Parameters
	Env = TEnv(argc, argv, TNotify::StdNotify);
	Env.PrepArgs(TStr::Fmt("Circles. build: %s, %s. Time: %s",
							 __TIME__, __DATE__, TExeTm::GetCurTm()));
	const TStr InEdges =
	Env.GetIfArgPrefixStr("-e:", "fb1.edges", "Input graph (undirected)");
	const TStr INFeatures =
	Env.GetIfArgPrefixStr("-f:", "fb1.feat", "Input node Features");
	const TStr InGT =
	Env.GetIfArgPrefixStr("-g:", "fb1.circles", "Groundtruth circles");
	const TInt K =
	Env.GetIfArgPrefixInt("-K:", 4, "Number of circles to detect");
	const TFlt Lambda =
	Env.GetIfArgPrefixFlt("-l:", 1, "Regularization Parameter Lambda");

	// Load an example graph
	PUNGraph Graph = TSnap::LoadEdgeList<PUNGraph>(InEdges);
	
	// Traverse the nodes
	for (TUNGraph::TNodeI NI = Graph->BegNI(); NI < Graph->EndNI(); NI++)
		printf("%d %d %d\n", NI.GetId(), NI.GetOutDeg(), NI.GetInDeg());

	
	return 0;
}
