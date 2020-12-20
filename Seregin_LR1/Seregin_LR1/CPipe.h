#pragma once
#include "Check_value.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <utility>
using namespace std;

class CPipe
{
private:
	static int MaxID;
	int Pipe_id;
	int Pipe_diameter;
	double Pipe_length;
	bool Is_under_repair;
	string Pipe_name;
	pair <int, int> ends = {-1,-1};

public:
	CPipe();
	CPipe(int id, int d, double l, bool repair, string name);
	void change_repair();
	void save_to_file(ofstream& fout);
	void load_from_file(ifstream& fin);
	void change_Pipe_param();
	friend ostream& operator<< (ostream& out, const CPipe& pipe);
	friend istream& operator>> (istream& in, CPipe& pipe);
	friend bool CheckPipeByRepairStatus(const CPipe& p, bool repair);
	friend bool CheckPipeByName(const CPipe& p, string str);
	int get_id();
	static void setMaxID(int i);
	static int getMaxID();
	void Set_ends(int s, int f);
	pair <int, int> Get_ends();
	int Get_weight();
	int Get_productivity();
};

