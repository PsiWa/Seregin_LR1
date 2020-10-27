#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "CompressorStation.h"
#include "Pipe.h"

using namespace std;

ostream& operator<< (ostream& out, const Pipe& pipe) {
	out << "Pipe id is " << pipe.Pipe_id << endl;
	out << "Pipe diameter is " << pipe.Pipe_diameter << endl;
	out << "Pipe length is " << pipe.Pipe_length << endl;
	out << (pipe.Is_under_repair ? "!Pipe is under repair!" : "") << endl;
	return out;
}

ostream& operator<< (ostream& out, const CompressorStation& comp) {
	out << "Compressor station id is " << comp.Comp_id << endl;
	out << "Compressor station name is " << comp.Comp_name << endl;
	out << "There are " << comp.Comp_number << " compressors\n";
	out << comp.Comp_inwork << " of them are operating\n";
	out << "Efficiency is " << comp.Comp_efficiency << "%\n";
	return out;
}

void PrintMenu() {
	cout << "1) Load Pipe from file\n";
	cout << "2) Load Compressor station from file\n";
	cout << "3) Create Pipe\n";
	cout << "4) Create Compressor station\n";
	cout << "5) Change current pipe status\n";
	cout << "6) Print info\n";
	cout << "7) Save  to file\n";
	cout << "8) Change number of working compressors\n";
	cout << "0) Exit\n";
}

int main() {
	Pipe pipe;
	CompressorStation comp;
	int i;
	while (1) {
		PrintMenu();
		cin >> i;
		switch (i)
		{
		case 1:
			pipe.load_from_file();
			break;

		case 2:
			comp.load_from_file();
			break;

		case 3:
			pipe.set_Pipe_param();
			break;
		case 4:
			comp.set_Compressor_param();
			break;

		case 5:
			pipe.change_repair();
			break;

		case 6:
			cout << "1) Print pipe info\n";
			cout << "2) Print compressor station info\n";
			cin >> i;
			switch (i) {
			case 1:
				cout << pipe;
				break;
			case 2:
				cout << comp;
				break;
			default:
				cout << "Choose from existing commands\n";
				break;
			}
			break;

		case 7:
			cout << "1) Save pipe\n";
			cout << "2) Save compressor station\n";
			cin >> i;
			switch (i) {
			case 1:
				pipe.save_to_file();
				break;
			case 2:
				comp.save_to_file();
				break;
			default:
				cout << "Choose from existing commands\n";
				break;
			}
			break;

		case 8:
			cout << "1) Turn on compressor\n";
			cout << "2) Turn off compressor\n";
			cin >> i;
			switch (i) {
			case 1:
				comp.add_compressor();
				break;
			case 2:
				comp.subs_compressor();
				break;
			default:
				cout << "Choose from existing commands\n";
				break;
			}
			break;

		case 0:
			return 0;

		default:
			cout << "Choose from existing commands\n";
			break;
		}
		cout << endl << endl;
	}
}