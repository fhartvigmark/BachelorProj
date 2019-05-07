#include <gtest/gtest.h>
#include <fwbw.h>

struct BasicFWBWTest : testing::Test {
	enhancedgraph *enhgraph;

	BasicFWBWTest()
	{
		PNGraph graph = TNGraph::New();
		graph->AddNode(1);
		graph->AddNode(2);
		graph->AddNode(3);
		graph->AddNode(4);
		graph->AddEdge(1, 2);
		graph->AddEdge(2, 1);
		graph->AddEdge(2, 3);
		graph->AddEdge(3, 4);
		graph->AddEdge(4, 3);

		enhgraph = new enhancedgraph(graph);
	}

	virtual ~BasicFWBWTest()
	{
		delete enhgraph;
	}
};

//Graph from figure 1
struct Graph1Test : testing::Test {
	enhancedgraph *enhgraph;

	Graph1Test()
	{
		PNGraph graph = TNGraph::New();
		graph->AddNode(1);
		graph->AddNode(2);
		graph->AddNode(3);
		graph->AddNode(4);
		graph->AddNode(5);
		graph->AddNode(6);
		graph->AddNode(7);
		graph->AddEdge(3, 1);
		graph->AddEdge(3, 2);
		graph->AddEdge(3, 7);
		graph->AddEdge(1, 4);
		graph->AddEdge(4, 3);
		graph->AddEdge(6, 4);
		graph->AddEdge(2, 5);
		graph->AddEdge(5, 2);
		graph->AddEdge(7, 5);

		enhgraph = new enhancedgraph(graph);
	}

	virtual ~Graph1Test()
	{
		delete enhgraph;
	}
};

TEST_F(Graph1Test, SCCheck){
	int status = fwbw::basicFWBW(enhgraph, 0, 0, 0);
	//No error
	EXPECT_EQ(status, 0);
	int scc1_1 = enhgraph->colors->GetDat(1);
	int scc2_1 = enhgraph->colors->GetDat(2);
	int scc1_2 = enhgraph->colors->GetDat(3);
	int scc1_3 = enhgraph->colors->GetDat(4);
	int scc2_2 = enhgraph->colors->GetDat(5);
	int scc3 = enhgraph->colors->GetDat(6);
	int scc4 = enhgraph->colors->GetDat(7);

	//Two SCCs found
	EXPECT_NE(scc1_1, scc2_1);
	EXPECT_NE(scc1_1, scc3);
	EXPECT_NE(scc1_1, scc4);
	EXPECT_NE(scc2_1, scc3);
	EXPECT_NE(scc2_1, scc4);
	EXPECT_NE(scc3, scc4);

	//SCCs contain correct nodes
	EXPECT_EQ(scc1_1, scc1_2);
	EXPECT_EQ(scc1_1, scc1_3);
	EXPECT_EQ(scc2_1, scc2_2);

	//SCCs have correct color
	EXPECT_NE(scc1_1, 0);
	EXPECT_NE(scc2_1, 0);
	EXPECT_NE(scc3, 0);
	EXPECT_NE(scc4, 0);
}

TEST_F(Graph1Test, ParSCCCheck)
{
	int status = fwbw::parFWBW(enhgraph, 0, 0, 0);
	//No error
	EXPECT_EQ(status, 0);
	int scc1_1 = enhgraph->colors->GetDat(1);
	int scc2_1 = enhgraph->colors->GetDat(2);
	int scc1_2 = enhgraph->colors->GetDat(3);
	int scc1_3 = enhgraph->colors->GetDat(4);
	int scc2_2 = enhgraph->colors->GetDat(5);
	int scc3 = enhgraph->colors->GetDat(6);
	int scc4 = enhgraph->colors->GetDat(7);

	//Two SCCs found
	EXPECT_NE(scc1_1, scc2_1);
	EXPECT_NE(scc1_1, scc3);
	EXPECT_NE(scc1_1, scc4);
	EXPECT_NE(scc2_1, scc3);
	EXPECT_NE(scc2_1, scc4);
	EXPECT_NE(scc3, scc4);

	//SCCs contain correct nodes
	EXPECT_EQ(scc1_1, scc1_2);
	EXPECT_EQ(scc1_1, scc1_3);
	EXPECT_EQ(scc2_1, scc2_2);

	//SCCs have correct color
	EXPECT_NE(scc1_1, 0);
	EXPECT_NE(scc2_1, 0);
	EXPECT_NE(scc3, 0);
	EXPECT_NE(scc4, 0);
}

TEST_F(BasicFWBWTest, ColorCheck){
	int status = fwbw::basicFWBW(enhgraph, 0, 0, 0);
	//No error
	EXPECT_EQ(status, 0);
	int scc1 = enhgraph->colors->GetDat(1);
	int scc2 = enhgraph->colors->GetDat(3);

	//Two SCCs found
	EXPECT_NE(scc1, scc2);

	//SCCs contain correct nodes
	EXPECT_EQ(enhgraph->colors->GetDat(2), scc1);
	EXPECT_EQ(enhgraph->colors->GetDat(4), scc2);

	//SCCs have correct color
	EXPECT_NE(scc1, 0);
	EXPECT_NE(scc2, 0);
}