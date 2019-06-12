#include <gtest/gtest.h>
#include <trim.h>

struct SimpleGraphTest : testing::Test {

    enhancedgraph* enhgraph;
	PNGraph graph;

    SimpleGraphTest() {
        graph = TNGraph::New();
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

		enhgraph = new enhancedgraph(&(*graph), false, false, 10, 0, 1);
	}

    virtual ~SimpleGraphTest() {
        delete enhgraph;
    }
};

struct SimpleChainTest :testing::Test {

	enhancedgraph *enhgraph;
	PNGraph graph;

	SimpleChainTest() {
		graph = TNGraph::New();
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

		enhgraph = new enhancedgraph(&(*graph), false, false, 10, 0, 1);
	}

	virtual ~SimpleChainTest() {
		delete enhgraph;
	}

};

struct ColorGraphTest : testing::Test {

    enhancedgraph* enhgraph;
	PNGraph graph;

    ColorGraphTest() {
        graph = TNGraph::New();
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

		enhgraph = new enhancedgraph(&(*graph), false, false, 10, 0, 1);

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
	PNGraph graph;

	SimpleTrim2GraphTest()
	{
		graph = TNGraph::New();
		graph->AddNode(1);
		graph->AddNode(2);
		graph->AddNode(3);
		graph->AddEdge(1, 2);
		graph->AddEdge(2, 1);
		graph->AddEdge(3, 1);

		enhgraph = new enhancedgraph(&(*graph), false, false, 10, 0, 1);
	}

	virtual ~SimpleTrim2GraphTest()
	{
		delete enhgraph;
	}
};

struct AdvancedTrim2GraphTest : testing::Test
{
	enhancedgraph *enhgraph;
	PNGraph graph;

	AdvancedTrim2GraphTest()
	{
		graph = TNGraph::New();
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

		enhgraph = new enhancedgraph(&(*graph), false, false, 10, 0, 1);
	}

	virtual ~AdvancedTrim2GraphTest()
	{
		delete enhgraph;
	}
};

struct SimpleTrim3Pattern1Test : testing::Test
{
	enhancedgraph *enhgraph;
	PNGraph graph;

	SimpleTrim3Pattern1Test()
	{
		graph = TNGraph::New();
		graph->AddNode(1);
		graph->AddNode(2);
		graph->AddNode(3);
		graph->AddNode(4);
		graph->AddNode(5);
		graph->AddNode(6);
		graph->AddNode(7);
		graph->AddEdge(1, 2);
		graph->AddEdge(2, 3);
		graph->AddEdge(3, 1);
		graph->AddEdge(4, 1);
		graph->AddEdge(5, 1);
		graph->AddEdge(6, 2);
		graph->AddEdge(7, 2);

		enhgraph = new enhancedgraph(&(*graph), false, false, 10, 0, 1);
	}

	virtual ~SimpleTrim3Pattern1Test()
	{
		delete enhgraph;
	}
};

struct SimpleTrim3Pattern2Test : testing::Test
{
	enhancedgraph *enhgraph;
	PNGraph graph;

	SimpleTrim3Pattern2Test()
	{
		graph = TNGraph::New();
		graph->AddNode(1);
		graph->AddNode(2);
		graph->AddNode(3);
		graph->AddNode(4);
		graph->AddNode(5);
		graph->AddNode(6);
		graph->AddNode(7);
		graph->AddEdge(1, 2);
		graph->AddEdge(1, 3);
		graph->AddEdge(3, 1);
		graph->AddEdge(2, 1);
		graph->AddEdge(4, 1);
		graph->AddEdge(5, 1);
		graph->AddEdge(6, 2);
		graph->AddEdge(7, 2);

		enhgraph = new enhancedgraph(&(*graph), false, false, 10, 0, 1);
	}

	virtual ~SimpleTrim3Pattern2Test()
	{
		delete enhgraph;
	}
};

struct Trim3MixedPatternTest : testing::Test
{
	enhancedgraph *enhgraph;
	PNGraph graph;

