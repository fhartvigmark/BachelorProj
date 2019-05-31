#include "Snap.h"
#include <random>
#include <string>
#include <iostream>
#include <fstream>
#include <time.h>
#include <stdio.h>
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

void generateNext(PNGraph graph, const int prevNode, bool inEdge);

void generate1(PNGraph graph, const int prevNode, bool inEdge) {
	nextNode++;
	graph->AddNode(nextNode);
	if (inEdge) {
		graph->AddEdge(prevNode, nextNode);
	} else {
		graph->AddEdge(nextNode, prevNode);
	}

	int count = myRand(1, 10);
	for (int v = 0; v < count; v++) {
		generateNext(graph, nextNode, !inEdge);
	}
}

void generate2(PNGraph graph, const int prevNode, bool inEdge) {
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

	int count = myRand(1, 10);
	for (int v = 0; v < count; v++) {
		generateNext(graph, nextNode, !inEdge);
	}
}

void generate3(PNGraph graph, const int prevNode, bool inEdge) {
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

	int count = myRand(1, 10);
	for (int v = 0; v < count; v++) {
		generateNext(graph, nextNode, !inEdge);
	}
}

void generateNext(PNGraph graph, const int prevNode, bool inEdge) {
	int choice = myRand(1, 3);

	if (choice == 1 && scc1 < size) {
		scc1++;
		generate1(graph, prevNode, inEdge);
		return;
	}
	else if (choice == 2 && scc2 < size) {
		scc2++;
		generate2(graph, prevNode, inEdge);
		return;
	} else if (choice == 3 && scc3 < size) {
		scc3++;
		generate3(graph, prevNode, inEdge);
		return;
	}

	if (scc1 < size) {
		scc1++;
		generate1(graph, prevNode, inEdge);
		return;
	}
	else if (scc2 < size) {
		scc2++;
		generate2(graph, prevNode, inEdge);
		return;
	} else if (scc3 < size) {
		scc3++;
		generate3(graph, prevNode, inEdge);
		return;
	}
	
}


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

	PNGraph graph = TNGraph::New();
	graph->AddNode(0);
	scc1++;
	generateNext(graph, 0, true);

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