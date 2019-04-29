#include <gtest/gtest.h>
#include <pivot.h>

struct pivot_state {
    int expectedOutput;
    int color;
};

struct SimpleGraphTest : testing::Test, testing::WithParamInterface<pivot_state> {

    enhancedgraph* enhgraph;

    SimpleGraphTest() {
        PNGraph graph = TNGraph::New();
        graph->AddNode(1);
        graph->AddNode(2);
        graph->AddNode(3);
        graph->AddEdge(1, 2);
        graph->AddEdge(2, 3);

		enhgraph = new enhancedgraph(graph);
	}

    virtual ~SimpleGraphTest() {
        delete enhgraph;
    }
};

struct ColorGraphTest : testing::Test, testing::WithParamInterface<pivot_state> {

    enhancedgraph* enhgraph;

    ColorGraphTest() {
        PNGraph graph = TNGraph::New();
        graph->AddNode(1);
        graph->AddNode(2);
        graph->AddNode(3);
        graph->AddEdge(1, 2);
        graph->AddEdge(2, 3);

		enhgraph = new enhancedgraph(graph);

		enhgraph->colors->AddDat(1, 2);
		enhgraph->colors->AddDat(3, 40);
	}

    virtual ~ColorGraphTest() {
        delete enhgraph;
    }
};

struct AdvancedColorGraphTest : testing::Test {

	enhancedgraph* enhgraph;

	AdvancedColorGraphTest()
	{
		PNGraph graph = TNGraph::New();
		graph->AddNode(1);
		graph->AddNode(2);
		graph->AddNode(3);
		graph->AddNode(4);
		graph->AddNode(5);
		graph->AddNode(6);
		graph->AddEdge(1, 2);
		graph->AddEdge(2, 1);
		graph->AddEdge(3, 2);
		graph->AddEdge(4, 3);
		graph->AddEdge(5, 3);
		graph->AddEdge(6, 3);

		enhgraph = new enhancedgraph(graph);

		enhgraph->colors->AddDat(1, 1);
		enhgraph->colors->AddDat(2, 1);
		enhgraph->colors->AddDat(3, 1);
		enhgraph->colors->AddDat(4, 2);
		enhgraph->colors->AddDat(5, 2);
		enhgraph->colors->AddDat(6, 2);
	}

	virtual ~AdvancedColorGraphTest()
	{
		delete enhgraph;
	}
};

TEST_P(SimpleGraphTest, CanFindStartnode) {
    auto gs = GetParam();
    int startnode = pivot::getPivot(enhgraph, gs.color);

	if (gs.expectedOutput == -1) {
		EXPECT_FALSE(enhgraph->graph->IsNode(startnode));

		for (THashKeyDatI<TInt, TInt> i = enhgraph->colors->BegI(); i < enhgraph->colors->EndI(); i++)
		{
			EXPECT_NE(gs.color, i.GetDat());
		}
	} else {
		EXPECT_TRUE(enhgraph->graph->IsNode(startnode));
		EXPECT_EQ(gs.expectedOutput, startnode);
		EXPECT_EQ(gs.color, enhgraph->colors->GetDat(startnode));
	}
}

TEST_P(ColorGraphTest, FindsCorrectNodeWhenMultipleColors) {
    auto gs = GetParam();
    int startnode = pivot::getPivot(enhgraph, gs.color);

	if (gs.expectedOutput == -1) {
		EXPECT_FALSE(enhgraph->graph->IsNode(startnode));

		for (THashKeyDatI<TInt, TInt> i = enhgraph->colors->BegI(); i < enhgraph->colors->EndI(); i++)
		{
			EXPECT_NE(gs.color, i.GetDat());
		}
	} else {
		EXPECT_TRUE(enhgraph->graph->IsNode(startnode));
		EXPECT_EQ(gs.expectedOutput, startnode);
		EXPECT_EQ(gs.color, enhgraph->colors->GetDat(startnode));
	}
}

TEST_F(AdvancedColorGraphTest, FindsMaxDegree)
{
	int startnode = pivot::getPivotMaxDegree(enhgraph, 1);

	EXPECT_EQ(startnode, 3);
}

TEST_F(AdvancedColorGraphTest, IgnoresNodesOfDifferentColor)
{
	int startnode = pivot::getPivotMaxDegreeColor(enhgraph, 1);

	EXPECT_EQ(startnode, 2);
}

INSTANTIATE_TEST_CASE_P(Default, SimpleGraphTest,
						testing::Values(
							pivot_state{1, 0},
							pivot_state{-1, 2}));
INSTANTIATE_TEST_CASE_P(Default, ColorGraphTest,
						testing::Values(
							pivot_state{2, 0},
							pivot_state{3, 40},
							pivot_state{1, 2},
							pivot_state{-1, 39}));