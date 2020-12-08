#include "CNetwork.h"

void CNetwork::AddPath(const unordered_map<int, CPipe>& pipes,const unordered_map<int, CCS>& CS)
{
	unsigned int i, js, jf;
	cout << "Create path using pipe:\n(ids of pipes: ";
	for (auto& p : pipes)
	{
		if (idofarcs.end() == find(idofarcs.begin(), idofarcs.end(), p.first))
			cout << p.first << " ";
	}
	cout << ")" << endl;
	if (pipes.find(i = InBetween(0, INT_MAX)) != pipes.end() && idofarcs.end() == find(idofarcs.begin(), idofarcs.end(), i))
	{
		cout << "From:\n(ids of CS: ";
		for (auto& c: CS)
			cout << c.first << " ";
		cout << ")" << endl;
		if (CS.find(js = InBetween(0, INT_MAX)) != CS.end())
		{
			cout << "to:\n(ids of CS: ";
			for (auto& c : CS)
				cout << c.first << " ";
			cout << ")" << endl;
			if (CS.find(jf = InBetween(0, INT_MAX)) != CS.end() && jf != js)
			{
				idofarcs.push_back(i);
				if (idofpeaks.end() == find(idofpeaks.begin(), idofpeaks.end(), js))
					idofpeaks.push_back(js);
				if (idofpeaks.end() == find(idofpeaks.begin(), idofpeaks.end(), jf))
					idofpeaks.push_back(jf);
				Incidence[i][js] = -1;
				Incidence[i][jf] = 1;
			}
			else cout << "You can't make a loop" << endl;
		}
		else cout << "There is no such CS" << endl;
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

unordered_map<int, int> CNetwork::TopologicalSort()
{
	bool IsDeadEnd;
	unordered_map<int, int> sort;
	int number = idofpeaks.size();
	list<int> copyofarcs = idofarcs;
	list<int> copyofpeaks = idofpeaks;
	list<int> arcstodelete;
	while (copyofarcs.size() > 0)
	{
		for (auto& j : copyofpeaks) {
			IsDeadEnd = true;
			arcstodelete.clear();
			for (auto& i : copyofarcs)
			{
				if (Incidence[i][j] == -1)
					IsDeadEnd = false;
			}
			if (IsDeadEnd)
			{
				sort[number--] = j;
				for (auto& i: copyofarcs)
				{
					if (Incidence[i][j] == 1)
						arcstodelete.push_back(i);
				}
				for (auto& i : arcstodelete)
					copyofarcs.erase(find(copyofarcs.begin(), copyofarcs.end(), i));
				copyofpeaks.erase(find(copyofpeaks.begin(), copyofpeaks.end(), j));
				for (auto& i : copyofpeaks)
					cout << i << " ";
				cout << endl;
				break;
			}
		}
	}
	for (auto& i : copyofpeaks)
		sort[number--] = i;
	return sort;
}
