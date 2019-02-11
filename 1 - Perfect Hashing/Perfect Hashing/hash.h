#pragma once

#include <iostream>
#include <string>
#include <vector>
using namespace std;

unsigned int firstHash(string str, int len) {
	unsigned int hash = str[0];
	for (unsigned int i = 1; i < str.length(); i++)
		hash = ((hash << 5) + hash) ^ str[i];
	return hash % (len == 0 ? 1 : len);
}

unsigned int secondHash(string str, int len, int multiplier) {
	unsigned int hash = str[0];
	for (unsigned int i = 1; i < str.length(); i++)
		hash = (hash * multiplier) ^ str[i];
	return hash % (len == 0 ? 1 : len);
}

struct Product {
	string name;			//	The name of product
	int daysTillExpired;	//	Number of days when product is fresh
	double weight;			//	[kg]	The regular weight of product
	double cost;			//	[$]		Recommended cost of product
};