#include "CCS.h"

int CCS::MaxID = 1;

CCS::CCS()
{
	Comp_id = -1;
	Comp_name = "default";
	Comp_number = 0;
	Comp_inwork = 0;
	Comp_efficiency = 0;
}

CCS::CCS(int id, string name, int number, int inwork, double eff)
{
	Comp_id = id;
	Comp_name = name;
	Comp_number = number;
	Comp_inwork = inwork;
	Comp_efficiency = eff;
}


void CCS::add_compressor()
{
	if (Comp_inwork + 1 <= Comp_number) Comp_inwork++;
	else cout << "All workshops are operating\n";
}

void CCS::subs_compressor()
{
	if (Comp_inwork - 1 >= 0) Comp_inwork--;
	else cout << "There are no operating workshops \n";
}

void CCS::save_to_file(ofstream& fout)
{
	fout << Comp_name << "\n" << Comp_id << " " << Comp_number << " " << Comp_inwork << " " << Comp_efficiency << endl;
}

void CCS::load_from_file(ifstream& fin)
{
	string line;
	getline(fin, line);
	Comp_name = line;
	getline(fin, line);
	istringstream iss(line);
	iss >> Comp_id >> Comp_number >> Comp_inwork >> Comp_efficiency;
}

void CCS::change_cs_param()
{
	string str;
	cout << "Insert new name\n";
	cin.ignore();
	getline(cin, str);
	if (str == "") cout << "Name was not changed\n";
	else Comp_name = str;
	cout << "Insert number of workshops\n";
	Comp_number = check_valuei();
	do {
		cout << "Insert number of active workshops\n";
		Comp_inwork = check_valuei();
	} while (Comp_inwork > Comp_number);
	do {
		cout << "Insert efficiency\n";
		Comp_efficiency = check_valuei();
	} while (Comp_efficiency > 100);
}

int CCS::get_id()
{
	return Comp_id;
}

void CCS::setMaxID(int i)
{
	CCS::MaxID = i;
}

int CCS::getMaxID()
{
	return CCS::MaxID;
}

