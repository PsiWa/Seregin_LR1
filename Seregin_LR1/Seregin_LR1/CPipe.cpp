#include "CPipe.h"

int CPipe::MaxID = 1;

CPipe::CPipe()
{
	Pipe_id = -1;
	Pipe_diameter = 1;
	Pipe_length = 1;
	Is_under_repair = false;
	Pipe_name = "default";
}


CPipe::CPipe(int id, int d, double l, bool repair, string name)
{
	Pipe_id = id;
	Pipe_diameter = d;
	Pipe_length = l;
	Is_under_repair = repair;
	Pipe_name = name;
}

void CPipe::change_repair()
{
	Is_under_repair = !Is_under_repair;
}

void CPipe::save_to_file(ofstream& fout)
{
	fout <<Pipe_name << "\n" << Pipe_id << " " << Pipe_diameter << " " << Pipe_length << " " << Is_under_repair << " " << ends.first << " " << ends.second <<  endl;
}

void CPipe::load_from_file(ifstream& fin)
{
	string line;
	getline(fin, line);
	Pipe_name = line;
	getline(fin, line);
	istringstream iss(line);
	iss >> Pipe_id >> Pipe_diameter >> Pipe_length >> Is_under_repair >> ends.first >> ends.second;
}


void CPipe::change_Pipe_param()
{
	int repair;
	string str;
	cout << "Insert new name:\n";
	cin.ignore();
	getline(cin, str);
	if (str == "") cout <<"Name was not changed\n";
	else Pipe_name = str;
	cout << "Insert new D\n";
	Pipe_diameter = check_valuei();
	cout << "Insert new l\n";
	Pipe_length = check_valued();
	cout << "Is pipe under repair?\n1.yes\n2.no\n(default - no)\n";
	repair = check_valuei();
	if (repair == 1) {
		Is_under_repair = true;
	}
	else Is_under_repair = false;
}

int CPipe::get_id()
{
	return Pipe_id;
}

void CPipe::setMaxID(int i)
{
	CPipe::MaxID = i;
}

int CPipe::getMaxID()
{
	return CPipe::MaxID;
}

void CPipe::Set_ends(int s, int f)
{
	ends.first = s;
	ends.second = f;
}

pair<int, int> CPipe::Get_ends()
{
	return ends;
}

int CPipe::Get_weight()
{
	int i;
	if (Is_under_repair)
		return INT_MAX;
	else
		return i = round(1/(sqrt(pow(Pipe_diameter/1000.0, 5)/ Pipe_length)));
}

int CPipe::Get_productivity()
{
	int d;
	if (Is_under_repair)
		return 0;
	else
		return d = round((sqrt(pow(Pipe_diameter / 1000.0, 5) / Pipe_length))*pow(10,5));
}
