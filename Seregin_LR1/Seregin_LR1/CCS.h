#pragma once
#include "Check_value.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

class CCS
{
private:
	static int MaxID;
	int Comp_id;
	string Comp_name;
	int Comp_number;
	int Comp_inwork;
	double Comp_efficiency;
public:
	CCS();
	CCS(int id, string name, int number, int inwork, double eff);
	void add_compressor();
	void subs_compressor();
	void save_to_file(ofstream& fout);
	void load_from_file(ifstream& fin);
	void change_cs_param();
	int get_id();
	static void setMaxID(int i);
	static int getMaxID();
	friend ostream& operator<< (ostream& out, const CCS& comp);
	friend istream& operator>> (istream& in, CCS& comp);
	friend bool CheckCSByName(const CCS& comp, string str);
	friend bool CheckCSByWorkshops(const CCS& comp, double percent);
};

