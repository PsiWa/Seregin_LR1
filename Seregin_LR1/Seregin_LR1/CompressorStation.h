#pragma once
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

	CompressorStation() {
		Comp_id = -1;
		Comp_name = "default";
		Comp_number = 0;
		Comp_inwork = 0;
		Comp_efficiency = 0;
	}

	CompressorStation(int id, string name, int number, int inwork, double eff) {
		Comp_id = id;
		Comp_name = name;
		Comp_number = number;
		Comp_inwork = inwork;
		Comp_efficiency = eff;
	}

	void set_Compressor_param() {
		
		cout << "Insert id\n";
		Comp_id = check_valuei();
		cout << "Insert Name\n";
		cin >> Comp_name;
		cout << "Insert number of compressors\n";
		Comp_number = check_valuei();
		do {
			cout << "Insert number of working compressors\n";
			Comp_inwork = check_valuei();
		} while (Comp_inwork > Comp_number);
		do {
			cout << "Insert efficiency\n";
			Comp_efficiency = check_valuei();
		} while (Comp_efficiency > 100);
		cout << "Compressor created\n";
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

	friend ostream& operator<< (ostream& out, const CompressorStation& comp);
};