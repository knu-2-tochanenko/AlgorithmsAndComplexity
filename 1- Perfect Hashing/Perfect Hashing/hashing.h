#pragma once

#include <iostream>
#include <string>
#include <vector>
using namespace std;

unsigned int firstHash(string str, int len) {
	unsigned int hash = str[0];
	for (int i = 1; i < str.length(); i++)
		hash = ((hash << 5) + hash) ^ str[i];
	return hash % len;
}

unsigned int secondHash(string str, int len) {
	unsigned int hash = str[0];
	for (int i = 1; i < str.length(); i++)
		hash = ((((hash << 5) + hash) << 5) + hash) ^ str[i];
	return hash % len;
}

template <typename T> struct Cell {
	int elements;				//	Number of elements in sublist
	vector<T> elementsList;		//	Elements with second hash

	T operator[](int n) {
		return this->elementsList[n];
	}
};