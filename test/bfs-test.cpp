#include <gtest/gtest.h>
#include <bfs.h>
#include <list>

struct graph_state {
    int startnode;
    int scccolor;
    std::list<int> scclist;
    std::list<int> fwlist;
    std::list<int> bwlist;
};

struct SimpleReachabilityTest : testing::Test, testing::WithParamInterface<graph_state> {

    struct enhancedgraph* enhgraph;

    SimpleReachabilityTest() {
        PNGraph graph = TNGraph::New();
        graph->AddNode(1);
        graph->AddNode(2);
        graph->AddNode(3);
        graph->AddEdge(1, 2);
        graph->AddEdge(2, 3);

        enhgraph->graph = &graph;
        TIntH colors;
        colors(graph->GetNodes());
        enhgraph->colors = &colors;
        enhgraph->colorGen = new ColorGenerator();
    }

    virtual ~SimpleReachabilityTest() {
        delete enhgraph;
    }
};

TEST_P(SimpleReachabilityTest, BasicColors) {
    auto gs = GetParam();
    auto newcolors = bfs::colorbfs(enhgraph, gs.scccolor, gs.startnode);
    EXPECT_EQ(enhgraph->colors->GetDat(gs.startnode), gs.scccolor);
}

INSTANTIATE_TEST_CASE_P(Default, SimpleReachabilityTest,
    testing::Values(
        graph_state{1, 42, {1}, {2, 3}, {}}
    ));