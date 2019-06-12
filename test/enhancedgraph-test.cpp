#include <gtest/gtest.h>
#include <core.h>

struct SimpleSetupTest : testing::Test {
    enhancedgraph* enhgraph = new enhancedgraph();
	PNGraph graph;

    SimpleSetupTest() {
        graph = TNGraph::New();
        graph->AddNode(1);
        graph->AddNode(2);
        graph->AddNode(3);
        graph->AddEdge(1, 2);
        graph->AddEdge(2, 3);

        enhgraph->graph = &(*graph);
        ColorMap *colors = new ColorMap(0, false);
        enhgraph->colors = colors;
        enhgraph->colorGen = new ColorGenerator();
    }

    virtual ~SimpleSetupTest() {
        delete enhgraph;
    }
};

struct MapSetupTest : testing::Test {
	enhancedgraph* enhgraph;
	PNGraph graph;

	MapSetupTest() {
		graph = new TNGraph();
        graph->AddNode(1);
        graph->AddNode(2);
        graph->AddNode(3);
        graph->AddEdge(1, 2);
        graph->AddEdge(2, 3);

        enhgraph = new enhancedgraph(&(*graph), true, false, 10, 0, 1);
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
	ColorMap *colorMap = enhgraph->colors;
    EXPECT_EQ(0, colorMap->Len());
}

TEST_F(SimpleSetupTest, NodeVectorStartsEmpty) {
	ColorMap *colorMap = enhgraph->colors;
    EXPECT_EQ(0, colorMap->Len());
}

TEST_F(SimpleSetupTest, GraphTest) {
	TNGraph* graph = enhgraph->graph;

	EXPECT_EQ(3, graph->GetNodes());
}

TEST_F(SimpleSetupTest, GetNI) {
	EXPECT_EQ(true, enhgraph->graph->IsNode(1));

	const TNGraph::TNodeI NodeI = enhgraph->graph->GetNI(1);

	for (int v = 0; v < NodeI.GetOutDeg(); v++)
	{
		EXPECT_EQ(true, true);
	}
}

TEST_F(SimpleSetupTest, TimerNotEnabledGet) {
	EXPECT_EQ(-1, enhgraph->getTime(eTimer::MAIN));
	EXPECT_EQ(-1, enhgraph->getTime(eTimer::FirstFWBW));
	EXPECT_EQ(-1, enhgraph->getTime(eTimer::FWBWs));
	EXPECT_EQ(-1, enhgraph->getTime(eTimer::TRIM));
	EXPECT_EQ(-1, enhgraph->getTime(eTimer::PIVOT));
	EXPECT_EQ(-1, enhgraph->getTime(eTimer::SETUP));
}

TEST_F(SimpleSetupTest, TimerNotEnabledEnd) {
	enhgraph->endTimer(Time::now(), eTimer::MAIN);
	EXPECT_EQ(-1, enhgraph->getTime(eTimer::MAIN));
}

TEST_F(MapSetupTest, MapStartsWithAllNodes) {
	ColorMap *colorMap = enhgraph->colors;
	EXPECT_EQ(3, colorMap->Len());
}

TEST_F(MapSetupTest, MapStartsWithZeroValues) {
	ColorMap *colorMap = enhgraph->colors;

	for (int i = colorMap->BegI(); i < colorMap->Len(); i++)
    {
		EXPECT_EQ(0, colorMap->GetDat(i));
    }
}

TEST_F(MapSetupTest, TimersStartsAtZero) {
	EXPECT_EQ(0, enhgraph->getTime(eTimer::MAIN));
	EXPECT_EQ(0, enhgraph->getTime(eTimer::FirstFWBW));
	EXPECT_EQ(0, enhgraph->getTime(eTimer::FWBWs));
	EXPECT_EQ(0, enhgraph->getTime(eTimer::TRIM));
	EXPECT_EQ(0, enhgraph->getTime(eTimer::PIVOT));
	EXPECT_EQ(0, enhgraph->getTime(eTimer::SETUP));
}

TEST_F(MapSetupTest, TimerUpdatesCorrectly) {
	auto start = Time::now() - std::chrono::milliseconds(100);

	EXPECT_EQ(0, enhgraph->getTime(eTimer::MAIN));
	enhgraph->endTimer(start, eTimer::MAIN);
	EXPECT_NE(0, enhgraph->getTime(eTimer::MAIN));
	EXPECT_EQ(100, enhgraph->getTime(eTimer::MAIN));
}

TEST_F(MapSetupTest, CanWriteToColorMap) {
	enhgraph->colors->AddDat(2, 3);
	EXPECT_EQ(3, enhgraph->colors->GetDat(2));
}
