#include <string>
#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

typedef long long llong;
long long const MAX = pow(2, 63);

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

vector<llong> rabinKarp(string source, string toFind, int mul) {
	llong sourceSize = source.size(), toFindSize = toFind.size(), sourceSub = 0, toFindSub = 0;
	vector<llong> result;
	if (sourceSize < toFindSize || sourceSize == 0 || toFindSize == 0)
		return result;

	llong mHash = findHash(toFindSize, MAX);
	for (llong i = 0; i < toFindSize; ++i) {
		sourceSub = (mul * sourceSub + toFind[i]) % MAX;
		toFindSub = (mul * toFindSub + source[i]) % MAX;
	}

	for (llong shift = 0; shift <= sourceSize - toFindSize; ++shift) {
		if (sourceSub == toFindSub)
			if (compareWithShift(source, toFind, shift))
				result.push_back(shift + 1);
		toFindSub = (mul * (toFindSub - source[shift] * mHash) + source[shift + toFindSize]) % MAX;
	}
	return result;
}

int main() {
	char matrix[5][5] = {	{'h', 'e', 'l', 'l', 'o'},
							{'a', 'e', 'a', 'a', 'o'}, 
							{'l', 'e', 'a', 'l', 'o'}, 
							{'l', 'e', 'a', 'l', 'o'}, 
							{'o', 'e', 'a', 'o', 'o'}, };
	string source, toFind = "hello";
	vector<llong> result;
	int mul = 13;
	
	for (int i = 0; i < 5; i++) {
		// Horizontal
		source = "";
		result.clear();
		for (int j = 0; j < 5; j++)
			source += matrix[i][j];
		result = rabinKarp(source, toFind, mul);
		if (result.size() > 0) {
			cout << "Horizontal in " << i << " row : ";
			for (int j = 0; j < result.size(); j++)
				cout << result[j] << " ";
			cout << endl;
		}

		// Vertical
		source = "";
		result.clear();
		for (int j = 0; j < 5; j++)
			source += matrix[j][i];
		result = rabinKarp(source, toFind, mul);
		if (result.size() > 0) {
			cout << "Horizontal in " << i << " column : ";
			for (int j = 0; j < result.size(); j++)
				cout << result[j] << " ";
			cout << endl;
		}
	}
	system("pause");
	return 0;
}