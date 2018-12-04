#include <iostream>
#include <random>
#include <time.h>
#include <string>
#include <vector>

using namespace std;

int const MAX_VALUE = 100;

int findParent(int number, int numberOfSons) {
	return (number - 1) / numberOfSons;
}

int findSon(int parent, int number, int numberOfSons) {
	return parent * numberOfSons + number;
}

void normalize(vector<int> &tree, int largeIndex, int numberOfSons) {
	int largest = largeIndex;
	for (int j = 1; j <= numberOfSons; j++) {
		int child = findSon(largeIndex, j, numberOfSons);
		if (child < tree.size() && tree[child] > tree[largest])
			largest = child;
	}
	if (largest != largeIndex) {
		swap(tree[largeIndex], tree[largest]);
		normalize(tree, largest, numberOfSons);
	}
}

int findMax(vector<int> &tree, int numberOfSons) {
	if (tree.size() < 1)
		return -1;
	int max_ = tree[0];
	tree[0] = tree[tree.size() - 1];
	tree.pop_back();
	normalize(tree, 0, numberOfSons);
	return max_;
}

int setKey(vector<int> &tree, int keyToChange, int newKey, int numberOfSons) {
	if (newKey < tree[keyToChange])
		return -1;
	tree[keyToChange] = newKey;
	while (keyToChange > 0 && tree[findParent(keyToChange, numberOfSons)] < tree[keyToChange]) {
		swap(tree[keyToChange], tree[findParent(keyToChange, numberOfSons)]);
		keyToChange = findParent(keyToChange, numberOfSons);
	}
	return newKey;
}

void insert(vector<int> &tree, int key, int numberOfSons) {
	tree.push_back(-9999);
	setKey(tree, tree.size() - 1, key, numberOfSons);
}

vector<int> generate(int numberOfSons, int numberOfElements) {
	srand(clock());
	vector<int> res;
	for (int i = 0; i < numberOfElements * numberOfSons; i++)
		res.push_back(rand() % MAX_VALUE);
	return res;
}

void test(vector<int> &tree) {
	for (int i = 0; i < tree.size(); i++)
		cout << tree[i] << " ";
	cout << endl;
}

int main() {
	int numberOfSons, numberOfElements;
	cin >> numberOfSons >> numberOfElements;
	vector<int> tree = generate(numberOfSons, numberOfElements);
	normalize(tree, 0, numberOfSons);
	test(tree);

	int key = rand() % (numberOfElements * numberOfSons),
		value = rand() % MAX_VALUE;
	cout << "Key : " << key << ". Value : " << value << endl;
	setKey(tree, key, value, numberOfSons);
	test(tree);

	key = rand() % MAX_VALUE;
	cout << "Key : " << key << endl;
	insert(tree, key, numberOfSons);
	test(tree);

	cout << "MAX : " << findMax(tree, numberOfSons) << endl;

	system("pause");
	return 0;
}
