#pragma once
#include <iostream>
#include <list>
#include <unordered_map>
#include "CPipe.h"
#include "CCS.h"
#include "Check_value.h"
#include <iterator>
using namespace std;

class CNetwork
{
private:
	list<int> idofpeaks;
	list<int> idofarcs;
	vector<int> subset_of_pipes;
	vector<int> subset_of_cs;
	unordered_map<int, CPipe> PipeMap;
	unordered_map<int, CCS> CSMap;

public:
	void Add_to_PipeMap();
	void Add_to_CSMap();

	void Load_from_file(string filename);
	void Save_to_file(string filename);

	CPipe& Select_pipe();
	CCS& Select_CS();

	void Pipe_erase();
	void CS_erase();

	void PipeMap_edit();
	void CSMap_edit();

	void Print_PipeMap();
	void Print_CSMap();

	void Delete_peak();
	void Delete_arc();

	void Edit_subset_of_pipes();
	void Edit_subset_of_cs();
	void Set_subset_of_pipes(vector<int> v);
	void Set_subset_of_cs(vector<int> v);

	void Print_subset_of_pipes();
	void Print_subset_of_CS();

	unordered_map<int, CPipe> Get_PipeMap();
	unordered_map<int, CCS> Get_CSMap();

	bool IsValid();

	void AddPath();
	unordered_map<int, unordered_map<int, int>> Make_Incidence();
	void PrintIncidence();
	unordered_map<int, unordered_map<int, int>> Make_Adjacency();
	void PrintAdjacency();
	bool Cycle_check(int i);
	unordered_map<int, int> TopologicalSort();
};