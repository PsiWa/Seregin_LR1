#pragma once
#include "Check_value.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

class CPipe
{
private:

	int Pipe_id;
	int Pipe_diameter;
	double Pipe_length;
	bool Is_under_repair;
	string Pipe_name;

public:
	static int MaxID;
	CPipe();
	CPipe(int id, int d, double l, bool repair, string name);
	void change_repair();
	void save_to_file(ofstream& fout);
	void load_from_file(ifstream& fin);
	void set_Pipe_param();
	void change_Pipe_param();
	friend ostream& operator<< (ostream& out, const CPipe& pipe);
	friend bool CheckPipeByRepairStatus(const CPipe& p, bool repair);
	friend bool CheckPipeByName(const CPipe& p, string str);
	int get_id();
};

