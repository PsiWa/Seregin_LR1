#include "CNetwork.h"

void CNetwork::Add_to_PipeMap()
{
	CPipe pipe;
	cin >> pipe;
	PipeMap[pipe.get_id()] = pipe;
}

void CNetwork::Add_to_CSMap()
{
	CCS comp;
	cin >> comp;
	CSMap[comp.get_id()] = comp;
}

void CNetwork::Load_from_file(string filename)
{
	ifstream fin;
	fin.open("Saves/" + filename + ".txt", ios::in);
	if (fin.is_open()) {
		string line;
		CPipe pipe;
		CCS comp;
		int size;
		getline(fin, line);
		size = stoi(line);
		getline(fin, line);
		CPipe::setMaxID(stoi(line));
		while (size--) {
			pipe.load_from_file(fin);
			PipeMap[pipe.get_id()] = pipe;
			if (pipe.Get_ends().first != -1 && pipe.Get_ends().second != -1)
			{
				idofarcs.push_back(pipe.get_id());
				if (idofpeaks.end() == find(idofpeaks.begin(), idofpeaks.end(), pipe.Get_ends().first))
					idofpeaks.push_back(pipe.Get_ends().first);
				if (idofpeaks.end() == find(idofpeaks.begin(), idofpeaks.end(), pipe.Get_ends().second))
					idofpeaks.push_back(pipe.Get_ends().second);
			}
		}
		getline(fin, line);
		size = stoi(line);
		getline(fin, line);
		CCS::setMaxID(stoi(line));
		while (size--) {
			comp.load_from_file(fin);
			CSMap[comp.get_id()] = comp;
		}
		fin.close();
		cout << "Loaded" << endl;
	}
	else cout << "failed" << endl;
}

void CNetwork::Save_to_file(string filename)
{
	ofstream fout;
	fout.open("Saves/" + filename + ".txt", ios::out);
	if (fout.is_open()) {
		fout << PipeMap.size() << endl << CPipe::getMaxID() << endl;
		for (auto& key : PipeMap) {
			key.second.save_to_file(fout);
		}
		fout << CSMap.size() << endl << CCS::getMaxID() << endl;
		for (auto& key : CSMap) {
			key.second.save_to_file(fout);
		}
		fout.close();
		cout << "Saved" << endl;
	}
}

CPipe& CNetwork::Select_pipe()
{
	if (PipeMap.size() > 0)
	{
		auto iter = PipeMap.begin();
		while (1)
		{
			if ((iter = PipeMap.find(check_valuei())) != PipeMap.end())
				return iter->second;
			else
			{
				cout << "Element was not found" << endl;
				break;
			}
		}
	}
	else
	{
		cout << "Empty" << endl;
	}
}


CCS& CNetwork::Select_CS()
{
	if (CSMap.size() > 0)
	{
		auto iter = CSMap.begin();
		{
			while (1)
			{
				if ((iter = CSMap.find(check_valuei())) != CSMap.end())
					return iter->second;
				else
				{
					cout << "Element was not found" << endl;
					break;
				}
			}
		}
	}
	else
		cout << "Empty" << endl;
}

void CNetwork::Pipe_erase()
{
	int i;
	while (PipeMap.find(i = InBetween(-1, INT_MAX)) != PipeMap.end())
	{
		idofarcs.erase(find(idofarcs.begin(), idofarcs.end(), i));
		PipeMap.erase(PipeMap.find(i));
		cout << "Deleted" << endl;
	}
}

void CNetwork::CS_erase()
{
	int i;
	while (CSMap.find(i = InBetween(-1, INT_MAX)) != CSMap.end())
	{
		list<int> arcstodelete;
		CSMap.erase(CSMap.find(i));
		idofpeaks.erase(find(idofpeaks.begin(), idofpeaks.end(), i));
		for (auto& j : idofarcs)
		{
			if (PipeMap[j].Get_ends().first == i || PipeMap[j].Get_ends().second == i)
				arcstodelete.push_back(j);
		}
		for (auto& j: arcstodelete)
			idofarcs.erase(find(idofarcs.begin(), idofarcs.end(), j));
		cout << "Deleted" << endl;
	}
}

