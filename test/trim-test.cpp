#include <gtest/gtest.h>
#include <trim.h>

struct SimpleGraphTest : testing::Test {

    enhancedgraph* enhgraph;

    SimpleGraphTest() {
        PNGraph graph = TNGraph::New();
        graph->AddNode(1);
        graph->AddNode(2);
        graph->AddNode(3);
        graph->AddEdge(1, 2);
        graph->AddEdge(2, 3);
        graph->AddEdge(3, 1);

		graph->AddNode(4);
        graph->AddNode(5);
        graph->AddNode(6);
		graph->AddEdge(1, 4);
        graph->AddEdge(5, 2);
        graph->AddEdge(6, 3);

		graph->AddNode(7);
		graph->AddEdge(1, 7);
		graph->AddEdge(7, 1);

		enhgraph = new enhancedgraph(graph);
	}

    virtual ~SimpleGraphTest() {
        delete enhgraph;
    }
};

struct SimpleChainTest :testing::Test {

	enhancedgraph *enhgraph;

	SimpleChainTest() {
		PNGraph graph = TNGraph::New();
		graph->AddNode(1);
		graph->AddNode(2);
		graph->AddNode(3);
		graph->AddEdge(1, 2);
		graph->AddEdge(2, 3);

		graph->AddNode(4);
		graph->AddNode(5);
		graph->AddNode(6);
		graph->AddEdge(6, 5);
		graph->AddEdge(5, 4);

		enhgraph = new enhancedgraph(graph);
	}

	virtual ~SimpleChainTest() {
		delete enhgraph;
	}

};

struct ColorGraphTest : testing::Test {

    enhancedgraph* enhgraph;

    ColorGraphTest() {
        PNGraph graph = TNGraph::New();
        graph->AddNode(1);
        graph->AddNode(2);
        graph->AddNode(3);
        graph->AddEdge(1, 2);
        graph->AddEdge(2, 3);
        graph->AddEdge(3, 1);

		graph->AddNode(4);
        graph->AddNode(5);
        graph->AddNode(6);
		graph->AddEdge(1, 4);
        graph->AddEdge(5, 2);
        graph->AddEdge(6, 3);

		graph->AddNode(7);
		graph->AddEdge(1, 7);
		graph->AddEdge(7, 1);

		enhgraph = new enhancedgraph(graph);

		enhgraph->colors->AddDat(4, 1);
		enhgraph->colors->AddDat(5, 2);
		enhgraph->colors->AddDat(6, 3);
		enhgraph->colors->AddDat(7, 4);
	}

    virtual ~ColorGraphTest() {
        delete enhgraph;
    }
};

struct SimpleTrim2GraphTest : testing::Test {
	enhancedgraph *enhgraph;

	SimpleTrim2GraphTest()
	{
		PNGraph graph = TNGraph::New();
		graph->AddNode(1);
		graph->AddNode(2);
		graph->AddNode(3);
		graph->AddEdge(1, 2);
		graph->AddEdge(2, 1);
		graph->AddEdge(3, 1);

		enhgraph = new enhancedgraph(graph);
	}

	virtual ~SimpleTrim2GraphTest()
	{
		delete enhgraph;
	}
};

struct AdvancedTrim2GraphTest : testing::Test
{
	enhancedgraph *enhgraph;

	AdvancedTrim2GraphTest()
	{
		PNGraph graph = TNGraph::New();
		graph->AddNode(1);
		graph->AddNode(2);
		graph->AddNode(3);
		graph->AddNode(4);
		graph->AddEdge(1, 2);
		graph->AddEdge(2, 1);
		graph->AddEdge(1, 3);
		graph->AddEdge(3, 4);
		graph->AddEdge(4, 3);
		graph->AddEdge(4, 2);

		enhgraph = new enhancedgraph(graph);
	}

	virtual ~AdvancedTrim2GraphTest()
	{
		delete enhgraph;
	}
};

