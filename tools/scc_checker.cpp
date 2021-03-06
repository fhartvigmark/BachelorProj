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
	int color;
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
	{
		cout << "Unable to open file\n";
	}

	vector<scc> sccSizes(0);

	for_each(sccMap.begin(), sccMap.end(),
			 [&sccSizes](const std::pair<int, int> &entry) {
				 scc current_scc = {entry.first, entry.second};
				 sccSizes.push_back(current_scc);
			 });

	std::sort(sccSizes.begin(), sccSizes.end(),
			  [](auto const &a, auto const &b) { return a.size > b.size; });

	if (!silent) 
	{
		cout << filename << " contains the following SCCs: \n";
	}

	if (!silent) 
	{
		for (int i = 0; i < sccSizes.size(); i++)
		{
			cout << "SCC color " << sccSizes.at(i).color << " has size " << sccSizes.at(i).size << "\n";
		}
	}
	
	if (!silent) 
	{
		cout << filename << " contains " << sccSizes.size() << " SCCs \n";
	}

	return sccSizes;
}

void generateOutput(string filename, vector<scc> list)
{
	ofstream outfile(filename);

	if (outfile.is_open())
	{
		for (int i = 0; i < list.size(); i++)
		{
			outfile << list.at(i).color << " " << list.at(i).size << "\n";
		}
		
		outfile.close();
	}
	else
	{
		cout << "Unable to open output file\n";
	}
}

int main(int argc, char **argv)
{
	bool generateOutputs = false;
	bool silent = false;
	int retVal = 0;
	string scc1FileName = "";
	string scc2FileName = "";

	if (argc == 3)
	{
		scc1FileName = argv[1];
		scc2FileName = argv[2];
	}
	else if (argc == 4)
	{
		scc1FileName = argv[1];
		scc2FileName = argv[2];
		string arg = argv[3];

		if (arg == "o") 
		{
			generateOutputs = true;
			silent = true;
		} 
		else if (arg == "s") 
		{
			silent = true;
		} 
		else 
		{
			cout << "Unkown third argument " << arg << "\n\n";
			cout << "Usage: ./scc_checker scc1File scc2File {OPTIONAL: o | s}  \n";
			cout << "o flag generates outputfiles \n";
			cout << "s suppress stdout listing of SCCs \n";
			return 1;
		}
		
	}
	else
	{
		cout << "Usage: ./scc_checker scc1File scc2File {OPTIONAL: o | s}  \n";
		cout << "o flag generates outputfiles \n";
		cout << "s suppress stdout listing of SCCs \n";
		return 1;
	}

	vector<scc> scclist1 = generateSCClist(scc1FileName, silent);
	vector<scc> scclist2 = generateSCClist(scc2FileName, silent);

	if(generateOutputs)
	{
		generateOutput("outfile1.out", scclist1);
		generateOutput("outfile2.out", scclist2);
	}

	int upperLimit = min(scclist1.size(), scclist2.size());

	if(scclist1.size()!=scclist2.size())
	{
		cout << "SCCs have different sizes " << scclist1.size() << " vs. " << scclist2.size() << "\n";
		retVal = -1;
	}

	for (int i = 0; i < upperLimit; i++)
	{
		if(scclist1.at(i).size!=scclist2.at(i).size)
		{
			cout << "Size of SCC " << i << " doesn't match " << scclist1.at(i).size << " vs. " << scclist2.at(i).size << "\n";
			retVal -1;
		}
	}

	if(retVal == 0)
	{
		cout << "SCCs seems to match \n";
	}
	else
	{
		cout << "SCCs appear to be different \n";
		cout << "Files " << scc1FileName << ", " << scc2FileName << "\n";
	}
	

	return retVal;
}
