#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "Check_value.h"
#include "CCS.h"
#include "CPipe.h"
#include <filesystem> // https://stackoverflow.com/questions/612097/how-can-i-get-the-list-of-files-in-a-directory-using-c-or-c

using namespace std::filesystem;


ostream& operator<< (ostream& out, const CPipe& pipe) {
	out << "Pipe id is " << pipe.Pipe_id << endl;
	out << "Pipe name is " << pipe.Pipe_name << endl;
	out << "Pipe diameter is " << pipe.Pipe_diameter << endl;
	out << "Pipe length is " << pipe.Pipe_length << endl;
	out << (pipe.Is_under_repair ? "!Pipe is under repair!" : "Pipe is not under repair") << endl;
	return out;
}

ostream& operator<< (ostream& out, const CCS& comp) {
	out << "Compressor station id is " << comp.Comp_id << endl;
	out << "Compressor station name is " << comp.Comp_name << endl;
	out << "There are " << comp.Comp_number << " workshops\n";
	out << comp.Comp_inwork << " of them are operating\n";
	out << "Efficiency is " << comp.Comp_efficiency << "%\n";
	return out;
}

void PrintMenu() {
	cout << "////////////////////////////////////////\n";
	cout << "1) Load from file\n";	
	cout << "2) Create Pipe\n";
	cout << "3) Create Compressor station\n";
	cout << "4) Change current pipe status\n";
	cout << "5) Print info\n";
	cout << "6) Save  to file\n";
	cout << "7) Change number of working compressors\n";
	cout << "8) Find by filter\n";
	cout << "9) Edit\n";
	cout << "0) Exit\n";
	cout << "////////////////////////////////////////\n";
}

template <class C>
C& SelectOne(vector<C>& c) {
	cout << "Choose number of element\n";
	unsigned int index = InBetween(1u, c.size());
	return c[index - 1];
}