TEST_F(SimpleGraphTest, TrimFindsSCCs) {
	TIntH *colors = enhgraph->colors;
    trim::doTrim(1, enhgraph, 0);

	EXPECT_EQ(0, colors->GetDat(1));
	EXPECT_EQ(0, colors->GetDat(2));
	EXPECT_EQ(0, colors->GetDat(3));
	EXPECT_EQ(0, colors->GetDat(7));

	EXPECT_NE(0, colors->GetDat(4));
	EXPECT_NE(0, colors->GetDat(5));
	EXPECT_NE(0, colors->GetDat(6));

	EXPECT_NE(colors->GetDat(4), colors->GetDat(5));
	EXPECT_NE(colors->GetDat(5), colors->GetDat(6));
	EXPECT_NE(colors->GetDat(4), colors->GetDat(6));
}

TEST_F(SimpleGraphTest, ParTrimFindsSCCs)
{
	TIntH *colors = enhgraph->colors;
	trim::doTrim(1, enhgraph, 0);

	EXPECT_EQ(0, colors->GetDat(1));
	EXPECT_EQ(0, colors->GetDat(2));
	EXPECT_EQ(0, colors->GetDat(3));
	EXPECT_EQ(0, colors->GetDat(7));

	EXPECT_NE(0, colors->GetDat(4));
	EXPECT_NE(0, colors->GetDat(5));
	EXPECT_NE(0, colors->GetDat(6));

	EXPECT_NE(colors->GetDat(4), colors->GetDat(5));
	EXPECT_NE(colors->GetDat(5), colors->GetDat(6));
	EXPECT_NE(colors->GetDat(4), colors->GetDat(6));
}

TEST_F(SimpleGraphTest, TrimRespectsColors) {
	TIntH *colors = enhgraph->colors;
    trim::doTrim(1, enhgraph, 1);

	EXPECT_EQ(0, colors->GetDat(1));
	EXPECT_EQ(0, colors->GetDat(2));
	EXPECT_EQ(0, colors->GetDat(3));	
	EXPECT_EQ(0, colors->GetDat(4));	
	EXPECT_EQ(0, colors->GetDat(5));	
	EXPECT_EQ(0, colors->GetDat(6));	
	EXPECT_EQ(0, colors->GetDat(7));	
}

TEST_F(SimpleGraphTest, ParTrimRespectsColors)
{
	TIntH *colors = enhgraph->colors;
	trim::doParTrim(1, enhgraph, 1);

	EXPECT_EQ(0, colors->GetDat(1));
	EXPECT_EQ(0, colors->GetDat(2));
	EXPECT_EQ(0, colors->GetDat(3));
	EXPECT_EQ(0, colors->GetDat(4));
	EXPECT_EQ(0, colors->GetDat(5));
	EXPECT_EQ(0, colors->GetDat(6));
	EXPECT_EQ(0, colors->GetDat(7));
}

TEST_F(ColorGraphTest, TrimRespectsColorsDegree) {
	TIntH *colors = enhgraph->colors;
    trim::doTrim(1, enhgraph, 4);

	EXPECT_EQ(0, colors->GetDat(1));
	EXPECT_EQ(0, colors->GetDat(2));
	EXPECT_EQ(0, colors->GetDat(3));
	EXPECT_EQ(1, colors->GetDat(4));
	EXPECT_EQ(2, colors->GetDat(5));
	EXPECT_EQ(3, colors->GetDat(6));

	EXPECT_NE(0, colors->GetDat(7));
	EXPECT_NE(4, colors->GetDat(7));
}

