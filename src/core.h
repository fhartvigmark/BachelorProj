#pragma once
#include "Snap.h"
#include <atomic>
#include <chrono>

enum ePivot{Random, Max, MaxColor, ParRandom, ParMax, ParMaxColor, RandWalk};
enum eMethod{FWBW, ParFWBW, RecFWBW};
enum eTrim{Trim1, Trim2, Trim3, ParTrim1, ParTrim2, ParTrim3};
enum eTimer{MAIN, FirstFWBW, FWBWs, TRIM, PIVOT, SETUP};

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

    	PNGraph graph;
		TIntH *colors;
		TIntV *NIds;
    	ColorGenerator *colorGen;

		//TODO: add timers to functions
		//TODO: test
		TimePoint startTimer();
		void endTimer(TimePoint start, eTimer timer);
		int64_t getTime(eTimer timer);

		//TODO: add deconstructor??
		enhancedgraph(PNGraph g, bool timer, int randwalk_iterations);
		enhancedgraph();
	private:
        Duration tMain;
        Duration tFirstFWBW;
        Duration tFWBW;
        Duration tTrim;
        Duration tPivot;
        Duration tSetup;

		omp_lock_t lMain;
        omp_lock_t lFirstFWBW;
        omp_lock_t lFWBW;
        omp_lock_t lTrim;
        omp_lock_t lPivot;
        omp_lock_t lSetup;
};