template <typename T>
using PipeFilter = bool(*)(const CPipe& p, T param);
template <typename T>
vector<int> FindPipeByFilter(const vector<CPipe>& p, PipeFilter<T> f, T param)
{
	vector<int> result;
	int i = 0;
	for (auto& pipe : p) {
		if (f(pipe,param))
		{
			result.push_back(i);
		}
		i++;
	}
	return result;
}
template <typename T>
using CSFilter = bool(*)(const CCS& p, T param);
template <typename T>
vector<int> FindCSByFilter(const vector<CCS>& c, CSFilter<T> f, T param)
{
	vector<int> result;
	int i = 0;
	for (auto& comp : c) {
		if (f(comp, param))
		{
			result.push_back(i);
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
	vector <CPipe> pipes;
	vector <CCS> CS;
	ofstream fout;
	ifstream fin;
	string line;
	int i;
	double d;
	vector<int> found;
	stringstream iss;
	while (1) {
		PrintMenu();
		i = InBetween(0,9);
		switch (i)
		{
		case 1: //LOAD
			cout << "Choose savefile\n";
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
					pipes.push_back(pipe);
				}
				getline(fin, line);
				size = stoi(line);
				getline(fin, line);
				CCS::MaxID = stoi(line);
				while (size--) {
					comp.load_from_file(fin);
					CS.push_back(comp);
				}
				fin.close();
				cout << "Loaded\n";
			}
			else cout << "failed\n";

			break;

		case 2: // ADD PIPE
			pipe.set_Pipe_param();
			pipes.push_back(pipe);
			break;

		case 3: // ADD CS
			comp.set_Compressor_param();
			CS.push_back(comp);
			break;

		case 4: // REPAIR
			SelectOne(pipes).change_repair();
			cout << "changed\n";
			break;

		case 5: //PRINT
			cout << "1) Print pipe info\n2) Print CS info\n";
			i = check_valuei();
			switch (i) {
			case 1:
				cout << "Pipe info:\n\n";
				i = 1;
				for (auto& pipe : pipes) {
					cout << i << ")\n";
					cout << pipe << endl;
					i++;
				}
				break;
			case 2:
				cout << "CS info:\n\n";
				i = 1;
				for (auto& comp : CS) {
					cout << i << ")\n";
					cout << comp << endl;
					i++;
				}
				break;
			default:
				cout << "Choose from existing commands\n";
				break;
			}
			break;

		case 6: //SAVE
			cout << "Choose savefile name\n";
			cin >> line;
			fout.open("Saves/"+line+".txt", ios::out);
			if (fout.is_open()) {
				fout << pipes.size() << endl;
				fout << CPipe::MaxID << endl;
				for (auto& pipe : pipes) {
					pipe.save_to_file(fout);
				}
				fout << CS.size() << endl;
				fout << CCS::MaxID << endl;
				for (auto& comp : CS) {
					comp.save_to_file(fout);
				}
				cout << "Saved\n";
				fout.close();
			}

			break;

		case 7: // WORKSHOPS
			cout << "1) Turn on workshop\n";
			cout << "2) Turn off workshop\n";
			i = check_valuei();
			switch (i) {
			case 1:
				SelectOne(CS).add_compressor();
				break;
			case 2:
				SelectOne(CS).subs_compressor();
				break;
			default:
				cout << "Choose from existing commands\n";
				break;
			}
			break;

		case 8: // FIND
			cout << "Find:\n1)Pipes\n2)Compressor Stations" << endl;
			i = check_valuei();
			switch (i)
			{
			case 1:
				cout << "Choose filter:\n1)By name\n2)By repair status" << endl;
				i = check_valuei();
				switch (i)
				{
				case 1:
					cout << "Enter name token:" << endl;
					cin >> line;
					found = FindPipeByFilter<string>(pipes, CheckPipeByName, line);
					for (auto& i : found)
					{
						cout << i+1 << ")\n";
						cout << pipes[i] << endl;
					}
					cout << "A subset of the found elements was saved" << endl;
					break;
				case 2:
					cout << "Choose status\n1)Not under repair\n2)Under repair" << endl;
					cin >> i;
					found = FindPipeByFilter(pipes, CheckPipeByRepairStatus, (bool)(i - 1));
					for (auto& i : found)
					{
						cout << i + 1 << ")\n";
						cout << pipes[i] << endl;
					}
					cout << "A subset of the found elements was saved" << endl;
					break;
				default:
					cout << "Choose from existing filters\n";
					break;
				}
				break;

			case 2:
				cout << "Choose filter:\n1)By name\n2)By percent of active workshops" << endl;
				i = check_valuei();
				switch (i)
				{
				case 1:
					cout << "Enter name token:" << endl;
					cin >> line;
					found = FindCSByFilter(CS, CheckCSByName, line);
					for (auto& i : found)
					{
						cout << i + 1 << ")\n";
						cout << CS[i] << endl;
					}
					cout << "A subset of the found elements was saved" << endl;
					break;
				case 2:
					cout << "Enter minimal percent of active workshops:" << endl;
					d = InBetween(0.0, 100.0);
					found = FindCSByFilter(CS, CheckCSByWorkshops, d);
					for (auto& i : found)
					{
						cout << i + 1 << ")\n";
						cout << CS[i] << endl;
					}
					cout << "A subset of the found elements was saved" << endl;
					break;
				default:
					cout << "Choose from existing filters\n";
					break;
				}
				break;

			default:
				cout << "Choose from existing commands\n";
				break;
			}
			break;

		case 9: // EDIT
			cout << "What do you want to edit? \n1)Pipes\n2)Compressor Stations\n";
			i = check_valuei();
			switch (i)
			{
			case 1:
				cout << "Do you want to edit elements from current subset?\n1)Yes\n2)No\n";
				i = check_valuei();
				switch (i)
				{
				case 1:
					for (auto& i : found)
					{
						cout << i + 1 << ")\n";
						cout << pipes[i] << endl;
						pipes[i].change_Pipe_param();
						cout << i + 1 << ") New:\n";
						cout << pipes[i] << endl;
					}
					break;
				case 2:
					cout << "Write the numbers of elements you want to edit. Type 0 to stop\n";
					found.clear();
					while ((i=InBetween<int>(0,(pipes.size()+1)))>0)
					{
						found.push_back(i-1);
					}
					for (auto& i : found)
					{
						cout << i + 1 << ")\n";
						cout << pipes[i] << endl;
						pipes[i].change_Pipe_param();
						cout << i + 1 << ") New:\n";
						cout << pipes[i] << endl;
					}
					break;
				default:
					cout << "Choose from existing commands\n";
					break;
				}
				break;

			case 2:
				cout << "Do you want to edit elements from current subset?\n1)Yes\n2)No\n";
				i = check_valuei();
				switch (i)
				{
				case 1:
					for (auto& i : found)
					{
						cout << i + 1 << ")\n";
						cout << CS[i] << endl;
						CS[i].change_cs_param();
						cout << i + 1 << ") New:\n";
						cout << CS[i] << endl;
					}
					break;
				case 2:
					cout << "Write the numbers of elements you want to edit. Type 0 to stop\n";
					found.clear();
					while ((i = InBetween<int>(0, (CS.size() + 1))) > 0)
					{
						found.push_back(i - 1);
					}
					for (auto& i : found)
					{
						cout << i + 1 << ")\n";
						cout << CS[i] << endl;
						CS[i].change_cs_param();
						cout << i + 1 << ") New:\n";
						cout << CS[i] << endl;
					}

				default:
					break;
				}
				
				break;

			default:
				cout << "Choose from existing commands\n";
				break;
			}
			break;

		case 0: // EXIT
			return 0;
			break;

		default:
			cout << "Choose from existing commands\n";
			break;
		}
		cout << "_____________________________________________\n";
	}
}