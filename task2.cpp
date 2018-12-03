#include <iostream>
#include <vector>
#include <random>
#include <string>
#include <time.h>

using namespace std;

int const MAX = 10;
int const MAX_VALUE = 10;

vector<int> searchNutBoldPairs(vector<int> &nuts, vector<int> &bolts) {
	vector<int> res;

	for (int i = 0; i < nuts.size(); ++i)
		for (int j = 0; j < bolts.size(); ++j)
			if (bolts[j] != 0 && nuts[i] == bolts[j]) {
				res.push_back(bolts[j]);
				nuts[i] = 0;
				bolts[j] = 0;
				break;
			}
	return res;
}

vector<int> findLeft(vector<int> &inputVector) {
	vector<int> res;
	for (int i = 0; i < inputVector.size(); ++i)
		if (inputVector[i] != 0) {
			res.push_back(inputVector[i]);
			inputVector[i] = 0;
		}
	return res;
}

void singleVector(string str, vector<int> &vec) {
	cout << str << endl;
	for (int i = 0; i < vec.size(); i++)
		cout << vec[i] << " ";
	cout << endl;
}

void test(vector<int> &res, vector<int> &leftBolts, vector<int> &leftNuts) {
	singleVector("Res:", res);
	singleVector("Left Bolts:", leftBolts);
	singleVector("Left Nuts:", leftNuts);
}

int main() {
	srand(clock());
	vector<int> bolts;
	vector<int> nuts;
	vector<int> res;
	vector<int> leftBolts;
	vector<int> leftNuts;

	for (int i = 0; i < MAX; i++) {
		bolts.push_back((rand() % MAX_VALUE) + 1);
		nuts.push_back((rand() % MAX_VALUE) + 1);
	}

	singleVector("Nuts:", nuts);
	singleVector("Bolts:", bolts);

	res = searchNutBoldPairs(nuts, bolts);
	leftBolts = findLeft(bolts);
	leftNuts = findLeft(nuts);
	
	test(res, leftBolts, leftNuts);

	system("pause");
	return 0;
}