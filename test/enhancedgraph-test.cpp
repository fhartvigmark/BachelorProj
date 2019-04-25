#include <gtest/gtest.h>
#include <core.h>

struct SimpleSetupTest : testing::Test {
    enhancedgraph* enhgraph = new enhancedgraph();

    SimpleSetupTest() {
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

    virtual ~SimpleSetupTest() {
        delete enhgraph;
    }
};

struct MapSetupTest : testing::Test {
	enhancedgraph* enhgraph;

	MapSetupTest() {
		PNGraph graph = TNGraph::New();
        graph->AddNode(1);
        graph->AddNode(2);
        graph->AddNode(3);
        graph->AddEdge(1, 2);
        graph->AddEdge(2, 3);

        enhgraph = new enhancedgraph(&graph);
	}

	virtual ~MapSetupTest() {
        delete enhgraph;
    }
};

TEST_F(SimpleSetupTest, ColorIncrementEachTime) {
	int color1 = enhgraph->colorGen->getNext();
	int color2 = enhgraph->colorGen->getNext();

    EXPECT_EQ(1, color1);
    EXPECT_EQ(2, color2);
}

TEST_F(SimpleSetupTest, HashMapStartsEmpty) {
	TIntH *colorMap = enhgraph->colors;
    EXPECT_EQ(0, colorMap->Len());
}

TEST_F(MapSetupTest, MapStartsWithAllNodes) {
	TIntH *colorMap = enhgraph->colors;
	EXPECT_EQ(3, colorMap->Len());
}

TEST_F(MapSetupTest, MapStartsWithZeroValues) {
	TIntH *colorMap = enhgraph->colors;

	for (THashKeyDatI<TInt, TInt> i = colorMap->BegI(); i < colorMap->EndI(); i++)
    {
		EXPECT_EQ(0, i.GetDat());
    }
}



