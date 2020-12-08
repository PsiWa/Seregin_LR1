#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <iterator>
#include "Check_value.h"
#include "CCS.h"
#include "CPipe.h"
#include <filesystem> // https://stackoverflow.com/questions/612097/how-can-i-get-the-list-of-files-in-a-directory-using-c-or-c

using namespace std::filesystem;


ostream& operator<< (ostream& out, const CPipe& pipe) {
	out << "Pipe id is " << pipe.Pipe_id << endl  << "Pipe name is " << pipe.Pipe_name <<endl << "Pipe diameter is " << pipe.Pipe_diameter << endl;
	out << "Pipe length is " << pipe.Pipe_length << endl << (pipe.Is_under_repair ? "!Pipe is under repair!" : "Pipe is not under repair") << endl;
	return out;
}

ostream& operator<< (ostream& out, const CCS& comp) {
	out << "Compressor station id is " << comp.Comp_id << endl << "Compressor station name is " << comp.Comp_name <<endl << "There are " << comp.Comp_number << " workshops" << endl;
	out << comp.Comp_inwork << " of them are operating\n" << "Efficiency is " << comp.Comp_efficiency << "%\n";
	return out;
}

void PrintMenu() {
	cout << "////////////////////////////////////////\n"
		<< "1) Load from file\n"
		<< "2) Create Pipe\n"
		<< "3) Create Compressor station\n"
		<< "4) Change pipe status\n"
		<< "5) Print info\n"
		<< "6) Save  to file\n"
		<< "7) Change number of working compressors\n"
		<< "8) Find by filter\n"
		<< "9) Edit\n"
		<< "10) Delete\n"
		<< "11) Edit network\n"
		<< "12) Topological sort\n"
		<< "0) Exit\n"
		<< "////////////////////////////////////////\n";
}

template <class C>
C& SelectOne(unordered_map<int,C>& c) {
	cout << "Choose element id\n";
	int i;
	auto iter = c.begin();
	{
		while (1)
		{
			if ((iter = c.find(i = check_valuei())) != c.end())
				return iter->second;
			else
			{
				cout << "Element was not found" << endl;
				break;
			}
		}
	}
}

template <typename T>
using PipeFilter = bool(*)(const CPipe& p, T param);
template <typename T>
vector<int> FindPipeByFilter(const unordered_map<int,CPipe>& p, PipeFilter<T> f, T param)
{
	vector<int> result;
	int i = 0;
	for (auto& key : p) {
		if (f(key.second,param))
		{
			result.push_back(key.first);
		}
		i++;
	}
	return result;
}

template <typename T>
using CSFilter = bool(*)(const CCS& p, T param);
template <typename T>
vector<int> FindCSByFilter(const unordered_map<int,CCS>& c, CSFilter<T> f, T param)
{
	vector<int> result;
	int i = 0;
	for (auto& key : c) {
		if (f(key.second, param))
		{
			result.push_back(key.first);
		}
		i++;
	}
	return result;
}

bool CheckPipeByRepairStatus(const CPipe& p, bool repair)
{
	return p.Is_under_repair == repair;
}
bool CheckPipeByName(const CPipe& p, string str)
{
	return p.Pipe_name.find(str)!=string::npos; //https://www.cplusplus.com/reference/string/string/find/
}

bool CheckCSByName(const CCS& comp, string str)
{
	return comp.Comp_name.find(str) != string::npos; 
}
bool CheckCSByWorkshops(const CCS& comp, double percent) 
{
	return (comp.Comp_inwork * 1.0 / comp.Comp_number * 1.0) >= percent/100;
}

