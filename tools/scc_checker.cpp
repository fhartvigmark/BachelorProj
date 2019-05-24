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

vector<scc> generateSCClist(string filename)
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
		cout << "Unable to open file";
	
	vector<scc> sccSizes(0);

	for_each(sccMap.begin(), sccMap.end(),
			 [&sccSizes](const std::pair<int, int> &entry) {
				 scc current_scc = {entry.first, entry.second};
				 sccSizes.push_back(current_scc);
			 });

	std::sort(sccSizes.begin(), sccSizes.end(),
			  [](auto const &a, auto const &b) { return a.size > b.size; });

	cout << filename << " contains the following SCCs: \n";
	for (int i = 0; i < sccSizes.size(); i++)
	{
		cout << "SCC color " << sccSizes.at(i).color << " has size " << sccSizes.at(i).size << "\n";
	}
	
	
	cout << filename << " contains " << sccSizes.size() << " SCCs \n";
	return sccSizes;
}

int main(int argc, char **argv)
{
	int retVal = 0;
	string scc1FileName = "";
	string scc2FileName = "";
	if (argc == 3)
	{
		scc1FileName = argv[1];
		scc2FileName = argv[2];
	}
	else
	{
		cout << "Usage: ./scc_checker scc1File scc2File\n";
		return 1;
	}

	vector<scc> scclist1 = generateSCClist(scc1FileName);
	vector<scc> scclist2 = generateSCClist(scc2FileName);

	int upperLimit = min(scclist1.size(), scclist2.size());

	if(scclist1.size()!=scclist2.size()){
		cout << "SCCs have different sizes";
		retVal = -1;
	}


	for (int i = 0; i < upperLimit; i++)
	{
		if(scclist1.at(i).size!=scclist2.at(i).size){
			cout << "Size of SCC " << i << " doesn't match " << scclist1.at(i).size << " vs. " << scclist2.at(i).size << "\n";
			retVal -1;
		}
	}

	if(retVal == 0){
		cout << "SCCs seems to match \n";
	}else{
		cout << "SCCs appear to be different \n";
	}
	

	return retVal;
}