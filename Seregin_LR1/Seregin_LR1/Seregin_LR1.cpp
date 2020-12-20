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
#include "CNetwork.h"
#include <filesystem> // https://stackoverflow.com/questions/612097/how-can-i-get-the-list-of-files-in-a-directory-using-c-or-c

using namespace std::filesystem;


ostream& operator<< (ostream& out, const CPipe& pipe) {
	out << "Pipe id is " << pipe.Pipe_id << endl  << "Pipe name is " << pipe.Pipe_name <<endl << "Pipe diameter is " << pipe.Pipe_diameter << endl;
	out << "Pipe length is " << pipe.Pipe_length << endl << (pipe.Is_under_repair ? "!Pipe is under repair!" : "Pipe is not under repair") << endl;
	return out;
}
istream& operator>> (istream& in, CPipe& pipe)
{
	int repair;
	string str;
	//cout << "Insert id\n";
	pipe.Pipe_id = CPipe::MaxID++;
	cout << "Insert Name\n";
	cin.ignore();
	getline(cin, str);
	if (str == "") pipe.Pipe_name = "nameless";
	else pipe.Pipe_name = str;
	cout << "Insert D\n";
	pipe.Pipe_diameter = check_valuei();
	cout << "Insert l\n";
	pipe.Pipe_length = check_valued();
	cout << "Is pipe under repair?\n1.yes\n2.no\n(default - no)\n";
	repair = check_valuei();
	if (repair == 1) {
		pipe.Is_under_repair = true;
	}
	else pipe.Is_under_repair = false;
	cout << "Pipe created\n";
	return in;
}

