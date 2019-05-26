#pragma once
#include "Snap.h"
#include <atomic>
#include <chrono>
#include <list>

enum ePivot{Random, Max, MaxColor, ParRandom, ParMax, ParMaxColor, RandWalk};
enum eMethod{FWBW, ParFWBW, RecFWBW};
enum eTrim{Trim1, Trim2, Trim3, ParTrim1, ParTrim2, ParTrim3};
enum eTimer{MAIN, FirstFWBW, FWBWs, TRIM, FirstTRIM, TRIM1, TRIM2, TRIM3, PIVOT, SETUP};
enum eDebug{tAmount, tColor, tType, pColor, pNode, bColor, bAmount};

typedef std::chrono::high_resolution_clock Time;
typedef std::chrono::high_resolution_clock::time_point TimePoint;
typedef std::chrono::high_resolution_clock::duration Duration;
typedef std::chrono::milliseconds Ms;

class ColorGenerator
{
    public:
        int getNext() { return ++lastColor; }
		ColorGenerator();
    private:
        std::atomic<int> lastColor;
};

class enhancedgraph
{
	public:
		const int RAND_WALK_ITERATIONS;
		const bool TIMER_ENABLED;
		const bool ANALYSE_ENABLED;

    	PNGraph graph;
		TIntH *colors;
		TIntV *NIds;
    	ColorGenerator *colorGen;

		TimePoint startTimer();
		void endTimer(TimePoint start, eTimer timer);
		int64_t getTime(eTimer timer);

		void reportFWBW(int depth);
		void reportBFS(int color, int amount);
		void reportTrim(int color, int amount, int type);
		void reportPivot(int color, int node);

		int64_t getCallsFWBW();
		int64_t getCallsTrim();
		int64_t getCallsPivot();
		int64_t getDepth();
		std::list<int>* getReports(eDebug data);


		enhancedgraph(PNGraph g, bool timer, bool analyse, int randwalk_iterations);
		enhancedgraph();

		~enhancedgraph();
	private:
		std::list<int> *trimAmount;
		std::list<int> *trimColor;
		std::list<int> *trimType;
		std::list<int> *pivotNode;
		std::list<int> *pivotColor;
		std::list<int> *bfsAmount;
		std::list<int> *bfsColor;

        Duration tMain;
        Duration tFirstFWBW;
        Duration tFWBW;
        Duration tTrim;
        Duration tTrim1;
        Duration tTrim2;
        Duration tTrim3;
        Duration tFirstTrim;
        Duration tPivot;
        Duration tSetup;

		omp_lock_t lMain;
        omp_lock_t lFirstFWBW;
        omp_lock_t lFWBW;
        omp_lock_t lTrim;
        omp_lock_t lFirstTrim;
        omp_lock_t lPivot;
        omp_lock_t lSetup;
		omp_lock_t lDebugFWBW;
		omp_lock_t lDebugBFS;
		omp_lock_t lDebugTrim;
		omp_lock_t lDebugPivot;
		
		int64_t callsFWBW;
		int64_t callsTrim;
		int64_t callsPivot;
		int64_t depthFWBW;
};

class Random
{
	public:
		static int randwalk(enhancedgraph *g, int color, int node, const int k);
		static int randwalk(enhancedgraph *g, int color, int node, const int k, int r, bool direction);
	private:
		static int randstep(enhancedgraph *g, int color, int node, unsigned int seed);
		static int randstepOut(enhancedgraph *g, int color, const int node, unsigned int seed);
		static int randstepIn(enhancedgraph *g, int color, const int node, unsigned int seed);
		static int myRand(unsigned int seed, int limit);	
};