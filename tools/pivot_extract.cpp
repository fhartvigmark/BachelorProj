#include <string>
#include <iostream>
#include <map>
#include <fstream>
#include <iterator>
#include <sstream>
#include <algorithm>
#include <vector>
using namespace std;

struct scc
{
	int depth;
	int size;
};

vector<scc> generateSCClist(string filename, bool silent)
{
	string line;
	ifstream sccFile(filename);
	map<int, int> sccMap;

	if (sccFile.is_open())
	{
		while (getline(sccFile, line))
		{
			string sccString;
			stringstream ss(line);

			ss >> sccString;
			ss >> sccString;

			int sccID = stoi(sccString);
			sccMap[sccID]++;
		}
		sccFile.close();
	}
	else
		cout << "Unable to open file\n";
	
	vector<scc> sccSizes(0);

	for_each(sccMap.begin(), sccMap.end(),
			 [&sccSizes](const std::pair<int, int> &entry) {
				 scc current_scc = {entry.first, entry.second};
				 sccSizes.push_back(current_scc);
			 });

	std::sort(sccSizes.begin(), sccSizes.end(),
			  [](auto const &a, auto const &b) { return a.size > b.size; });

	if (!silent) {
		cout << filename << " contains the following SCCs: \n";
	}

	if (!silent) {
		for (int i = 0; i < sccSizes.size(); i++)
		{
			cout << "SCC color " << sccSizes.at(i).depth << " has size " << sccSizes.at(i).size << "\n";
		}
	}
	
	if (!silent) {
		cout << filename << " contains " << sccSizes.size() << " SCCs \n";
	}
	return sccSizes;
}

void generateOutput(string filename, vector<scc> list){
	ofstream outfile(filename);

	if (outfile.is_open())
	{
		for (int i = 0; i < list.size(); i++)
		{
			outfile << list.at(i).depth << " " << list.at(i).size << "\n";
		}
		
		outfile.close();
	}
	else
		cout << "Unable to open output file\n";
}

int main(int argc, char **argv)
{
	bool generateOutputs = false;
	bool silent = false;
	int retVal = 0;
	string filename = "";
	string outname = "";
	if (argc == 3)
	{
		filename = argv[1];
		outname = argv[2];
	}
	else
	{
		cout << "Usage: ./scc_checker debugFile outFile \n";
		return 1;
	}

	string line;
	ifstream infile(filename);
	getline(infile, line);
	//cout << "1 " << line << "\n";
	getline(infile, line);
	//cout << "2 " << line << "\n";

	vector<scc> lines(0);

	while (getline(infile, line)) {
		int depth;
		int size;
		istringstream iss(line);

		iss >> depth;
		iss >> size;

		lines.push_back({depth, size});
	}

	std::sort(lines.begin(), lines.end(),
			  [](auto const &a, auto const &b) { return a.depth < b.depth; });

	int size = lines.size();
	ofstream outfile(outname);

	if (outfile.is_open())
	{
		for (int i = 0; i < min(size, 100); i++)
		{
			outfile << lines.at(i).size << "\n";
		}
		
		outfile.close();
	}
	else {
		cout << "Unable to open output file\n";
	}

	return retVal;
}