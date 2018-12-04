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
	vector<int> shiftGoodChar(toFind.size() + 1, toFind.size());
	vector<int> shiftBadChar(256, toFind.size());

	vector<int> sub(toFind.size(), 0);
	for (int j = 1, maxZidx = 0, maxZ = 0; j < toFind.size(); j++) {
		if (j <= maxZ)
			sub[j] = min(maxZ - j + 1, sub[j - maxZidx]);
		while (j + sub[j] < toFind.size() && source[toFind.size() - 1 - sub[j]] == source[toFind.size() - 1 - (j + sub[j])])
			sub[j]++;
		if (j + sub[j] - 1 > maxZ) {
			maxZidx = j;
			maxZ = j + sub[j] - 1;
		}
	}
	for (int j = toFind.size() - 1; j > 0; j--)
		shiftGoodChar[toFind.size() - sub[j]] = j;
	for (int j = 1, r = 0; j <= toFind.size() - 1; j++)
		if (j + sub[j] == toFind.size())
			for (; r <= j; r++)
				if (shiftGoodChar[r] == toFind.size()) shiftGoodChar[r] = j;

	for (int j = 0; j < toFind.size() - 1; j++)
		shiftBadChar[(int)toFind[j]] = toFind.size() - j - 1;

	int i = 0;
	while (i <= (source.size() - toFind.size())) {
		int j = 0;
		for (j = toFind.size() - 1; j >= 0 && toFind[j] == source[i + j]; j--);
		if (j < 0)
			return i;
		else
			i += max((shiftGoodChar[j + 1]), (int)(shiftBadChar[(int)source[i + j]] - toFind.size() + j + 1));
	}
	return -1;
}
// Boyer Moore ////////////////////////////////////////////////

// KPM ////////////////////////////////////////////////
int KPM(const string source, const string toFind) {
	vector<int> sub;
	sub.resize(toFind.size());
	sub[0] = 0;
	for (int i = 1; i < sub.size(); i++) {
		int pos = sub[i - 1];
		while (pos > 0 && toFind[pos] != toFind[i])
			pos = sub[pos - 1];
		sub[i] = pos + (toFind[pos] == toFind[i] ? 1 : 0);
	}

	int pos = 0;
	for (int i = 0; i < source.size(); i++) {
		while (pos > 0 && (pos >= toFind.size() || toFind[pos] != source[i]))
			pos = sub[pos - 1];
		if (source[i] == toFind[pos]) pos++;
		if (pos == toFind.size())
			return (i - pos + 1);
	}
	return -1;
}
// KPM ////////////////////////////////////////////////

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
	string source = "This helllo is the most terrifying hhelloo in the world.", toFind = "hello";
	vector<llong> result;
	int mul = 10;
	
	cout << "Naive : " << naiveFind(source, toFind) << endl;
	cout << "Rabin Karp : " << rabinKarp(source, toFind, mul) << endl;
	cout << "Horspol : " << horspool(source, toFind) << endl;
	cout << "Boyer Moore : " << boyerMoore(source, toFind) << endl;
	cout << "KPM : " << KPM(source, toFind) << endl;

	system("pause");
	return 0;
}