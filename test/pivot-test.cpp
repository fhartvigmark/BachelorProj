#include <gtest/gtest.h>
#include <pivot.h>
#include <tuple>

struct pivot_state {
    int expectedOutput;
    int color;
	int method;
	bool parallel;

	friend std::ostream& operator<<(std::ostream& os, const pivot_state& obj) {
        return os
            << "expected output: " << obj.expectedOutput
            << ", color: " << obj.color
            << ", method: " << obj.method
			<< ", parallel: " << obj.parallel;
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

		enhgraph = new enhancedgraph(graph, false, false, 10);
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

		enhgraph = new enhancedgraph(graph, false, false, 10);

		enhgraph->colors->AddDat(1, 2);
		enhgraph->colors->AddDat(3, 40);
	}

    virtual ~ColorGraphTest() {
        delete enhgraph;
    }
};

struct AdvancedColorGraphTest : testing::Test, testing::WithParamInterface<pivot_state> {

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

		enhgraph = new enhancedgraph(graph, false, false, 10);

		enhgraph->colors->AddDat(1, 1);
		enhgraph->colors->AddDat(2, 1);
		enhgraph->colors->AddDat(3, 1);
		enhgraph->colors->AddDat(4, 2);
		enhgraph->colors->AddDat(5, 2);
		enhgraph->colors->AddDat(6, 2);

		enhgraph->calculateDegree();
	}

	virtual ~AdvancedColorGraphTest()
	{
		delete enhgraph->degree;
		delete enhgraph;
	}
};

TEST(Default, OmpCancelEnabled) {
	EXPECT_TRUE(omp_get_cancellation());
}

TEST_P(SimpleGraphTest, CanFindStartnode) {
    auto gs = GetParam();
	std::tuple<int, int, int> out;
    
	if (gs.parallel) {
		out = pivot::findParPivot(enhgraph, gs.color, gs.method, enhgraph->colors->BegI(), enhgraph->colors->EndI()-1);
	} else {
		out = pivot::findPivot(enhgraph, gs.color, gs.method, enhgraph->colors->BegI(), enhgraph->colors->EndI()-1);
	}

	int startnode = std::get<0>(out);
	

	if (gs.expectedOutput == -1) {
		EXPECT_FALSE(enhgraph->graph->IsNode(startnode));

		for (int i = enhgraph->colors->BegI(); i < enhgraph->colors->EndI(); i++)
		{
			EXPECT_NE(gs.color, enhgraph->colors->GetDat(i));
		}
	} else if (gs.method == 0 && gs.parallel) {
		EXPECT_TRUE(enhgraph->graph->IsNode(startnode));
		//EXPECT_TRUE(startnode <= omp_get_max_threads());
		//TODO: fix???
		EXPECT_EQ(gs.color, enhgraph->colors->GetDat(startnode));
	} else if (gs.method == 3) {
		EXPECT_TRUE(enhgraph->graph->IsNode(startnode));
		EXPECT_EQ(gs.color, enhgraph->colors->GetDat(startnode));
	} else {
		EXPECT_TRUE(enhgraph->graph->IsNode(startnode));
		EXPECT_EQ(gs.expectedOutput, startnode);
		EXPECT_EQ(gs.color, enhgraph->colors->GetDat(startnode));
	}
}

TEST_P(ColorGraphTest, FindsCorrectNodeWhenMultipleColors) {
    auto gs = GetParam();
    std::tuple<int, int, int> out;
    
	if (gs.parallel) {
		out = pivot::findParPivot(enhgraph, gs.color, gs.method, enhgraph->colors->BegI(), enhgraph->colors->EndI()-1);
	} else {
		out = pivot::findPivot(enhgraph, gs.color, gs.method, enhgraph->colors->BegI(), enhgraph->colors->EndI()-1);
	}

	int startnode = std::get<0>(out);

	if (gs.expectedOutput == -1) {
		EXPECT_FALSE(enhgraph->graph->IsNode(startnode));

		for (int i = enhgraph->colors->BegI(); i < enhgraph->colors->EndI(); i++)
		{
			EXPECT_NE(gs.color, enhgraph->colors->GetDat(i));
		}
	} else {
		EXPECT_TRUE(enhgraph->graph->IsNode(startnode));
		EXPECT_EQ(gs.expectedOutput, startnode);
		EXPECT_EQ(gs.color, enhgraph->colors->GetDat(startnode));
	}
}

