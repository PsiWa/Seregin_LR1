#pragma once
#include <string>
#include "Check_value.h"

using namespace std;

class CompressorStation {
private:
	int Comp_id;
	string Comp_name;
	int Comp_number;
	int Comp_inwork;
	double Comp_efficiency;
public:
	CompressorStation(int id, string name, int number, int inwork, double eff) {
		Comp_id = id;
		Comp_name = name;
		Comp_number = number;
		Comp_inwork = inwork;
		Comp_efficiency = eff;
	}

	void set_Compressor_param() {
		do
		{
			cin.clear();
			cin.ignore();
			cout << "Insert id\n";
			cin >> Comp_id;
			cout << "Insert Name\n";
			cin >> Comp_name;
			cout << "Insert number of compressors\n";
			cin >> Comp_number;
		} while (cin.fail() || !check_value(Comp_id) || !check_value(Comp_number));
		do {
			cout << "Insert number of working compressors\n";
			cin >> Comp_inwork;
		} while (Comp_inwork > Comp_number);
		do {
			cout << "Insert efficiency\n";
			cin >> Comp_efficiency;
		} while (!check_value(Comp_efficiency) || Comp_efficiency > 100);
		cout << "Compressor created\n";
	}

	void print_Compressor_param() {
		cout << "Compressor station id is " << Comp_id << endl;
		cout << "Compressor station name is " << Comp_name << endl;
		cout << "There are " << Comp_number << " compressors\n";
		cout << Comp_inwork << " of them are operating\n";
		cout << "Efficiency is " << Comp_efficiency << "%\n";
	}

	void add_compressor() {
		if (Comp_inwork + 1 <= Comp_number) Comp_inwork++;
		else cout << "All compressors are working\n";
	}

	void subs_compressor() {
		if (Comp_inwork - 1 >= 0) Comp_inwork--;
		else cout << "There are no working compressors \n";
	}

	void save_to_file() {
		ofstream fout;
		fout.open("Compressor stations.txt", ios::out);
		if (fout.is_open()) {
			fout << Comp_id << " " << Comp_name << " " << Comp_number << " " << Comp_inwork << " " << Comp_efficiency << endl;
			fout.close();
		}
		cout << "Saved";
	}

	void load_from_file() {
		ifstream fin;
		fin.open("Compressor stations.txt", ios::in);
		if (fin.is_open()) {
			string line;
			getline(fin, line);
			istringstream iss(line);
			iss >> Comp_id >> Comp_name >> Comp_number >> Comp_inwork >> Comp_efficiency;
			fin.close();
			cout << "Loaded\n";
		}
		else cout << "Not found\n";
	}

};