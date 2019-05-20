#pragma once
#include "Snap.h"
#include <atomic>
#include <chrono>

enum ePivot{Random, Max, MaxColor, ParRandom, ParMax, ParMaxColor, RandWalk};
enum eMethod{FWBW, ParFWBW, RecFWBW};
enum eTrim{Trim1, Trim2, Trim3, ParTrim1, ParTrim2, ParTrim3};
enum eTimer{MAIN, FirstFWBW, FWBWs, TRIM, PIVOT, SETUP};

//typedef std::duration<unsigned long long> my_duration;

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

		//TODO: add code to main that outputs times
		//TODO: test
		void endTimer(std::chrono::high_resolution_clock::time_point start, eTimer timer);
		int64_t getTime(eTimer timer);

		//TODO: add deconstructor??
		enhancedgraph(PNGraph g);
		enhancedgraph();
	private:
        std::chrono::high_resolution_clock::duration tMain;
        std::chrono::high_resolution_clock::duration tFirstFWBW;
        std::chrono::high_resolution_clock::duration tFWBW;
        std::chrono::high_resolution_clock::duration tTrim;
        std::chrono::high_resolution_clock::duration tPivot;
        std::chrono::high_resolution_clock::duration tSetup;

		omp_lock_t lMain;
        omp_lock_t lFirstFWBW;
        omp_lock_t lFWBW;
        omp_lock_t lTrim;
        omp_lock_t lPivot;
        omp_lock_t lSetup;
};