void CNetwork::PipeMap_edit()
{
	int i;
	while (PipeMap.find(i = InBetween(-1, INT_MAX)) != PipeMap.end())
	{
		PipeMap[i].change_Pipe_param();
		cout << "Write the numbers of element you want to edit. Type -1 to stop\n";
	}
}

void CNetwork::CSMap_edit()
{
	int i;
	while (CSMap.find(i = InBetween(-1, INT_MAX)) != CSMap.end())
	{
		CSMap[i].change_cs_param();
		cout << "Write the numbers of element you want to edit. Type -1 to stop\n";
	}
}

void CNetwork::Print_PipeMap()
{
	cout << "Pipe info:\n\n";
	for (auto& key : PipeMap) {
		cout << key.first << ")\n" << key.second << endl;
	}
}

void CNetwork::Print_CSMap()
{
	cout << "CS info:\n\n";
	for (auto& key : CSMap) {
		cout << key.first << ")\n" << key.second << endl;
	}
}

void CNetwork::Delete_peak()
{
	int i;
	while (find(idofpeaks.begin(), idofpeaks.end(), i = InBetween(-1, INT_MAX)) != idofpeaks.end())
	{
		list<int> arcstodelete;
		idofpeaks.erase(find(idofpeaks.begin(), idofpeaks.end(), i));
		for (auto& j : idofarcs)
		{
			if (PipeMap[j].Get_ends().first == i || PipeMap[j].Get_ends().second == i)
			{
				PipeMap[j].Set_ends(-1, -1);
				arcstodelete.push_back(j);
			}
		}
		for (auto& j : arcstodelete)
			idofarcs.erase(find(idofarcs.begin(), idofarcs.end(), j));
		cout << "Deleted" << endl;
	}
}

void CNetwork::Delete_arc()
{
	int i;
	while (find(idofarcs.begin(), idofarcs.end(), i = InBetween(-1, INT_MAX)) != idofarcs.end())
	{
		idofarcs.erase(find(idofarcs.begin(), idofarcs.end(), i));
		PipeMap[i].Set_ends(-1, -1);
		cout << "Deleted" << endl;
	}
}

void CNetwork::Edit_subset_of_pipes()
{
	for (auto& i : subset_of_pipes)
	{
		cout << i << ")\n" << PipeMap[i] << endl;
		PipeMap[i].change_Pipe_param();
		cout << i << ") New:\n" << PipeMap[i] << endl;
	}
}

void CNetwork::Edit_subset_of_cs()
{
	for (auto& i : subset_of_cs)
	{
		cout << i + 1 << ")\n" << CSMap[i] << endl;
		CSMap[i].change_cs_param();
		cout << i + 1 << ") New:\n" << CSMap[i] << endl;
	}
}

void CNetwork::Set_subset_of_pipes(vector<int> v)
{
	subset_of_pipes.clear();
	subset_of_pipes = v;
}

void CNetwork::Set_subset_of_cs(vector<int> v)
{
	subset_of_cs.clear();
	subset_of_cs = v;
}

void CNetwork::Print_subset_of_pipes()
{
	for (auto& i : subset_of_pipes)
		cout << PipeMap[i] << endl;
}

void CNetwork::Print_subset_of_CS()
{
	for (auto& i : subset_of_cs)
		cout << CSMap[i] << endl;
}

unordered_map<int, CPipe> CNetwork::Get_PipeMap()
{
	unordered_map<int, CPipe> result;
	return result = PipeMap;
}

unordered_map<int, CCS> CNetwork::Get_CSMap()
{
	unordered_map<int, CCS> result;
	return result = CSMap;
}

