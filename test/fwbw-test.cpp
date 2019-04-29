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