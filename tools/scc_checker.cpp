#include <string>
#include <iostream>
#include <map>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
using namespace std;

vector<int> generateSCClist(string filename)
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

	vector<int> sccSizes(0);
	
	for_each(sccMap.begin(), sccMap.end(),
			 [&sccSizes](const std::pair<int, int> &entry) {
				 sccSizes.push_back(entry.second);
			 });

	sort(sccSizes.begin(), sccSizes.end());
	reverse(sccSizes.begin(), sccSizes.end());
	
	cout << filename << " contains " << sccSizes.size() << " SCCs \n";
	for (int i = 0; i < sccSizes.size(); i++)
	{
		cout << "SCC " << i << " has size " << sccSizes.at(i) << "\n";
	}
	
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

	vector<int> scclist1 = generateSCClist(scc1FileName);
	vector<int> scclist2 = generateSCClist(scc2FileName);

	if(scclist1.size()!=scclist2.size()){
		cout << "SCCs have different sizes";
		retVal = -1;
	}

	for (int i = 0; i < scclist1.size(); i++)
	{
		if(scclist1.at(i)!=scclist2.at(i)){
			cout << "Size of SCC " << i << " doesn't match " << scclist1.at(i) << " vs. " << scclist2.at(i) << "\n";
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