bool CNetwork::IsValid()
{
	return (PipeMap.size()>0 && CSMap.size()>0);
}


void CNetwork::AddPath()
{
	unsigned int i, js, jf;
	cout << "Create path using pipe:\n(ids of pipes: ";
	for (auto& p : PipeMap)
	{
		if (idofarcs.end() == find(idofarcs.begin(), idofarcs.end(), p.first))
			cout << p.first << " ";
	}
	cout << ")" << endl;
	if (PipeMap.find(i = InBetween(0, INT_MAX)) != PipeMap.end() && idofarcs.end() == find(idofarcs.begin(), idofarcs.end(), i))
	{
		cout << "From:\n(ids of CS: ";
		for (auto& c: CSMap)
			cout << c.first << " ";
		cout << ")" << endl;
		if (CSMap.find(js = InBetween(0, INT_MAX)) != CSMap.end())
		{
			cout << "to:\n(ids of CS: ";
			for (auto& c : CSMap)
				cout << c.first << " ";
			cout << ")" << endl;
			if (CSMap.find(jf = InBetween(0, INT_MAX)) != CSMap.end() && jf != js)
			{
				idofarcs.push_back(i);
				PipeMap[i].Set_ends(js, jf);
				if (idofpeaks.end() == find(idofpeaks.begin(), idofpeaks.end(), js))
					idofpeaks.push_back(js);
				if (idofpeaks.end() == find(idofpeaks.begin(), idofpeaks.end(), jf))
					idofpeaks.push_back(jf);
			}
			else cout << "You can't make a loop" << endl;
		}
		else cout << "There is no such CS" << endl;
	}
	else cout << "You cant use this pipe!\nIt isn't created or already used" << endl;
}

unordered_map<int, unordered_map<int, int>> CNetwork::Make_Incidence()
{
	unordered_map<int, unordered_map<int, int>> Incidence;
	for (auto& i : idofarcs)
	{
		Incidence[i][PipeMap[i].Get_ends().first] = -1;
		Incidence[i][PipeMap[i].Get_ends().second] = 1;
	}
	return Incidence;
}

void CNetwork::PrintIncidence()
{
	unordered_map<int, unordered_map<int, int>> Incidence = this->Make_Incidence();
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

unordered_map<int, unordered_map<int, int>> CNetwork::Make_Adjacency()
{
	unordered_map<int, unordered_map<int, int>> Adjacency;
	for (auto& i : idofarcs)
	{
		Adjacency[PipeMap[i].Get_ends().first][PipeMap[i].Get_ends().second] = 1;
	}
	return Adjacency;
}

void CNetwork::PrintAdjacency()
{
	unordered_map<int, unordered_map<int, int>> Adjacency = this->Make_Adjacency();
	for (auto& i : idofpeaks)
	{
		cout << "v" << i << "| ";
		for (auto& j : idofpeaks)
			cout << "v" << j << "|" << Adjacency[i][j] << " ";
		cout << endl;
	}
}

bool CNetwork::Cycle_check(int i)
{
	unordered_map<int, bool> visited;
	for (auto& i : idofpeaks)
		visited[i] = false;
	unordered_map<int, unordered_map<int, int>> Adjacency = this->Make_Adjacency();
	for (auto& i : idofpeaks)
	{
		visited[i] = true;
		for (auto& j : idofpeaks)
			if (Adjacency[i][j] == 1)
				if (visited[j] == true)
					return true;
	}
	return false;
}

unordered_map<int, int> CNetwork::TopologicalSort()
{
	for (auto& i : idofpeaks)
	{
		if (this->Cycle_check(i))
		{
			cout << "There is loop\n";
			return { { 0,0 } };
		}
	}
	unordered_map<int, unordered_map<int, int>> Incidence = this->Make_Incidence();
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
				break;
			}
		}
	}
	for (auto& i : copyofpeaks)
		sort[number--] = i;
	return sort;
}