TEST_F(ColorGraphTest, ParTrimRespectsColorsDegree)
{
	TIntH *colors = enhgraph->colors;
	trim::doParTrim(1, enhgraph, 4);

	EXPECT_EQ(0, colors->GetDat(1));
	EXPECT_EQ(0, colors->GetDat(2));
	EXPECT_EQ(0, colors->GetDat(3));
	EXPECT_EQ(1, colors->GetDat(4));
	EXPECT_EQ(2, colors->GetDat(5));
	EXPECT_EQ(3, colors->GetDat(6));

	EXPECT_NE(0, colors->GetDat(7));
	EXPECT_NE(4, colors->GetDat(7));
}

TEST_F(ColorGraphTest, TrimRespectsColors) {
	TIntH *colors = enhgraph->colors;
    trim::doTrim(1, enhgraph, 0);

	EXPECT_EQ(0, colors->GetDat(1));
	EXPECT_EQ(0, colors->GetDat(2));
	EXPECT_EQ(0, colors->GetDat(3));	
	EXPECT_EQ(1, colors->GetDat(4));	
	EXPECT_EQ(2, colors->GetDat(5));	
	EXPECT_EQ(3, colors->GetDat(6));	
	EXPECT_EQ(4, colors->GetDat(7));	
}

TEST_F(ColorGraphTest, ParTrimRespectsColors)
{
	TIntH *colors = enhgraph->colors;
	trim::doParTrim(1, enhgraph, 0);

	EXPECT_EQ(0, colors->GetDat(1));
	EXPECT_EQ(0, colors->GetDat(2));
	EXPECT_EQ(0, colors->GetDat(3));
	EXPECT_EQ(1, colors->GetDat(4));
	EXPECT_EQ(2, colors->GetDat(5));
	EXPECT_EQ(3, colors->GetDat(6));
	EXPECT_EQ(4, colors->GetDat(7));
}

TEST_F(ColorGraphTest, TrimFindsSCCs) {
	TIntH *colors = enhgraph->colors;
    trim::doParTrim(1, enhgraph, 2);

	EXPECT_EQ(0, colors->GetDat(1));
	EXPECT_EQ(0, colors->GetDat(2));
	EXPECT_EQ(0, colors->GetDat(3));
	EXPECT_EQ(1, colors->GetDat(4));
	EXPECT_EQ(3, colors->GetDat(6));
	EXPECT_EQ(4, colors->GetDat(7));

	EXPECT_NE(0, colors->GetDat(5));	
	EXPECT_NE(2, colors->GetDat(5));	
}

TEST_F(ColorGraphTest, ParTrimFindsSCCs)
{
	TIntH *colors = enhgraph->colors;
	trim::doParTrim(1, enhgraph, 2);

	EXPECT_EQ(0, colors->GetDat(1));
	EXPECT_EQ(0, colors->GetDat(2));
	EXPECT_EQ(0, colors->GetDat(3));
	EXPECT_EQ(1, colors->GetDat(4));
	EXPECT_EQ(3, colors->GetDat(6));
	EXPECT_EQ(4, colors->GetDat(7));

	EXPECT_NE(0, colors->GetDat(5));
	EXPECT_NE(2, colors->GetDat(5));
}

TEST_F(SimpleChainTest, TrimRunsMultiplePasses) {
	TIntH *colors = enhgraph->colors;
	trim::doTrim(1, enhgraph, 0);

	EXPECT_NE(0, colors->GetDat(1));
	EXPECT_NE(0, colors->GetDat(2));
	EXPECT_NE(0, colors->GetDat(3));
	EXPECT_NE(0, colors->GetDat(4));
	EXPECT_NE(0, colors->GetDat(5));
	EXPECT_NE(0, colors->GetDat(6));

	EXPECT_NE(colors->GetDat(1), colors->GetDat(2));
	EXPECT_NE(colors->GetDat(1), colors->GetDat(3));
	EXPECT_NE(colors->GetDat(1), colors->GetDat(4));
	EXPECT_NE(colors->GetDat(1), colors->GetDat(5));
	EXPECT_NE(colors->GetDat(1), colors->GetDat(6));

	EXPECT_NE(colors->GetDat(2), colors->GetDat(3));
	EXPECT_NE(colors->GetDat(2), colors->GetDat(4));
	EXPECT_NE(colors->GetDat(2), colors->GetDat(5));
	EXPECT_NE(colors->GetDat(2), colors->GetDat(6));

	EXPECT_NE(colors->GetDat(3), colors->GetDat(4));
	EXPECT_NE(colors->GetDat(3), colors->GetDat(5));
	EXPECT_NE(colors->GetDat(3), colors->GetDat(6));

	EXPECT_NE(colors->GetDat(4), colors->GetDat(5));
	EXPECT_NE(colors->GetDat(4), colors->GetDat(6));

	EXPECT_NE(colors->GetDat(5), colors->GetDat(6));
}

