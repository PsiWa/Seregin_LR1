﻿#include <iostream>
#include <string>
#include <fstream>

using namespace std;

template <typename T>
bool check_value(T x) {
	return x > 0 ? true : false;
}


class Pipe
{
private:

	int Pipe_id;
	double Pipe_diameter;
	double Pipe_length;
	bool Is_under_repair;

public:

	Pipe(int id, double d, double l, bool repair) {
		Pipe_id = id;
		Pipe_diameter = d;
		Pipe_length = l;
		Is_under_repair = repair;
	}

	void set_Pipe_param() {
		int repair;
		bool is_correct;
		while (!check_value(Pipe_id) || !check_value(Pipe_length) || !check_value(Pipe_diameter))
		{
			cout << "Insert id\n";
			cin >> Pipe_id;
			cout << "Insert D\n";
			cin >> Pipe_diameter;
			cout << "Insert l\n";
			cin >> Pipe_length;
		}
		
		do {
			cout << "Is pipe under repair?\n1.yes\n2.no\n";
			cin >> repair;
			switch (repair)
			{
			case 1:
				Is_under_repair = true;
				is_correct = true;
				break;
			case 2:
				Is_under_repair = false;
				is_correct = true;
				break;
			default:
				Is_under_repair = false;
				is_correct = false;
				break;
			}
		} while (is_correct == false);
		cout << "Pipe created\n";
	}

	void get_Pipe_param() {
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
		fout.open("Pipe №" + to_string(Pipe_id) + ".txt",ios::out);
		if (fout.is_open()) {
			fout << Pipe_id << endl << Pipe_diameter << endl << Pipe_length << endl << Is_under_repair << endl;
			fout.close();
		}
		cout << "Saved";
	}

	void load_from_file(int id) {
		ifstream fin;
		fin.open("Pipe №" + to_string(id) + ".txt", ios::in);
		if (fin.is_open()) {
			fin >> Pipe_id;
			fin >> Pipe_diameter;
			fin >> Pipe_length;
			fin >> Is_under_repair;
			fin.close();
		}
		cout << "Loaded";
	}
	
	~Pipe() {}
};

int main() {
	Pipe pipe(0,0,0,false);
	//pipe.set_Pipe_param();
	pipe.load_from_file(1);
	pipe.change_repair();
	pipe.get_Pipe_param();
	//pipe.save_to_file();
	return 0;
}