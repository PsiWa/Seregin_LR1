#include "CNetwork.h"

void CNetwork::AddPath(const unordered_map<int, CPipe>& pipes,const unordered_map<int, CCS>& CS)
{
	unsigned int i, j;
	cout << "Create path using pipe:\n(ids of pipes: ";
	for (auto& p : pipes)
	{
		if (idofarcs.end() == find(idofarcs.begin(), idofarcs.end(), p.first))
			cout << p.first << " ";
	}
	cout << ")" << endl;
	if (pipes.find(i = InBetween(0, INT_MAX)) != pipes.end() && idofarcs.end() == find(idofarcs.begin(), idofarcs.end(), i))
	{
		idofarcs.push_back(i);
		cout << "From:\n(ids of CS: ";
		for (auto& c: CS)
			cout << c.first << " ";
		cout << ")" << endl;
		if (CS.find(j = InBetween(0, INT_MAX)) != CS.end())
		{
			if (idofpeaks.end() == find(idofpeaks.begin(), idofpeaks.end(), j))
				idofpeaks.push_back(j);
			Incidence[i][j] = -1;
		}
		cout << "to:\n(ids of CS: ";
		for (auto& c : CS)
			cout << c.first << " ";
		cout << ")" << endl;
		if (CS.find(j = InBetween(0, INT_MAX)) != CS.end())
		{
			if (idofpeaks.end() == find(idofpeaks.begin(), idofpeaks.end(), j))
				idofpeaks.push_back(j);
			Incidence[i][j] = 1;
		}
	}
	else cout << "You cant use this pipe!\nIt isn't created or already used" << endl;
}

void CNetwork::PrintIncidence()
{
	for (auto& i : idofarcs) {
		cout << "l" << i << "| ";
		for (auto& j : idofpeaks)
		{
			cout << "|v" << j<< "|"
			<< Incidence[i][j] << " ";
		}
		cout << endl;
	}
}
