#include "main.h"
#include "scc.h"

int main(int argc, char **argv)
{
    // Input Parameters
    Env = TEnv(argc, argv, TNotify::StdNotify);
    Env.PrepArgs(TStr::Fmt("Circles. build: %s, %s. Time: %s",
                           __TIME__, __DATE__, TExeTm::GetCurTm()));
    const TStr InEdges =
        Env.GetIfArgPrefixStr("-g:", "fb1.edges", "Input graph (directed)");
    const TInt Trimlevels =
        Env.GetIfArgPrefixInt("-t:", 0, "Input number of trim levels");
    const TInt PivotMethod =
        Env.GetIfArgPrefixInt("-p:", 0, "Pivot selection (0 for random)");
    const TInt FwBwMethod =
        Env.GetIfArgPrefixInt("-m:", 0, "Specify FW-BW variant");

    // Load an example graph
    PNGraph Graph = TSnap::LoadEdgeList<PNGraph>(InEdges);
    int n = scc::FindSCCs(Graph, Trimlevels, PivotMethod, FwBwMethod);
    // Traverse the nodes
    for (TNGraph::TNodeI NI = Graph->BegNI(); NI < Graph->EndNI(); NI++)
    {
        printf("%d %d %d\n", NI.GetId(), NI.GetOutDeg(), NI.GetInDeg());
    }

    return 0;
}