#pragma once
#include <iostream>
#include <list>
#include <unordered_map>
#include "CPipe.h"
#include "CCS.h"
#include "Check_value.h"
using namespace std;

class CNetwork
{
private:
	list<int> idofpeaks;
	list<int> idofarcs;
	unordered_map<int, unordered_map<int, int>> Incidence;

public:
	void AddPath(const unordered_map<int, CPipe>& pipes,const unordered_map<int, CCS>& CS);
	void PrintIncidence();

};