	Trim3MixedPatternTest()
	{
		graph = TNGraph::New();
		graph->AddNode(1);
		graph->AddNode(2);
		graph->AddNode(3);
		graph->AddNode(4);
		graph->AddNode(5);
		graph->AddNode(6);
		graph->AddEdge(1, 2);
		graph->AddEdge(2, 3);
		graph->AddEdge(3, 1);
		graph->AddEdge(1, 4);
		graph->AddEdge(4, 5);
		graph->AddEdge(5, 4);
		graph->AddEdge(5, 6);
		graph->AddEdge(6, 5);
		graph->AddEdge(2, 4);
		graph->AddEdge(2, 5);
		graph->AddEdge(3, 6);

		enhgraph = new enhancedgraph(&(*graph), false, false, 10, 0, 1);
	}

	virtual ~Trim3MixedPatternTest()
	{
		delete enhgraph;
	}
};

struct Trim3SelfEdgesTest : testing::Test
{
	enhancedgraph *enhgraph;
	PNGraph graph;

	Trim3SelfEdgesTest()
	{
		graph = TNGraph::New();
		graph->AddNode(1);
		graph->AddNode(2);
		graph->AddNode(3);
		graph->AddNode(4);
		graph->AddEdge(1, 2);
		graph->AddEdge(2, 3);
		graph->AddEdge(2, 1);
		graph->AddEdge(3, 2);
		graph->AddEdge(1, 1);
		graph->AddEdge(2, 2);
		graph->AddEdge(3, 3);
		graph->AddEdge(2, 4);

		enhgraph = new enhancedgraph(&(*graph), false, false, 10, 0, 1);
	}

