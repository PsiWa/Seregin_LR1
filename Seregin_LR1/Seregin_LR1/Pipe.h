#pragma once
#include <string>
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

	Pipe(int id, int d, double l, bool repair) {
		Pipe_id = id;
		Pipe_diameter = d;
		Pipe_length = l;
		Is_under_repair = repair;
	}

	void set_Pipe_param() {
		int repair;
		bool is_correct;
		do
		{
			cin.clear();
			cin.ignore();
			cout << "Insert id\n";
			cin >> Pipe_id;
			cout << "Insert D\n";
			cin >> Pipe_diameter;
			cout << "Insert l\n";
			cin >> Pipe_length;
			cout << "Is pipe under repair?\n1.yes\n2.no\n(default - no)\n";
			cin >> repair;
			if (repair == 1) {
				Is_under_repair = true;
			}
			else Is_under_repair = false;
		} while (cin.fail() || !check_value(Pipe_id) || !check_value(Pipe_length) || !check_value(Pipe_diameter));

		cout << "Pipe created\n";
	}

	void print_Pipe_param() {
		cout << "Pipe id is " << Pipe_id << endl;
		cout << "Pipe diameter is " << Pipe_diameter << endl;
		cout << "Pipe length is " << Pipe_length << endl;
		cout << (Is_under_repair ? "!Pipe is under repair!" : "") << endl;
	}

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