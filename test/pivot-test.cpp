#include <gtest/gtest.h>
#include <pivot.h>

struct pivot_state {
    int expectedOutput;
    int color;
	int method;

	friend std::ostream& operator<<(std::ostream& os, const pivot_state& obj) {
        return os
            << "expected output: " << obj.expectedOutput
            << ", color: " << obj.color
            << ", method: " << obj.method;
    }
};

struct SimpleGraphTest : testing::Test, testing::WithParamInterface<pivot_state> {

    enhancedgraph* enhgraph;

    SimpleGraphTest() {
        PNGraph graph = TNGraph::New();
        graph->AddNode(1);
        graph->AddNode(2);
        graph->AddNode(3);
        graph->AddNode(4);
        graph->AddNode(5);
        graph->AddNode(6);
        graph->AddNode(7);
        graph->AddNode(8);
        graph->AddNode(9);
        graph->AddNode(10);
        graph->AddNode(11);
        graph->AddNode(12);
        graph->AddNode(13);
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

TEST(Default, OmpCancelEnabled) {
	EXPECT_TRUE(omp_get_cancellation());
}

TEST_P(SimpleGraphTest, CanFindStartnode) {
    auto gs = GetParam();
    int startnode = pivot::findPivot(enhgraph, gs.color, gs.method);

	if (gs.expectedOutput == -1) {
		EXPECT_FALSE(enhgraph->graph->IsNode(startnode));

		for (THashKeyDatI<TInt, TInt> i = enhgraph->colors->BegI(); i < enhgraph->colors->EndI(); i++)
		{
			EXPECT_NE(gs.color, i.GetDat());
		}
	} else if (gs.method == 3) {
		EXPECT_TRUE(enhgraph->graph->IsNode(startnode));
		EXPECT_TRUE(startnode < omp_get_max_threads());
		EXPECT_EQ(gs.color, enhgraph->colors->GetDat(startnode));
	} else {
		EXPECT_TRUE(enhgraph->graph->IsNode(startnode));
		EXPECT_EQ(gs.expectedOutput, startnode);
		EXPECT_EQ(gs.color, enhgraph->colors->GetDat(startnode));
	}
}

TEST_P(ColorGraphTest, FindsCorrectNodeWhenMultipleColors) {
    auto gs = GetParam();
    int startnode = pivot::findPivot(enhgraph, gs.color, gs.method);

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
	int startnode = pivot::findPivot(enhgraph, 1, 1);

	EXPECT_EQ(startnode, 3);
}

TEST_F(AdvancedColorGraphTest, IgnoresNodesOfDifferentColor)
{
	int startnode = pivot::findPivot(enhgraph, 1, 2);

	EXPECT_EQ(startnode, 2);
}

TEST_F(AdvancedColorGraphTest, FindsMaxDegreePar)
{
	int startnode = pivot::findPivot(enhgraph, 1, 4);

	EXPECT_EQ(startnode, 3);
}

TEST_F(AdvancedColorGraphTest, IgnoresNodesOfDifferentColorPar)
{
	int startnode = pivot::findPivot(enhgraph, 1, 5);

	EXPECT_EQ(startnode, 2);
}

INSTANTIATE_TEST_CASE_P(Default, SimpleGraphTest,
						testing::Values(
							pivot_state{1, 0, 0},
							pivot_state{-1, 2, 0},
							pivot_state{1, 0, 3},
							pivot_state{-1, 2, 3}));
INSTANTIATE_TEST_CASE_P(Default, ColorGraphTest,
						testing::Values(
							pivot_state{2, 0, 0},
							pivot_state{3, 40, 0},
							pivot_state{1, 2, 0},
							pivot_state{-1, 39, 0},
							pivot_state{2, 0, 3},
							pivot_state{3, 40, 3},
							pivot_state{1, 2, 3},
							pivot_state{-1, 39, 3}));