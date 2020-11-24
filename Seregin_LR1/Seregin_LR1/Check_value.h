#pragma once
#include <iostream>
using namespace std;

inline int check_valuei() {
	int x;
	while ((cin >> x).fail() || x <= 0) {
		cin.clear();
		cin.ignore();
	} 
	return x;
}

inline double check_valued() {
	double x;
	while ((cin >> x).fail() || x <= 0) {
		cin.clear();
		cin.ignore();
	}
	return x;
}

template <typename T>
inline T InBetween(T a, T b) {
	T x;
	while ((cin >> x).fail() || x < a || x > b) {
		cin.clear();
		cin.ignore();
	}
	return x;
}