	virtual ~Trim3SelfEdgesTest()
	{
		delete enhgraph;
	}
};

TEST_F(SimpleGraphTest, TrimFindsSCCs) {
	ColorMap *colors = enhgraph->colors;
    trim::doTrim(1, enhgraph, 0, enhgraph->colors->BegI(), enhgraph->colors->EndI()-1);

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
	ColorMap *colors = enhgraph->colors;
	trim::doTrim(1, enhgraph, 0, enhgraph->colors->BegI(), enhgraph->colors->EndI()-1);

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
	ColorMap *colors = enhgraph->colors;
    trim::doTrim(1, enhgraph, 1, enhgraph->colors->BegI(), enhgraph->colors->EndI()-1);

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
	ColorMap *colors = enhgraph->colors;
	trim::doParTrim(1, enhgraph, 1, enhgraph->colors->BegI(), enhgraph->colors->EndI()-1);

	EXPECT_EQ(0, colors->GetDat(1));
	EXPECT_EQ(0, colors->GetDat(2));
	EXPECT_EQ(0, colors->GetDat(3));
	EXPECT_EQ(0, colors->GetDat(4));
	EXPECT_EQ(0, colors->GetDat(5));
	EXPECT_EQ(0, colors->GetDat(6));
	EXPECT_EQ(0, colors->GetDat(7));
}

TEST_F(ColorGraphTest, TrimRespectsColorsDegree) {
	ColorMap *colors = enhgraph->colors;
    trim::doTrim(1, enhgraph, 4, enhgraph->colors->BegI(), enhgraph->colors->EndI()-1);

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
	ColorMap *colors = enhgraph->colors;
	trim::doParTrim(1, enhgraph, 4, enhgraph->colors->BegI(), enhgraph->colors->EndI()-1);

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
	ColorMap *colors = enhgraph->colors;
    trim::doTrim(1, enhgraph, 0, enhgraph->colors->BegI(), enhgraph->colors->EndI()-1);

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
	ColorMap *colors = enhgraph->colors;
	trim::doParTrim(1, enhgraph, 0, enhgraph->colors->BegI(), enhgraph->colors->EndI()-1);

	EXPECT_EQ(0, colors->GetDat(1));
	EXPECT_EQ(0, colors->GetDat(2));
	EXPECT_EQ(0, colors->GetDat(3));
	EXPECT_EQ(1, colors->GetDat(4));
	EXPECT_EQ(2, colors->GetDat(5));
	EXPECT_EQ(3, colors->GetDat(6));
	EXPECT_EQ(4, colors->GetDat(7));
}

TEST_F(ColorGraphTest, TrimFindsSCCs) {
	ColorMap *colors = enhgraph->colors;
    trim::doParTrim(1, enhgraph, 2, enhgraph->colors->BegI(), enhgraph->colors->EndI()-1);

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
	ColorMap *colors = enhgraph->colors;
	trim::doParTrim(1, enhgraph, 2, enhgraph->colors->BegI(), enhgraph->colors->EndI()-1);

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
	ColorMap *colors = enhgraph->colors;
	trim::doTrim(1, enhgraph, 0, enhgraph->colors->BegI(), enhgraph->colors->EndI()-1);

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
	ColorMap *colors = enhgraph->colors;
	trim::doParTrim(1, enhgraph, 0, enhgraph->colors->BegI(), enhgraph->colors->EndI()-1);

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
	ColorMap *colors = enhgraph->colors;
	trim::doTrim(2, enhgraph, 0, enhgraph->colors->BegI(), enhgraph->colors->EndI()-1);

	EXPECT_NE(0, colors->GetDat(1));
	EXPECT_NE(0, colors->GetDat(2));

	EXPECT_EQ(colors->GetDat(1), colors->GetDat(2));
	EXPECT_NE(colors->GetDat(1), colors->GetDat(3));
}

TEST_F(AdvancedTrim2GraphTest, Trim2CorrectlyLooksUpNeighbours)
{
	ColorMap *colors = enhgraph->colors;
	trim::doTrim(2, enhgraph, 0, enhgraph->colors->BegI(), enhgraph->colors->EndI()-1);

	EXPECT_EQ(0, colors->GetDat(1));
	EXPECT_EQ(0, colors->GetDat(2));
	EXPECT_EQ(0, colors->GetDat(3));
	EXPECT_EQ(0, colors->GetDat(4));
}

TEST_F(SimpleTrim2GraphTest, ParTrim2FindsSize2SCCs)
{
	ColorMap *colors = enhgraph->colors;
	trim::doParTrim(2, enhgraph, 0, enhgraph->colors->BegI(), enhgraph->colors->EndI()-1);

	EXPECT_NE(0, colors->GetDat(1));
	EXPECT_NE(0, colors->GetDat(2));

	EXPECT_EQ(colors->GetDat(1), colors->GetDat(1));
	EXPECT_NE(colors->GetDat(1), colors->GetDat(3));
}

TEST_F(SimpleTrim3Pattern1Test, Trim3FindsPattern1)
{
	ColorMap *colors = enhgraph->colors;
	EXPECT_EQ(colors->GetDat(3), 0);
	trim::doTrim(3, enhgraph, 0, enhgraph->colors->BegI(), enhgraph->colors->EndI()-1);
	int scc = colors->GetDat(1);

	EXPECT_NE(0, scc);
	EXPECT_NE(0, colors->GetDat(2));
	EXPECT_NE(0, colors->GetDat(3));

	EXPECT_NE(scc, colors->GetDat(4));
	EXPECT_NE(scc, colors->GetDat(5));
	EXPECT_NE(scc, colors->GetDat(6));
	EXPECT_NE(scc, colors->GetDat(7));

	EXPECT_EQ(scc, colors->GetDat(2));
	EXPECT_EQ(scc, colors->GetDat(3));
}

TEST_F(SimpleTrim3Pattern2Test, Trim3FindsPattern2)
{
	ColorMap *colors = enhgraph->colors;
	EXPECT_EQ(colors->GetDat(3), 0);
	trim::doTrim(3, enhgraph, 0, enhgraph->colors->BegI(), enhgraph->colors->EndI()-1);
	int scc = colors->GetDat(1);

	EXPECT_NE(0, scc);
	EXPECT_NE(0, colors->GetDat(2));
	EXPECT_NE(0, colors->GetDat(3));

	EXPECT_NE(scc, colors->GetDat(4));
	EXPECT_NE(scc, colors->GetDat(5));
	EXPECT_NE(scc, colors->GetDat(6));
	EXPECT_NE(scc, colors->GetDat(7));

	EXPECT_EQ(scc, colors->GetDat(2));
	EXPECT_EQ(scc, colors->GetDat(3));
}

TEST_F(Trim3MixedPatternTest, Trim3FindsMultipleSCCs)
{
	ColorMap *colors = enhgraph->colors;
	trim::doTrim(3, enhgraph, 0, enhgraph->colors->BegI(), enhgraph->colors->EndI()-1);
	int scc1 = colors->GetDat(1);
	int scc2 = colors->GetDat(4);

	EXPECT_NE(0, scc1);
	EXPECT_NE(0, colors->GetDat(2));
	EXPECT_NE(0, colors->GetDat(3));
	EXPECT_NE(0, colors->GetDat(4));
	EXPECT_NE(0, colors->GetDat(5));
	EXPECT_NE(0, colors->GetDat(6));

	EXPECT_NE(scc1, scc2);

	EXPECT_EQ(scc1, colors->GetDat(2));
	EXPECT_EQ(scc1, colors->GetDat(3));

	EXPECT_EQ(scc2, colors->GetDat(5));
	EXPECT_EQ(scc2, colors->GetDat(6));
};

TEST_F(SimpleTrim3Pattern1Test, ParTrim3FindsPattern1)
{
	ColorMap *colors = enhgraph->colors;
	trim::doParTrim(3, enhgraph, 0, enhgraph->colors->BegI(), enhgraph->colors->EndI()-1);
	int scc = colors->GetDat(1);

	EXPECT_NE(0, scc);
	EXPECT_NE(0, colors->GetDat(2));
	EXPECT_NE(0, colors->GetDat(3));

	EXPECT_NE(scc, colors->GetDat(4));
	EXPECT_NE(scc, colors->GetDat(5));
	EXPECT_NE(scc, colors->GetDat(6));
	EXPECT_NE(scc, colors->GetDat(7));

	EXPECT_EQ(scc, colors->GetDat(2));
	EXPECT_EQ(scc, colors->GetDat(3));
}

TEST_F(SimpleTrim3Pattern2Test, ParTrim3FindsPattern2)
{
	ColorMap *colors = enhgraph->colors;
	trim::doParTrim(3, enhgraph, 0, enhgraph->colors->BegI(), enhgraph->colors->EndI()-1);
	int scc = colors->GetDat(1);

	EXPECT_NE(0, scc);
	EXPECT_NE(0, colors->GetDat(2));
	EXPECT_NE(0, colors->GetDat(3));

	EXPECT_NE(scc, colors->GetDat(4));
	EXPECT_NE(scc, colors->GetDat(5));
	EXPECT_NE(scc, colors->GetDat(6));
	EXPECT_NE(scc, colors->GetDat(7));

	EXPECT_EQ(scc, colors->GetDat(2));
	EXPECT_EQ(scc, colors->GetDat(3));
}

TEST_F(Trim3MixedPatternTest, ParTrim3FindsMultipleSCCs)
{
	ColorMap *colors = enhgraph->colors;
	trim::doParTrim(3, enhgraph, 0, enhgraph->colors->BegI(), enhgraph->colors->EndI()-1);
	int scc1 = colors->GetDat(1);
	int scc2 = colors->GetDat(4);

	EXPECT_NE(0, scc1);
	EXPECT_NE(0, colors->GetDat(2));
	EXPECT_NE(0, colors->GetDat(3));
	EXPECT_NE(0, colors->GetDat(4));
	EXPECT_NE(0, colors->GetDat(5));
	EXPECT_NE(0, colors->GetDat(6));

	EXPECT_NE(scc1, scc2);

	EXPECT_EQ(scc1, colors->GetDat(2));
	EXPECT_EQ(scc1, colors->GetDat(3));

	EXPECT_EQ(scc2, colors->GetDat(5));
	EXPECT_EQ(scc2, colors->GetDat(6));
};

TEST_F(Trim3SelfEdgesTest, Trim3EdgeCase)
{
	ColorMap *colors = enhgraph->colors;
	trim::doTrim(3, enhgraph, 0, enhgraph->colors->BegI(), enhgraph->colors->EndI()-1);
	int scc1 = colors->GetDat(1);
	int scc2 = colors->GetDat(4);

	EXPECT_NE(0, scc1);
	EXPECT_NE(0, colors->GetDat(2));
	EXPECT_NE(0, colors->GetDat(3));

	EXPECT_NE(scc1, scc2);

	EXPECT_EQ(scc1, colors->GetDat(2));
	EXPECT_EQ(scc1, colors->GetDat(3));

};

TEST_F(Trim3SelfEdgesTest, ParTrim3EdgeCase)
{
	ColorMap *colors = enhgraph->colors;
	trim::doParTrim(3, enhgraph, 0, enhgraph->colors->BegI(), enhgraph->colors->EndI()-1);
	int scc1 = colors->GetDat(1);
	int scc2 = colors->GetDat(4);

	EXPECT_NE(0, scc1);
	EXPECT_NE(0, colors->GetDat(2));
	EXPECT_NE(0, colors->GetDat(3));

	EXPECT_NE(scc1, scc2);

	EXPECT_EQ(scc1, colors->GetDat(2));
	EXPECT_EQ(scc1, colors->GetDat(3));
};