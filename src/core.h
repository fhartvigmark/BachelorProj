#pragma once
#include "Snap.h"
#include <atomic>
#include <chrono>
#include <list>

enum ePivot{Random, Max, MaxColor, ParRandom, ParMax, ParMaxColor, RandWalk};
enum eMethod{FWBW, ParFWBW, RecFWBW};
enum eTrim{Trim1, Trim2, Trim3, ParTrim1, ParTrim2, ParTrim3};
enum eTimer{MAIN, FirstFWBW, FWBWs, TRIM, FirstTRIM, PIVOT, SETUP};

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
		void reportTrim(int color, int amount);
		void reportPivot(int color, int node);

		enhancedgraph(PNGraph g, bool timer, bool analyse, int randwalk_iterations);
		enhancedgraph();

		~enhancedgraph();
	private:
		std::list<int> *trimAmount;
		std::list<int> *trimColor;
		std::list<int> *pivotNode;
		std::list<int> *pivotColor;

        Duration tMain;
        Duration tFirstFWBW;
        Duration tFWBW;
        Duration tTrim;
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
		omp_lock_t lDebugTrim;
		omp_lock_t lDebugPivot;
		
		int64_t callsFWBW;
		int64_t depthFWBW;
};