#include "Snap.h"
#include <random>
#include <string>
#include <iostream>
#include <fstream>
#include <time.h>
#include <stdio.h>
#include <queue>
#include <deque>
using namespace std;

int myRand(const int low, const int high) {
    static thread_local std::mt19937 randGenerator(static_cast<std::mt19937::result_type>(time(nullptr)));

    std::uniform_int_distribution<int> distribution(low,high);
    return distribution(randGenerator);
}

int size;
int scc1 = 0;
int scc2 = 0;
int scc3 = 0;
int nextNode = 0;

int t1 = 0;
int t2 = 0;
PNGraph graph;

int generateNext();

int generate1(const int prevNode, bool inEdge) {
	nextNode++;
	graph->AddNode(nextNode);
	if (inEdge) {
		graph->AddEdge(prevNode, nextNode);
	} else {
		graph->AddEdge(nextNode, prevNode);
	}

	return nextNode;
}

int generate2(const int prevNode, bool inEdge) {
	int i = ++nextNode;
	nextNode++;
	graph->AddNode(i);
	graph->AddNode(nextNode);
	if (inEdge) {
		graph->AddEdge(prevNode, nextNode);
	} else {
		graph->AddEdge(nextNode, prevNode);
	}

	graph->AddEdge(nextNode, i);
	graph->AddEdge(i, nextNode);

	return nextNode;
}

int generate3(const int prevNode, bool inEdge) {
	int choice = myRand(1, 2);
	int i = ++nextNode;
	int j = ++nextNode;
	nextNode++;
	graph->AddNode(i);
	graph->AddNode(j);
	graph->AddNode(nextNode);
	if (inEdge) {
		graph->AddEdge(prevNode, nextNode);
	} else {
		graph->AddEdge(nextNode, prevNode);
	}
	
	if (choice == 1) {
		graph->AddEdge(i, j);
		graph->AddEdge(j, nextNode);
		graph->AddEdge(nextNode, i);
		t1++;
	} else {
		graph->AddEdge(i, nextNode);
		graph->AddEdge(nextNode, i);
		graph->AddEdge(j, nextNode);
		graph->AddEdge(nextNode, j);
		t2++;
	}

	return nextNode;
}

int generateNext() {
	int choice = myRand(1, 3);

	if (choice == 1 && scc1 < size) {
		scc1++;
		return 1;
	}
	else if (choice == 2 && scc2 < size) {
		scc2++;
		return 2;
	} else if (choice == 3 && scc3 < size) {
		scc3++;
		return 3;
	}

	if (scc1 < size) {
		scc1++;
		return 1;
	}
	else if (scc2 < size) {
		scc2++;
		return 2;
	} else if (scc3 < size) {
		scc3++;
		return 3;
	}
	
	return 0;
}

struct gen
{
	int prevNode;
	bool inEdge;
};

int main(int argc, char **argv)
{
	int retVal = 0;
	string filename = "";
	if (argc == 3)
	{
		filename = argv[1];
		size = stoi(argv[2]);
	}
	else
	{
		cout << "Usage: ./trim_graph_creator outfile size \n";
		return 1;
	}

	std::deque<gen> q;
	q.push_back({0, true});
	graph = TNGraph::New();
	graph->AddNode(0);
	scc1++;
	int next = generateNext();

	while (!q.empty()) {
		gen toGen = q.front();
		q.pop_front();
		int newNode;

		if (next == 1) {
			newNode = generate1(toGen.prevNode, toGen.inEdge);
		} else if (next == 2) {
			newNode = generate2(toGen.prevNode, toGen.inEdge);
		} else if (next == 3) {
			newNode = generate3(toGen.prevNode, toGen.inEdge);
		}

		next = generateNext();
		if (next > 0) {
			int count = myRand(1, 3);
			for (int v = 0; v < count; v++) {
				q.push_back({newNode, !toGen.inEdge});
			}
		}
	}
	

	ofstream outfile(filename);

	if (outfile.is_open())
	{
		for (PNGraph::TObj::TEdgeI EI = graph->BegEI(); EI < graph->EndEI(); EI++)
		{
			outfile << EI.GetSrcNId() << " " << EI.GetDstNId() << "\n";
		}
		
		outfile.close();
	}
	else {
		cout << "Unable to open output file\n";
	}

	cout << t1 << "-" << t2 << "\n";
	
	return retVal;
}