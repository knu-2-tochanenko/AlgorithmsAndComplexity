#pragma once

#include <iostream>
#include <string>
using std::string;

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