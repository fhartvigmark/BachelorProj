#include "main.h"
#include "scc.h"

int main(int argc, char **argv)
{
    // Input Parameters
    Env = TEnv(argc, argv, TNotify::StdNotify);
    Env.PrepArgs(TStr::Fmt("SCC. build: %s, %s. Time: %s",
                           __TIME__, __DATE__, TExeTm::GetCurTm()));
    const TStr InEdges =
        Env.GetIfArgPrefixStr("-g ", "fb1.edges", "Input graph (directed)");
    const TInt Trimlevels =
        Env.GetIfArgPrefixInt("-t ", 0, "Input number of trim levels");
    const TInt PivotMethod =
        Env.GetIfArgPrefixInt("-p ", 0, "Pivot selection (0 for random)");
    const TInt FwBwMethod =
        Env.GetIfArgPrefixInt("-m ", 0, "Specify FW-BW variant");
	const TInt Timer =
        Env.GetIfArgPrefixInt("-time ", 0, "Time execution");
	const TInt Output =
        Env.GetIfArgPrefixInt("-out ", 0, "Print SCC output to file");

    // Load the graph 
    PNGraph Graph = TSnap::LoadEdgeList<PNGraph>(InEdges);

	scc::FindSCCs(Graph, 0, 0, 0);

	//TODO add printing and timing handler here

    return 0;
}