TEST_P(AdvancedColorGraphTest, FindsMaxDegree) {
    auto gs = GetParam();
	std::tuple<int, int, int> out;
    
	if (gs.parallel) {
		out = pivot::findParPivot(enhgraph, gs.color, gs.method, enhgraph->colors->BegI(), enhgraph->colors->EndI()-1);
	} else {
		out = pivot::findPivot(enhgraph, gs.color, gs.method, enhgraph->colors->BegI(), enhgraph->colors->EndI()-1);
	}

	int startnode = std::get<0>(out);

	if (gs.expectedOutput == -1) {
		EXPECT_FALSE(enhgraph->graph->IsNode(startnode));

		for (int i = enhgraph->colors->BegI(); i < enhgraph->colors->EndI(); i++)
		{
			EXPECT_NE(gs.color, enhgraph->colors->GetDat(i));
		}
	} else if (gs.color == 2) {
		EXPECT_TRUE(enhgraph->graph->IsNode(startnode));
		EXPECT_TRUE(gs.expectedOutput > 3);
		EXPECT_TRUE(gs.expectedOutput < 7);
		EXPECT_EQ(gs.color, enhgraph->colors->GetDat(startnode));
	} else {
		EXPECT_TRUE(enhgraph->graph->IsNode(startnode));
		EXPECT_EQ(gs.expectedOutput, startnode);
		EXPECT_EQ(gs.color, enhgraph->colors->GetDat(startnode));
	}
}

INSTANTIATE_TEST_CASE_P(Default, AdvancedColorGraphTest,
						testing::Values(
							pivot_state{3, 1, 1, false},
							pivot_state{3, 1, 4, false},
							pivot_state{2, 1, 2, false},
							pivot_state{3, 1, 1, true},
							pivot_state{3, 1, 4, true},
							pivot_state{2, 1, 2, true},
							pivot_state{-1, 0, 1, false},
							pivot_state{-1, 0, 4, false},
							pivot_state{-1, 0, 2, false},
							pivot_state{-1, 0, 1, true},
							pivot_state{-1, 0, 4, true},
							pivot_state{-1, 0, 2, true},
							pivot_state{4, 2, 1, false},
							pivot_state{4, 2, 4, false},
							pivot_state{4, 2, 2, false},
							pivot_state{4, 2, 1, true},
							pivot_state{4, 2, 4, true},
							pivot_state{4, 2, 2, true}));

INSTANTIATE_TEST_CASE_P(Default, SimpleGraphTest,
						testing::Values(
							pivot_state{1, 0, 0, false},
							pivot_state{-1, 2, 0, false},
							pivot_state{1, 0, 0, true},
							pivot_state{-1, 2, 0, true},
							pivot_state{1, 0, 3, false},
							pivot_state{-1, 2, 3, false},
							pivot_state{1, 0, 3, true},
							pivot_state{-1, 2, 3, true}));
INSTANTIATE_TEST_CASE_P(Default, ColorGraphTest,
						testing::Values(
							pivot_state{2, 0, 0, false},
							pivot_state{3, 40, 0, false},
							pivot_state{1, 2, 0, false},
							pivot_state{-1, 39, 0, false},
							pivot_state{2, 0, 0, true},
							pivot_state{3, 40, 0, true},
							pivot_state{1, 2, 0, true},
							pivot_state{-1, 39, 0, true},
							pivot_state{2, 0, 3, false},
							pivot_state{3, 40, 3, false},
							pivot_state{1, 2, 3, false},
							pivot_state{-1, 39, 3, false},
							pivot_state{2, 0, 3, true},
							pivot_state{3, 40, 3, true},
							pivot_state{1, 2, 3, true},
							pivot_state{-1, 39, 3, true}));