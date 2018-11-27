#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include <map>

using namespace std;

typedef long long llong;
long long const MAX = pow(2, 63);
// Naive //////////////////////////////////////////////////////
int naiveFind(string source, string toFind) {
	int sourceSize = source.size();
	for (int i = 0; i < sourceSize; i++) {
		bool found = true;
		for (int j = 0; (j < toFind.size()) && (found); j++) {
			if (source[i + j] != toFind[j])
				found = false;
		}
		if (found)
			return i;
	}
	return -1;
}
// Naive //////////////////////////////////////////////////////

// Horspool ///////////////////////////////////////////////////
int horspool(string source, string toFind) {
	int numberOfLetters = 'z' - 'a';
	map<char, int> shift;
	map<char, int>::iterator it;
	int toFindSize = toFind.size();
	int sourceSize = source.size();
	for (int i = 0; i < toFind.size() - 1; i++)
		shift[toFind[i]] = toFindSize - i - 1;

	for (int i = 0; i <= sourceSize - toFindSize;) {
		if (toFind[toFindSize - 1] == source[i + toFindSize - 1]) {
			bool same = true;
			for (int j = 0; j < toFindSize; j++)
				if (source[i + toFindSize - 1 - j] != toFind[toFindSize - 1 - j])
					same = false;
			if (same)
				return i;
		}
		it = shift.find(source[i + toFindSize - 1]);
		if (it == shift.end())
			i += toFindSize;
		else
			i += shift[source[i + toFindSize - 1]];
	}
	return -1;
}
// Horspool ///////////////////////////////////////////////////

// Boyer Moore ////////////////////////////////////////////////
int boyerMoore(string source, string toFind) {
	return -1;
}
// Boyer Moore ////////////////////////////////////////////////

// Rabin Karp /////////////////////////////////////////////////
llong findHash(llong m, llong q) {
	llong newHash = 1;
	for (long long i = 1; i < m; ++i)
		newHash *= 10 % q;
	return newHash;
}
bool compareWithShift(string s1, string s2, llong shift) {
	for (llong i = 0; i < s2.size(); ++i)
		if (s1[i + shift] != s2[i])
			return false;
	return true;
}
llong rabinKarp(string source, string toFind, int mul) {
	llong sourceSize = source.size(), toFindSize = toFind.size(), sourceSub = 0, toFindSub = 0;
	if (sourceSize < toFindSize || sourceSize == 0 || toFindSize == 0)
		return -1;

	llong mHash = findHash(toFindSize, MAX);
	for (llong i = 0; i < toFindSize; ++i) {
		sourceSub = (mul * sourceSub + toFind[i]) % MAX;
		toFindSub = (mul * toFindSub + source[i]) % MAX;
	}

	for (llong shift = 0; shift <= sourceSize - toFindSize; ++shift) {
		if (sourceSub == toFindSub)
			if (compareWithShift(source, toFind, shift))
				return shift;
		toFindSub = (mul * (toFindSub - source[shift] * mHash) + source[shift + toFindSize]) % MAX;
	}
	return -1;
}
// Rabin Karp /////////////////////////////////////////////////
int main() {
	// we can find only lowercase letters
	string source = "This helllo is the most terrifying helllo in the world.", toFind = "hello";
	vector<llong> result;
	int mul = 10;
	
	cout << "Naive : " << naiveFind(source, toFind) << endl;
	cout << "Rabin Karp : " << rabinKarp(source, toFind, mul) << endl;
	cout << "Horspol : " << horspool(source, toFind) << endl;

	system("pause");
	return 0;
}