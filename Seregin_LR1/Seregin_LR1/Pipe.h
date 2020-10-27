#pragma once
#include "Check_value.h"

using namespace std;

class Pipe
{
private:

	int Pipe_id;
	int Pipe_diameter;
	double Pipe_length;
	bool Is_under_repair;

public:

	Pipe() {
		Pipe_id = -1;
		Pipe_diameter = 1;
		Pipe_length = 1;
		Is_under_repair = false;
	}

	Pipe(int id, int d, double l, bool repair) {
		Pipe_id = id;
		Pipe_diameter = d;
		Pipe_length = l;
		Is_under_repair = repair;
	}

	void set_Pipe_param() {
		int repair;
		cout << "Insert id\n";
		Pipe_id = check_valuei();
		cout << "Insert D\n";
		Pipe_diameter = check_valuei();
		cout << "Insert l\n";
		Pipe_length = check_valued();
		cout << "Is pipe under repair?\n1.yes\n2.no\n(default - no)\n";
		repair = check_valuei();
		if (repair == 1) {
			Is_under_repair = true;
		}
		else Is_under_repair = false;

		cout << "Pipe created\n";
	}

	friend ostream& operator<< (ostream& out, const Pipe& pipe);

	void change_repair() {
		Is_under_repair = !Is_under_repair;
	}

	void save_to_file() {
		ofstream fout;
		fout.open("Pipes.txt", ios::out);
		if (fout.is_open()) {
			fout << Pipe_id << " " << Pipe_diameter << " " << Pipe_length << " " << Is_under_repair << endl;
			fout.close();
		}
		cout << "Saved";
	}

	void load_from_file() {
		ifstream fin;
		fin.open("Pipes.txt", ios::in);
		if (fin.is_open()) {
			string line;
			getline(fin, line);
			istringstream iss(line);
			iss >> Pipe_id >> Pipe_diameter >> Pipe_length >> Is_under_repair;
			fin.close();
			cout << "Loaded\n";
		}
		else cout << "Not found\n";
	}

};