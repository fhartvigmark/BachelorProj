#include <gtest/gtest.h>
#include <bfs.h>
#include <list>

struct graph_state {
	int variant;
    int startnode;
    int color;
    std::list<int> scclist;
    std::list<int> fwlist;
    std::list<int> bwlist;
};

std::pair<int, int> testBFS(int variant, enhancedgraph *g, int color, int startNode){
	switch (variant){
		case 0:
			return bfs::colorbfs(g, color, startNode);
		case 1:
			return bfs::parbfs(g, color, startNode);
		case 2:
			return bfs::relaxedSearch(g, color, startNode);
	}
}

struct SimpleReachabilityTest : testing::Test,
									testing::WithParamInterface<graph_state>
{

	enhancedgraph* enhgraph;

    SimpleReachabilityTest() {
        PNGraph graph = TNGraph::New();
        graph->AddNode(1);
        graph->AddNode(2);
        graph->AddNode(3);
        graph->AddEdge(1, 2);
        graph->AddEdge(2, 3);

		enhgraph = new enhancedgraph(graph, false, false, 10);
	}

    virtual ~SimpleReachabilityTest() {
        delete enhgraph;
    }
};

struct SimpleColorTest : testing::Test, testing::WithParamInterface<graph_state>
{

	enhancedgraph *enhgraph;

	SimpleColorTest()
	{
		PNGraph graph = TNGraph::New();
		graph->AddNode(1);
		graph->AddNode(2);
		graph->AddNode(3);
		graph->AddEdge(1, 2);
		graph->AddEdge(2, 3);

		enhgraph = new enhancedgraph(graph, false, false, 10);

		enhgraph->colors->AddDat(3, 42);
	}

	virtual ~SimpleColorTest()
	{
		delete enhgraph;
	}
};

struct SmallCycleTest : testing::Test, testing::WithParamInterface<graph_state>
{

	enhancedgraph *enhgraph;

	SmallCycleTest()
	{
		PNGraph graph = TNGraph::New();
		graph->AddNode(1);
		graph->AddNode(2);
		graph->AddEdge(1, 2);
		graph->AddEdge(2, 1);

		enhgraph = new enhancedgraph(graph, false, false, 10);

	}

	virtual ~SmallCycleTest()
	{
		delete enhgraph;
	}
};

struct AdvancedCycleTest : testing::Test, testing::WithParamInterface<graph_state>
{

	enhancedgraph *enhgraph;

	AdvancedCycleTest()
	{
		PNGraph graph = TNGraph::New();
		graph->AddNode(1);
		graph->AddNode(2);
		graph->AddNode(3);
		graph->AddNode(4);
		graph->AddNode(5);
		graph->AddEdge(1, 2);
		graph->AddEdge(2, 3);
		graph->AddEdge(2, 4);
		graph->AddEdge(4, 5);
		graph->AddEdge(5, 2);

		enhgraph = new enhancedgraph(graph, false, false, 10);
	}

	virtual ~AdvancedCycleTest()
	{
		delete enhgraph;
	}
};

TEST_P(SimpleReachabilityTest, BasicColors) {
    auto gs = GetParam();
    auto newcolors = testBFS(gs.variant, enhgraph, gs.color, gs.startnode);

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
	auto newcolors = testBFS(gs.variant, enhgraph, gs.color, gs.startnode);

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

TEST_P(SimpleColorTest, IgnoresDifferentColors)
{
	auto gs = GetParam();
	auto newcolors = testBFS(gs.variant, enhgraph, gs.color, gs.startnode);

	EXPECT_EQ(enhgraph->colors->GetDat(3), 42);

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

TEST_P(AdvancedCycleTest, DetectCycle)
{
	auto gs = GetParam();
	auto newcolors = testBFS(gs.variant, enhgraph, gs.color, gs.startnode);

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
							graph_state{0, 1, 0, {1}, {2, 3}, {}},
							graph_state{0, 2, 0, {2}, {3}, {1}},
							graph_state{0, 3, 0, {3}, {}, {1, 2}},
							graph_state{1, 1, 0, {1}, {2, 3}, {}},
							graph_state{1, 2, 0, {2}, {3}, {1}},
							graph_state{1, 3, 0, {3}, {}, {1, 2}},
							graph_state{2, 1, 0, {1}, {2, 3}, {}},
							graph_state{2, 2, 0, {2}, {3}, {1}},
							graph_state{2, 3, 0, {3}, {}, {1, 2}}));
INSTANTIATE_TEST_CASE_P(Default, SimpleColorTest,
						testing::Values(
							graph_state{0, 1, 0, {1}, {2}, {}},
							graph_state{0, 2, 0, {2}, {}, {1}},
							graph_state{1, 1, 0, {1}, {2}, {}},
							graph_state{1, 2, 0, {2}, {}, {1}},
							graph_state{2, 1, 0, {1}, {2}, {}},
							graph_state{2, 2, 0, {2}, {}, {1}}));
INSTANTIATE_TEST_CASE_P(Default, SmallCycleTest,
						testing::Values(
							graph_state{0, 1, 0, {1, 2}, {}, {}},
							graph_state{0, 2, 0, {1, 2}, {}, {} },
							graph_state{1, 1, 0, {1, 2}, {}, {}},
							graph_state{1, 2, 0, {1, 2}, {}, {}},
							graph_state{2, 1, 0, {1, 2}, {}, {}},
							graph_state{2, 2, 0, {1, 2}, {}, {}}));
INSTANTIATE_TEST_CASE_P(Default, AdvancedCycleTest,
						testing::Values(
							graph_state{0, 2, 0, {2, 4, 5}, {3}, {1}},
							graph_state{0, 4, 0, {2, 4, 5}, {3}, {1}},
							graph_state{0, 5, 0, {2, 4, 5}, {3}, {1}},
							graph_state{1, 2, 0, {2, 4, 5}, {3}, {1}},
							graph_state{1, 4, 0, {2, 4, 5}, {3}, {1}},
							graph_state{1, 5, 0, {2, 4, 5}, {3}, {1}},
							graph_state{2, 2, 0, {2, 4, 5}, {3}, {1}},
							graph_state{2, 4, 0, {2, 4, 5}, {3}, {1}},
							graph_state{2, 5, 0, {2, 4, 5}, {3}, {1}}));