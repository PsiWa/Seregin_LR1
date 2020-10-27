#pragma once
using namespace std;

int check_valuei() {
	int x;
	do {
		cin.clear();
		cin.ignore();
		cin >> x;
	} while (cin.fail() || x <= 0);
	return x;
}

double check_valued() {
	double x;
	do {
		cin.clear();
		cin.ignore();
		cin >> x;
	} while (cin.fail() || x <= 0);
	return x;
}
