#pragma once

#include <iostream>
#include <string>
#include <vector>

using namespace std;

template <typename T>
class HashTable {
private:
	struct Cell {
		shared_ptr<T> element;
		int numberOfElements;
		vector<T> elementsList;
	}
	vector<Cell> table;

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

public:

};