ostream& operator<< (ostream& out, const CCS& comp) {
	out << "Compressor station id is " << comp.Comp_id << endl << "Compressor station name is " << comp.Comp_name <<endl << "There are " << comp.Comp_number << " workshops" << endl;
	out << comp.Comp_inwork << " of them are operating\n" << "Efficiency is " << comp.Comp_efficiency << "%\n";
	return out;
}
istream& operator>> (istream& in, CCS& comp)
{
	string str;
	comp.Comp_id = CCS::MaxID++;
	cout << "Insert Name\n";
	cin.ignore();
	getline(cin, str);
	if (str == "") comp.Comp_name = "nameless";
	else comp.Comp_name = str;
	cout << "Insert number of workshops\n";
	comp.Comp_number = check_valuei();
	do {
		cout << "Insert number of active workshops\n";
		comp.Comp_inwork = check_valuei();
	} while (comp.Comp_inwork > comp.Comp_number);
	do {
		cout << "Insert efficiency\n";
		comp.Comp_efficiency = check_valuei();
	} while (comp.Comp_efficiency > 100);
	cout << "Compressor station created\n";
	return in;
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
		<< "////////////////////////////////////////\n"
		<< "11) Edit network\n"
		<< "12) Topological sort\n"
		<< "13) Print Incidence matrix\n"
		<< "14) Delete arcs/peaks\n"
		<< "15) Find Min Path\n"
		<< "16) Find Max Flow\n"
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
vector<int> FindPipeByFilter(unordered_map<int,CPipe> p, PipeFilter<T> f, T param)
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
	CNetwork network;

	while (1) {
		PrintMenu();
		switch (InBetween(0, 16))
		{
		case 1: // LOAD
		{
			string line;
			ifstream fin;
			cout << "Choose savefile" << endl;
			for (const auto& entry : directory_iterator("Saves"))
			{
				std::cout << entry.path().stem() << endl;
			}
			cin >> line;
			network.Load_from_file(line);
			break;
		}

		case 2: // ADD PIPE
		{
			network.Add_to_PipeMap();
			break;
		}

		case 3: // ADD CS
		{
			network.Add_to_CSMap();
			break;
		}

		case 4: // REPAIR
		{
			network.Select_pipe().change_repair();
			cout << "changed" << endl;
			break;
		}

		case 5: // PRINT
		{
			cout << "1) Print pipe info\n2) Print CS info" << endl;
			switch (check_valuei())
			{
			case 1: // PRINT PIPE INFO
			{
				network.Print_PipeMap();
				break;
			}
			case 2: //PRINT CS INFO
			{
				network.Print_CSMap();
				break;
			}
			default:
			{
				cout << "Choose from existing commands" << endl;
				break;
			}
			}
			break;
		}

		case 6: // SAVE
		{
			ofstream fout;
			string line;
			cout << "Choose savefile name" << endl;
			cin >> line;
			network.Save_to_file(line);
			break;
		}

		case 7: // WORKSHOPS
		{
			cout << "1) Turn on workshop\n2) Turn off workshop" << endl;
			switch (check_valuei()) {
			case 1: // TURN ON
			{
				network.Select_CS().add_compressor();
				break;
			}
			case 2: // TURN OFF
			{
				network.Select_CS().subs_compressor();
				break;
			}
			default:
			{
				cout << "Choose from existing commands" << endl;
				break;
			}
			}
			break;
		}

		case 8: // FIND
		{
			cout << "Find:\n1)Pipes\n2)Compressor Stations" << endl;
			switch (check_valuei())
			{
			case 1: // FIND PIPES
			{
				cout << "Choose filter:\n1)By name\n2)By repair status" << endl;
				switch (check_valuei())
				{
				case 1: // BY NAME
				{
					string line;
					cout << "Enter name token:" << endl;
					cin >> line;
					network.Set_subset_of_pipes(FindPipeByFilter<string>(network.Get_PipeMap(), CheckPipeByName, line));
					network.Print_subset_of_pipes();
					cout << "A subset of the found elements was saved" << endl;
					break;
				}
				case 2: // BY REPAIR STATUS
				{
					int i;
					cout << "Choose status\n1)Not under repair\n2)Under repair" << endl;
					i = InBetween(1, 2);
					network.Set_subset_of_pipes(FindPipeByFilter(network.Get_PipeMap(), CheckPipeByRepairStatus, (bool)(i - 1)));
					network.Print_subset_of_pipes();
					cout << "A subset of the found elements was saved" << endl;
					break;
				}
				default:
				{
					cout << "Choose from existing filters" << endl;
					break;
				}
				}
				break;
			}

			case 2: // FIND CS
			{
				cout << "Choose filter:\n1)By name\n2)By percent of active workshops" << endl;
				switch (check_valuei())
				{
				case 1: // BY NAME
				{
					string line;
					cout << "Enter name token:" << endl;
					cin >> line;
					network.Set_subset_of_cs(FindCSByFilter(network.Get_CSMap(), CheckCSByName, line));
					network.Print_subset_of_CS();
					cout << "A subset of the found elements was saved" << endl;
					break;
				}
				case 2: // BY ACTIVE WORKSHOPS
				{
					cout << "Enter minimal percent of active workshops:" << endl;
					network.Set_subset_of_cs(FindCSByFilter(network.Get_CSMap(), CheckCSByWorkshops, InBetween(0.0, 100.0)));
					network.Print_subset_of_CS();
					cout << "A subset of the found elements was saved" << endl;
					break;
				}
				default:
				{
					cout << "Choose from existing filters" << endl;
					break;
				}
				break;
				}
				break;
			}

			default:
			{
				cout << "Choose from existing commands" << endl;
				break;
			}
			}
			break;
		}

		case 9: // EDIT
		{
			cout << "What do you want to edit? \n1)Pipes\n2)Compressor Stations" << endl;
			switch (check_valuei())
			{
			case 1: // EDIT PIPES
			{
				cout << "Do you want to edit elements from current subset?\n1)Yes\n2)No" << endl;
				switch (check_valuei())
				{
				case 1: // FROM SUBSET
				{
					network.Edit_subset_of_pipes();
					break;
				}
				case 2: // BY PICK
				{
					cout << "Write the numbers of element you want to edit. Type -1 to stop\n";
					network.PipeMap_edit();
					break;
				}
				default:
				{
					cout << "Choose from existing commands" << endl;
					break;
				}
				}
				break;
			}
			case 2: // EDIT CS
			{
				cout << "Do you want to edit elements from current subset?\n1)Yes\n2)No" << endl;
				switch (check_valuei())
				{
				case 1: // FROM SUBSET
				{
					network.Edit_subset_of_cs(); 
					break;
				}
				case 2: // BY PICK
				{
					cout << "Write the numbers of elements you want to edit. Type -1 to stop" << endl;
					network.CSMap_edit();
					break;
				}

				default:
				{
					cout << "Choose from existing commands" << endl;
					break;
				}
				}
				break;
			}
			default:
			{
				cout << "Choose from existing commands" << endl;
				break;
			}
			}
			break;
		}

		case 10:// DELETE
		{
			cout << "What do you want to delete? \n1)Pipes\n2)Compressor Stations" << endl;
			switch (check_valuei())
			{
			case 1: // DELETE PIPES
			{
				cout << "Type id of elements you want to delete (Type '-1' to finish)" << endl;
				network.Pipe_erase();
				break;
			}
			case 2: // DELETE CS
			{
				cout << "Type id of elements you want to delete (Type '-1' to finish)" << endl;
				network.CS_erase();
				break;
			}
			default:
			{
				break;
			}
			}
			break;
		}

		case 11:// EDIT NETWORK
		{
			network.AddPath();
			network.PrintIncidence();

			break;
		}

		case 12:// TOPOLOGICAL SORT
		{
			for (auto& i : network.TopologicalSort())
				cout << i.first << ")" << i.second << " ";
			cout << endl;
			break;
		}

		case 13:// PRINT GRAPH
		{
			network.PrintIncidence();
			cout << endl;
			network.PrintAdjacency();
			break;
		}
		case 14: // DELETE PEAKS/ARCS
			cout << "What do you want to delete?\n1)Arcs\n2)Peaks\n";
			switch (InBetween(-1,2))
			{
			case 1:
			{
				cout << "Type id of elements you want to delete (Type '-1' to finish)" << endl;
				network.Delete_arc();
				break;
			}
			case 2:
			{
				cout << "Type id of elements you want to delete (Type '-1' to finish)" << endl;
				network.Delete_peak();
				break;
			}
			default:
				break;
			}
			break;

		case 15:
		{
			cout << "Enter start point:\n";
			int start = network.Selecr_peak();
			cout << "Enter finish point:\n";
			int finish = network.Selecr_peak();
			network.MinPath(start, finish);
			break;
		}
		case 16:
		{
			cout << "Enter start point:\n";
			int start = network.Selecr_peak();
			cout << "Enter finish point:\n";
			int finish = network.Selecr_peak();
			cout << network.Max_flow(start, finish) << endl;
			break;
		}
		case 0: // EXIT
		{
			return 0;
			break;
		}

		default:
		{
			cout << "Choose from existing commands" << endl;
			break;
		}
		}
		cout << "_____________________________________________" << endl;
	}
}