TEST_F(SimpleChainTest, ParTrimRunsMultiplePasses)
{
	TIntH *colors = enhgraph->colors;
	trim::doParTrim(1, enhgraph, 0);

	EXPECT_NE(0, colors->GetDat(1));
	EXPECT_NE(0, colors->GetDat(2));
	EXPECT_NE(0, colors->GetDat(3));
	EXPECT_NE(0, colors->GetDat(4));
	EXPECT_NE(0, colors->GetDat(5));
	EXPECT_NE(0, colors->GetDat(6));

	EXPECT_NE(colors->GetDat(1), colors->GetDat(2));
	EXPECT_NE(colors->GetDat(1), colors->GetDat(3));
	EXPECT_NE(colors->GetDat(1), colors->GetDat(4));
	EXPECT_NE(colors->GetDat(1), colors->GetDat(5));
	EXPECT_NE(colors->GetDat(1), colors->GetDat(6));

	EXPECT_NE(colors->GetDat(2), colors->GetDat(3));
	EXPECT_NE(colors->GetDat(2), colors->GetDat(4));
	EXPECT_NE(colors->GetDat(2), colors->GetDat(5));
	EXPECT_NE(colors->GetDat(2), colors->GetDat(6));

	EXPECT_NE(colors->GetDat(3), colors->GetDat(4));
	EXPECT_NE(colors->GetDat(3), colors->GetDat(5));
	EXPECT_NE(colors->GetDat(3), colors->GetDat(6));

	EXPECT_NE(colors->GetDat(4), colors->GetDat(5));
	EXPECT_NE(colors->GetDat(4), colors->GetDat(6));

	EXPECT_NE(colors->GetDat(5), colors->GetDat(6));
}

TEST_F(SimpleTrim2GraphTest, Trim2FindsSize2SCCs)
{
	TIntH *colors = enhgraph->colors;
	trim::doTrim(2, enhgraph, 0);

	EXPECT_NE(0, colors->GetDat(1));
	EXPECT_NE(0, colors->GetDat(2));

	EXPECT_EQ(colors->GetDat(1), colors->GetDat(1));
	EXPECT_NE(colors->GetDat(1), colors->GetDat(3));
}

TEST_F(AdvancedTrim2GraphTest, Trim2CorrectlyLooksUpNeighbours)
{
	TIntH *colors = enhgraph->colors;
	trim::doTrim(2, enhgraph, 0);

	EXPECT_EQ(0, colors->GetDat(1));
	EXPECT_EQ(0, colors->GetDat(2));
	EXPECT_EQ(0, colors->GetDat(3));
	EXPECT_EQ(0, colors->GetDat(4));
}

TEST_F(SimpleTrim2GraphTest, ParTrim2FindsSize2SCCs)
{
	TIntH *colors = enhgraph->colors;
	trim::doParTrim(2, enhgraph, 0);

	EXPECT_NE(0, colors->GetDat(1));
	EXPECT_NE(0, colors->GetDat(2));

	EXPECT_EQ(colors->GetDat(1), colors->GetDat(1));
	EXPECT_NE(colors->GetDat(1), colors->GetDat(3));
}
