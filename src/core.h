#pragma once
#include "Snap.h"
#include <atomic>
#include <chrono>

enum ePivot{Random, Max, MaxColor, ParRandom, ParMax, ParMaxColor, RandWalk};
enum eMethod{FWBW, ParFWBW, RecFWBW};
enum eTrim{Trim1, Trim2, Trim3, ParTrim1, ParTrim2, ParTrim3};
enum eTimer{Main, FirstFWBW, FWBWs, TRIM, PIVOT, SETUP};

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

		//TODO: implement function
		//TODO: add code to main that outputs times
		//TODO: test
		void endTimer(std::chrono::high_resolution_clock::time_point start, eTimer timer);

		enhancedgraph(PNGraph g);
		enhancedgraph();
	private:
        std::atomic<long> tMain;
        std::atomic<long> tFirstFWBW;
        std::atomic<long> tFWBW;
        std::atomic<long> tTrim;
        std::atomic<long> tPivot;
        std::atomic<long> tSetup;
};