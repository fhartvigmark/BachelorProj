#include <gtest/gtest.h>
#include <bfs.h>
#include <list>

struct graph_state {
    int startnode;
    int color;
    std::list<int> scclist;
    std::list<int> fwlist;
    std::list<int> bwlist;
};

struct SimpleReachabilityTest : testing::Test, testing::WithParamInterface<graph_state> {

    enhancedgraph* enhgraph = new enhancedgraph;

    SimpleReachabilityTest() {
        PNGraph graph = TNGraph::New();
        graph->AddNode(1);
        graph->AddNode(2);
        graph->AddNode(3);
        graph->AddEdge(1, 2);
        graph->AddEdge(2, 3);

        enhgraph->graph = &graph;
        TIntH *colors = new TIntH();
        enhgraph->colors = colors;
        enhgraph->colorGen = new ColorGenerator();
    }

    virtual ~SimpleReachabilityTest() {
        delete enhgraph;
    }
};

struct SmallCycleTest : testing::Test, testing::WithParamInterface<graph_state>
{

	enhancedgraph *enhgraph = new enhancedgraph;

	SmallCycleTest()
	{
		PNGraph graph = TNGraph::New();
		graph->AddNode(1);
		graph->AddNode(2);
		graph->AddEdge(1, 2);
		graph->AddEdge(2, 1);

		enhgraph->graph = &graph;
		TIntH *colors = new TIntH();
		enhgraph->colors = colors;
		enhgraph->colorGen = new ColorGenerator();
	}

	virtual ~SmallCycleTest()
	{
		delete enhgraph;
	}
};

TEST_P(SimpleReachabilityTest, BasicColors) {
    auto gs = GetParam();
    auto newcolors = bfs::colorbfs(enhgraph, gs.color, gs.startnode);

	for (int sccnode : gs.scclist)
	{
		EXPECT_NE(enhgraph->colors->GetDat(sccnode), gs.color);
		EXPECT_NE(enhgraph->colors->GetDat(sccnode), newcolors.first);
		EXPECT_NE(enhgraph->colors->GetDat(sccnode), newcolors.second);
	}

	for (int fwnode: gs.fwlist) {
		EXPECT_EQ(enhgraph->colors->GetDat(fwnode), newcolors.first);
	}

	for (int bwnode : gs.bwlist) {
		EXPECT_EQ(enhgraph->colors->GetDat(bwnode), newcolors.second);
	}
}

TEST_P(SmallCycleTest, BasicCycle)
{
	auto gs = GetParam();
	auto newcolors = bfs::colorbfs(enhgraph, gs.color, gs.startnode);

	for (int sccnode : gs.scclist)
	{
		EXPECT_NE(enhgraph->colors->GetDat(sccnode), gs.color);
		EXPECT_NE(enhgraph->colors->GetDat(sccnode), newcolors.first);
		EXPECT_NE(enhgraph->colors->GetDat(sccnode), newcolors.second);
	}

	for (int fwnode : gs.fwlist)
	{
		EXPECT_EQ(enhgraph->colors->GetDat(fwnode), newcolors.first);
	}

	for (int bwnode : gs.bwlist)
	{
		EXPECT_EQ(enhgraph->colors->GetDat(bwnode), newcolors.second);
	}
}

INSTANTIATE_TEST_CASE_P(Default, SimpleReachabilityTest,
						testing::Values(
							graph_state{1, 0, {1}, {2, 3}, {}},
							graph_state{2, 0, {2}, {3}, {1}},
							graph_state{3, 0, {3}, {}, {1, 2}}));

INSTANTIATE_TEST_CASE_P(Default, SmallCycleTest,
						testing::Values(
							graph_state{1, 0, {1, 2}, {}, {}},
							graph_state{2, 0, {1, 2}, {}, {}}));