int main() {
	int size;
	CPipe pipe;
	CCS comp;
	ofstream fout;
	ifstream fin;
	string line;
	int i,j;
	double d;
	vector<int> found;

	unordered_map<int, CPipe> PipeMap;
	unordered_map<int, CCS> CSMap;

	unordered_map<int, unordered_map<int, int>> network;
	unordered_map<int, unordered_map<int, int>>::iterator netiterator;

	vector<unordered_map<int, unordered_map<int, int>>::iterator> todelete;

	bool IsDeadEnd;

	int maxsize = 0;

	int last;

	while (1) {
		PrintMenu();
		switch (InBetween(0, 12))
		{
		case 1: //LOAD
			cout << "Choose savefile" << endl;
			for (const auto& entry : directory_iterator("Saves"))
			{
				std::cout << entry.path().stem() << endl;
			}
			cin >> line;
			fin.open("Saves/"+line+".txt", ios::in);
			if (fin.is_open()) {
				getline(fin, line);
				size = stoi(line);
				getline(fin, line);
				CPipe::MaxID = stoi(line);
				while (size--) {
					pipe.load_from_file(fin);
					PipeMap[pipe.get_id()] = pipe;
				}
				getline(fin, line);
				size = stoi(line);
				getline(fin, line);
				CCS::MaxID = stoi(line);
				while (size--) {
					comp.load_from_file(fin);
					CSMap[comp.get_id()] = comp;
				}
				fin.close();
				cout << "Loaded" << endl;
			}
			else cout << "failed" << endl;

			break;

		case 2: // ADD PIPE
			pipe.set_Pipe_param();
			PipeMap[pipe.get_id()] = pipe;
			//pipes.push_back(pipe);
			break;

		case 3: // ADD CS
			comp.set_Compressor_param();
			CSMap[comp.get_id()] = comp;
			break;

		case 4: // REPAIR
			SelectOne(PipeMap).change_repair();
			cout << "changed" << endl;
			break;

		case 5: //PRINT
			cout << "1) Print pipe info\n2) Print CS info" << endl;
			switch (check_valuei()) {
			case 1:
				cout << "Pipe info:\n\n";
				i = 1;
				for (auto& key : PipeMap) {
					cout << key.first << ")\n" << key.second << endl;
					i++;
				}
				break;
			case 2:
				cout << "CS info:\n\n";
				i = 1;
				for (auto& key : CSMap) {
					cout << key.first << ")\n" << key.second << endl;
					i++;
				}
				break;
			default:
				cout << "Choose from existing commands" << endl;
				break;
			}
			break;

		case 6: //SAVE
			cout << "Choose savefile name" << endl;
			cin >> line;
			fout.open("Saves/"+line+".txt", ios::out);
			if (fout.is_open()) {
				fout << PipeMap.size() << endl << CPipe::MaxID << endl;
				for (auto& key : PipeMap) {
					key.second.save_to_file(fout);
				}
				fout << CSMap.size() << endl << CCS::MaxID << endl;
				for (auto& key : CSMap) {
					key.second.save_to_file(fout);
				}
				fout.close();
				cout << "Saved" << endl;
			}

			break;

		case 7: // WORKSHOPS
			cout << "1) Turn on workshop\n2) Turn off workshop" << endl;
			switch (check_valuei()) {
			case 1:
				SelectOne(CSMap).add_compressor();
				break;
			case 2:
				SelectOne(CSMap).subs_compressor();
				break;
			default:
				cout << "Choose from existing commands" << endl;
				break;
			}
			break;

		case 8: // FIND
			cout << "Find:\n1)Pipes\n2)Compressor Stations" << endl;
			switch (check_valuei())
			{
			case 1:
				cout << "Choose filter:\n1)By name\n2)By repair status" << endl;
				switch (check_valuei())
				{
				case 1:
					cout << "Enter name token:" << endl;
					cin >> line;
					found = FindPipeByFilter<string>(PipeMap, CheckPipeByName, line);
					for (auto& i : found)
					{
						cout << i << ")\n" << PipeMap[i] << endl;
					}
					cout << "A subset of the found elements was saved" << endl;
					break;
				case 2:
					cout << "Choose status\n1)Not under repair\n2)Under repair" << endl;
					cin >> i;
					found = FindPipeByFilter(PipeMap, CheckPipeByRepairStatus, (bool)(i - 1));
					for (auto& i : found)
					{
						cout << i << ")\n" << PipeMap[i] << endl;
					}
					cout << "A subset of the found elements was saved" << endl;
					break;
				default:
					cout << "Choose from existing filters" << endl;
					break;
				}
				break;

			case 2:
				cout << "Choose filter:\n1)By name\n2)By percent of active workshops" << endl;
				switch (check_valuei())
				{
				case 1:
					cout << "Enter name token:" << endl;
					cin >> line;
					found = FindCSByFilter(CSMap, CheckCSByName, line);
					for (auto& i : found)
					{
						cout << i << ")\n" << CSMap[i] << endl;
					}
					cout << "A subset of the found elements was saved" << endl;
					break;
				case 2:
					cout << "Enter minimal percent of active workshops:" << endl;
					found = FindCSByFilter(CSMap, CheckCSByWorkshops, InBetween(0.0, 100.0));
					for (auto& i : found)
					{
						cout << i << ")\n" << CSMap[i] << endl;
					}
					cout << "A subset of the found elements was saved" << endl;
					break;
				default:
					cout << "Choose from existing filters" << endl;
					break;
				}
				break;

			default:
				cout << "Choose from existing commands" << endl;
				break;
			}
			break;

		case 9: // EDIT
			cout << "What do you want to edit? \n1)Pipes\n2)Compressor Stations" << endl;
			switch (check_valuei())
			{
			case 1:
				cout << "Do you want to edit elements from current subset?\n1)Yes\n2)No" << endl;
				switch (check_valuei())
				{
				case 1:
					for (auto& i : found)
					{
						cout << i << ")\n" << PipeMap[i] << endl;
						PipeMap[i].change_Pipe_param();
						cout << i << ") New:\n" << PipeMap[i] << endl;
					}
					break;
				case 2:
					cout << "Write the numbers of elements you want to edit. Type 0 to stop\n";
					while (PipeMap.find(i=check_valuei())!=PipeMap.end())
					{
						cout << i << ")\n" << PipeMap[i] << endl;
						PipeMap[i].change_Pipe_param();
						cout << i << ") New:\n" << PipeMap[i] << endl;
					}
					break;
				default:
					cout << "Choose from existing commands" << endl;
					break;
				}
				break;

			case 2:
				cout << "Do you want to edit elements from current subset?\n1)Yes\n2)No" << endl;
				switch (check_valuei())
				{
				case 1:
					for (auto& i : found)
					{
						cout << i + 1 << ")\n" << CSMap[i] << endl;
						CSMap[i].change_cs_param();
						cout << i + 1 << ") New:\n" << CSMap[i] << endl;
					}
					break;
				case 2:
					cout << "Write the numbers of elements you want to edit. Type 0 to stop" << endl;
					found.clear();
					while (CSMap.find(i = check_valuei()) != CSMap.end())
					{
						cout << i << ")\n" << CSMap[i] << endl;
						CSMap[i].change_cs_param();
						cout << i << ") New:\n" << CSMap[i] << endl; 
					}

				default:
					cout << "Choose from existing commands" << endl;
					break;
				}
				
				break;

			default:
				cout << "Choose from existing commands" << endl;
				break;
			}
			break;

		case 10: // DELETE
			cout << "What do you want to edit? \n1)Pipes\n2)Compressor Stations" << endl;
			switch (check_valuei())
			{
			case 1:
				cout << "Type id of element you want to delete (Type '-1' to finish)" << endl;
				while (PipeMap.find(i = InBetween(-1,INT_MAX)) != PipeMap.end())
				{
					PipeMap.erase(PipeMap.find(i));
					cout << "Deleted" << endl;
				}
				break;
			case 2:
				cout << "Type id of element you want to delete (Type '-1' to finish)" << endl;
				while (CSMap.find(i = InBetween(-1, INT_MAX)) != CSMap.end())
				{
					CSMap.erase(CSMap.find(i));
				}
				break;
			default:
				break;
			}
			break;

		case 11: //EDIT NETWORK
			cout << "Create path using pipe:\n(ids of pipes: ";
			for  (auto iter = PipeMap.begin(); iter != PipeMap.end();iter++)
				cout << iter->first << " ";
			cout << ")" << endl;
			if (PipeMap.find(i = InBetween(0, INT_MAX)) != PipeMap.end())
			{
				cout << "From:\n(ids of CS: ";
				for (auto iter = CSMap.begin(); iter != CSMap.end(); iter++)
					cout << iter->first << " ";
				cout << ")" << endl;
				if (CSMap.find(j = InBetween(0, INT_MAX)) != CSMap.end())
				{
					network[i][j] = -1;
					if (j > maxsize)
						maxsize = j;
				}
				cout << "to:\n(ids of CS: ";
				for (auto iter = CSMap.begin(); iter != CSMap.end(); iter++)
					cout << iter->first << " ";
				cout << ")" << endl;
				if (CSMap.find(j = InBetween(0, INT_MAX)) != CSMap.end())
				{
					network[i][j] = 1;
					if (j > maxsize)
						maxsize = j;
				}
			}

			/*for (auto iter1 = network.begin(); iter1 != network.end(); iter1++) {
				cout << "l" << iter1->first << "| ";
				for (auto iter2 = iter1->second.begin(); iter2 != iter1->second.end(); iter2++)
					cout << "|v" << iter2->first << "|" << iter2->second << " ";
				cout << endl;
			}*/
			/*maxsize = 0;
			for (auto iter1 = network.begin(); iter1 != network.end(); iter1++) {
				if (maxsize < iter1->second.size())
					maxsize = iter1->second.size();
			}
			cout << maxsize << endl;*/

			for (auto iter = network.begin(); iter != network.end(); iter++) {
				for (int j = 1; j <= maxsize; j++)
					if (iter->second[j] != 1 && iter->second[j] !=-1)
						iter->second[j] = 0;
			}

			for (auto iter = network.begin(); iter != network.end(); iter++) {
				for (int j = 1; j <= maxsize; j++)
					cout << iter->second[j] << " ";
				cout << endl;
			}


			break;


		case 12:
			found.clear();
			while (network.size()>0)
			{
				for (int j = 1; j <= maxsize; j++) {
					IsDeadEnd = true;
					for (auto iter = network.begin(); iter != network.end(); iter++)
					{
						if (iter->second[j] == -1)
						{
							IsDeadEnd = false;
							if (network.size() == 1)
								last = j;
						}
					}
					if (IsDeadEnd)
					{
						netiterator = network.begin();
						for (auto iter = network.begin(); iter != network.end(); iter++)
						{
							if (iter->second[j] == 1)
							{
								found.push_back(j);
								todelete.push_back(iter);
							}
						}
						
					}
				}
				for (auto& i : todelete)
					network.erase(i);
			}
			found.push_back(last);
			for (auto& i : found)
			{
				cout << i << endl;
			}
			break;

		case 0: // EXIT
			return 0;
			break;

		default:
			cout << "Choose from existing commands" << endl;
			break;
		}
		cout << "_____________________________________________" << endl;
	}
}