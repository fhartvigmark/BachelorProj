#include <string>
#include <iostream>
#include <map>
#include <fstream>
#include <iterator>
#include <sstream>
using namespace std;

int main(int argc, char **argv)
{
	int retVal = 0;
	string inputFileName = "";
	string outputFileName = "";

	if (argc == 3)
	{
		inputFileName = argv[1];
		outputFileName = argv[2];
	}
	else
	{
		cout << "Usage: ./graph_converter graph outfile \n";
		return 1;
	}

	ofstream outfile;
	outfile.open(outputFileName);

	if (!outfile.is_open()) 
	{
		cout << "Unable to open output file\n";
		return 1;
	}

	string line;
	ifstream infile(inputFileName);
	map<int, int> nodes;
	int counter = 0;

	if (!infile.is_open()) 
	{
		cout << "Unable to open input file\n";
		return 1;
	}

	while (getline(infile, line)) 
	{
		if (line.at(0) == '#') 
		{
			//outfile << line;
			continue;
		}

		int from;
		int to;
		istringstream iss(line);

		iss >> from;
		iss >> to;

		if (nodes.count(from) == 0) 
		{
			nodes[from] = counter;
			counter++;
		}

		if (nodes.count(to) == 0) 
		{
			nodes[to] = counter;
			counter++;
		}

		outfile << nodes.at(from) << " " << nodes.at(to) << "\n";

	}

	infile.close();
	outfile.close();

	return retVal;
}
