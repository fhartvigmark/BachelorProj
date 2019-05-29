#pragma once
#include "Snap.h"
#include <atomic>
#include <chrono>
#include <list>

enum ePivot{Random, Max, MaxColor, ParRandom, ParMax, ParMaxColor, RandWalk};
enum eMethod{FWBW, ParFWBW, RecFWBW};
enum eTrim{Trim1, Trim2, Trim3, ParTrim1, ParTrim2, ParTrim3};
enum eTimer{MAIN, FirstFWBW, FWBWs, TRIM, FirstTRIM, TRIM1, TRIM2, TRIM3, PIVOT, SETUP, PREP};
enum eDebug{dCOLOR, dNODE, dTRIM1, dTRIM2, dTRIM3, dBFS, dDEPTH, dFW, dBW};

typedef std::chrono::high_resolution_clock Time;
typedef std::chrono::high_resolution_clock::time_point TimePoint;
typedef std::chrono::high_resolution_clock::duration Duration;
typedef std::chrono::microseconds Us;
typedef std::chrono::milliseconds Ms;

class ColorMap
{
	public:
		void AddDat(int i, int data);
		int GetDat(int i);
		int Len();
		int BegI();
		int EndI();

		ColorMap(int size, bool hasZero);
		~ColorMap();
	private:
		int *array;
		int length;
		int beg;
		int end;
};

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
		const int TRIM_CUTOFF;
		const int TRIM_STEPS;

    	PNGraph graph;
		ColorMap *colors;
    	ColorGenerator *colorGen;
		ColorMap *degree;

		void calculateDegree();

		TimePoint startTimer();
		void endTimer(TimePoint start, eTimer timer);
		int64_t getTime(eTimer timer);

		void reportFWBW(int color, int node, int trim1, int trim2, int trim3, int bfs, int depth, int fw, int bw);

		int64_t getCallsFWBW();
		int64_t getCallsTrim();
		int64_t getCallsPivot();
		int64_t getDepth();
		std::list<int>* getReports(eDebug data);


		enhancedgraph(PNGraph g, bool timer, bool analyse, int randwalk_iterations, int cutoff, int steps);
		enhancedgraph();

		~enhancedgraph();
	private:
		std::list<int> *dColor;
		std::list<int> *dNode;
		std::list<int> *dTrim1;
		std::list<int> *dTrim2;
		std::list<int> *dTrim3;
		std::list<int> *dBFS;
		std::list<int> *dDepth;
		std::list<int> *dFW;
		std::list<int> *dBW;

		//Color
		//Pivot node
		//Trim-1 amount
		//Trim-2 amount
		//Trim-3 amount
		//BFS amount
		//FWBW depth
		//FW color
		//BW color

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
        Duration tPrep;

		omp_lock_t lMain;
        omp_lock_t lFirstFWBW;
        omp_lock_t lFWBW;
        omp_lock_t lTrim;
        omp_lock_t lFirstTrim;
        omp_lock_t lPivot;
        omp_lock_t lSetup;
		omp_lock_t lDebugFWBW;
		
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