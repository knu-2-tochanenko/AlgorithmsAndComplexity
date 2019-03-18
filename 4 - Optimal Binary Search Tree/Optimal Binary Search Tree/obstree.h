#pragma once

#include "datafile.h"
#include <iostream>
#include <iomanip>

constexpr auto INT_INFINITY = 1000000;

using namespace std;

/***
	Struct which has all needed fields for Splay Tree
//*/
struct Node {
	Product* key;
	Node* left;
	Node* right;
	Node* parent;

	float weight;

	double price() {
		return this->key->price;
	}

	Node() {
		this->key = NULL;
		this->left = NULL;
		this->right = NULL;
		this->parent = NULL;
		this->weight = 0;
	}
	Node(Product* key, Node* left, Node* right, Node* parent) {
		this->key = key;
		this->left = left;
		this->right = right;
		this->parent = parent;
		this->weight = 0;
	}
};

class OBSTree {
private:
	Node* head = NULL;
	int mode = 0;		//	1	:	Name
						//	2	:	Days till expired
						//	3	:	Weight
						//	0..	:	Price

	/***
		Structures are needed for Optimal Binary Search tree algorithm.
		Vector is used for easier elgorithm implementation.
		Can be changed to static arrays in the future.
	//*/
	vector< float > prefWeightList;
	vector< vector< float > > weights;
	vector< vector< short int> > roots;
	vector< Node* > elementsList;

	/***
		Compares two elements using specific mode
		-1	- The first value is less than second
		1	- The first value is begger than second
		0	- Values are even
	//*/
	bool compare(Product* x, Product* y) {
		/*
		if (x == NULL || y == NULL)
			return false;
		switch (mode) {
		case 1:
			if (x->name < y->name)
				return -1;
			else if (x->name > y->name)
				return 1;
			else return 0;
		case 2:
			if (x->daysTillExpired < y->daysTillExpired)
				return -1;
			else if (x->daysTillExpired > y->daysTillExpired)
				return 1;
			else return 0;
		case 3:
			if (x->weight < y->weight)
				return -1;
			else if (x->weight > y->weight)
				return 1;
			else return 0;
		default:
			if (x->price < y->price)
				return -1;
			else if (x->price > y->price)
				return 1;
			else return 0;
		}
		//*/

		//*
		string a = x->name;
		string b = y->name;
		for (int i = 0; i < min(a.length(),b.length()); ++i) {
		if (toupper(a[i]) < toupper(b[i]))
			return -1;
		else if (toupper(a[i]) > toupper(b[i]))
			return 1;
		}
		if (a.length() > b.length())
			return 1;
		else if (a.length() < b.length())
			return -1;
		else
			return 0;
		//*/
	}

	/****
		DFS is used for searching for an item
	//*/
	Node* DFS(Node* node, Product* key) {
		if (node == NULL)
			return node;
		else if (compare(node->key, key) == 1)
			return DFS(node->left, key);
		else if (compare(node->key, key) == -1)
			return DFS(node->right, key);
		return node;
	}

	/***
		A functions which is used in building the tree
	//*/
	void buildHeap(vector< Node* >& mass, int father, int treeNumber) {
		int maximumSon;
		while (father <= treeNumber / 2) {
			if (father * 2 == treeNumber)
				maximumSon = father * 2;
			else if (compare(mass[father * 2]->key, mass[father * 2 - 1]->key) == 1)
				maximumSon = father * 2 + 1;
			else
				maximumSon = father * 2;
			if (compare(mass[father - 1]->key, mass[maximumSon - 1]->key) == -1) {
				swap(mass[father - 1], mass[maximumSon - 1]);
				father = maximumSon;
			}
			else
				father = treeNumber;
		}
	}

	/***
		Another function which is needed for building OBS Tree
	//*/
	void newSort(vector< Node* > &mass) {
		for (int i = mass.size() / 2; i > 0; i--)
			buildHeap(mass, i, mass.size());

		for (int i = mass.size(); i > 0; i--) {
			swap(mass[i - 1], mass[0]);
			buildHeap(mass, 1, i - 1);
		}
	}

	/***
		Main functions for building a main tree
	//*/
	void buildTree(vector< Node* >& mass, Node* node, int x, int y) {
		if (roots[x][y] < 0)
			return;
		Node* curV = mass[roots[x][y] - 1];
		if (node == NULL)
			head = curV;
		else {
			curV->parent = node;
			if (compare(curV->key, node->key) == 1)
				node->right = curV;
			else
				node->left = curV;
		}
		if (roots[x][y] > x)
			buildTree(mass, curV, x, roots[x][y] - 1);
		if (roots[x][y] < y)
			buildTree(mass, curV, roots[x][y] + 1, y);
	}

	/***
		Display matrixes
	//*/
	// TODO : Rewrite
	void showMatrixRootsAndWeights() {
		cout << "<-------------------------------prefics weight list------------------------------->\n";
		for (int i = 0; i < prefWeightList.size(); i++) {
			cout.width(9);
			cout << fixed << setprecision(6) << prefWeightList[i];
		}
		cout << "\n\n<-------------------------------matrix of weights------------------------------->\n";
		for (int i = 0; i < weights.size(); i++) {
			for (int j = 0; j < weights[i].size(); j++) {
				cout.width(9);
				if (weights[i][j] == 1e+6)
					cout << -1;
				else
					cout << fixed << setprecision(6) << weights[i][j];
			}
			cout << '\n';
		}

		cout << "\n<-------------------------------matrix of roots------------------------------->\n";
		for (int i = 0; i < roots.size(); i++) {
			for (int j = 0; j < roots[i].size(); j++) {
				cout.width(3);
				cout << roots[i][j];
			}
			cout << '\n';
		}
	}

	/***
		Build matrixes
	//*/
	// TODO : Rewrite
	void buildRootsAndWeights(vector< Node* >& nodeList) {
		if (nodeList.empty())
			return;
		prefWeightList.push_back(nodeList[0]->weight);
		for (int i = 1; i < nodeList.size(); i++)
			prefWeightList.push_back(prefWeightList[i - 1] + nodeList[i]->weight);
		roots.resize(nodeList.size() + 2, vector<short int>(nodeList.size() + 2, -1));
		weights.resize(nodeList.size() + 2, vector<float>(nodeList.size() + 2, INT_INFINITY));
		for (int i = 1; i <= nodeList.size() + 1; i++)
		{
			roots[i][i - 1] = 0;
			weights[i][i - 1] = 0;
		}
		for (int step = 1; step <= nodeList.size(); ++step)
		{
			for (int i = 1; i + step <= nodeList.size() + 1; i++)
			{
				int j = i + step - 1;
				int myLeft = i, myRight = j;
				if (step > 1)
				{
					myLeft = roots[i][j - 1];
					myRight = roots[i + 1][j];
				}

				double prefSum = prefWeightList[j - 1] - (i > 1 ? prefWeightList[i - 2] : 0);
				for (int k = myLeft; k <= myRight; ++k)
				{
					double val = weights[i][k - 1] + weights[k + 1][j] + prefSum;
					if (val < weights[i][j])
					{
						weights[i][j] = val;
						roots[i][j] = k;
					}
				}
			}
		}

		if (nodeList.size() <= 16)
			showMatrixRootsAndWeights();

		buildTree(nodeList, NULL, 1, nodeList.size());
	}

	/***
		A function which is used for displaying path for the node
	//*/
	// TODO : Rewrite
	void showDfs(Node* v, int numberOfTabs)
	{
		if (v->right != NULL)
			showDfs(v->right, numberOfTabs + 1);
		else
			cout << '\n';

		for (int i = 0; i < numberOfTabs; i++)
			cout << "\t";
		cout << v->key->name << "   " << v->weight;

		if (v->left != NULL)
			showDfs(v->left, numberOfTabs + 1);
		else
			cout << '\n';
	}

	// TODO : Make internal list which is going to be inhabited with elements
	void mainBuild() {
		vector< Node* > tempList, nodeList;
		for (int i = 0; i < elementsList.size(); i++)
			tempList.push_back(elementsList[i]);

		newSort(tempList);

		double allWeight = 0;
		for (int i = 0; i < tempList.size(); i++)
		{
			if (tempList[i]->key->name != "") {
				nodeList.push_back(tempList[i]);
				nodeList[nodeList.size() - 1]->weight = rand() % (elementsList.size()*elementsList.size());
				allWeight += nodeList[nodeList.size() - 1]->weight;
			}
		}
		tempList.clear();
		for (int i = 0; i < nodeList.size(); i++)
			nodeList[i]->weight /= allWeight;

		buildRootsAndWeights(nodeList);
	}

	/***
		Displays individual node (using fancy colors)
	//*/
	void displayNode(int mode, Node* node, int colorMode) {
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, colorMode);
		cout << node->key->name;
		switch (mode) {
		case 1:
			cout << "-" << node->key->name;
			break;
		case 2:
			cout << "-" << node->key->daysTillExpired;
			break;
		case 3:
			cout << "-" << node->key->weight;
			break;
		default:
			cout << "-" << node->key->price;
			break;
		}
		if (node->left != NULL || node->right != NULL)
			cout << " { ";
		if (node->left != NULL)
			displayNode(mode, node->left, colorMode == 15 ? 9 : colorMode + 1);
		SetConsoleTextAttribute(hConsole, colorMode);
		if (node->left != NULL || node->right != NULL)
			cout << " | ";
		if (node->right != NULL)
			displayNode(mode, node->right, colorMode == 15 ? 9 : colorMode + 1);
		SetConsoleTextAttribute(hConsole, colorMode);
		if (node->left != NULL || node->right != NULL)
			cout << " } ";
	}

public:
	/***
		You NEED to chose which mode to use
	//*/
	OBSTree(int mode) { this->mode = mode; }

	/***
		Adds element to the tree
	//*/
	// TODO : Rewrite
	void addElement(Product *node) {
		elementsList.push_back(new Node(node, NULL, NULL, NULL));
	}

	/***
		CALL THIS FUNCTION AFTER ALL NODES WERE ADDED
	//*/
	void build() {
		mainBuild();
	}

	/***
		Use this function to find element with specific
		values. Note, that key will be created in the "store" class
	//*/
	// TODO : Rewrite
	Node* getElement(Product* node) {
		Node* v = DFS(head, node);
		if (v != NULL)
			return v;
		return NULL;
	}

	/***
		Displays full tree
	//*/
	// TODO : Rewrite
	void displayTree(int mode) {
		/*
			shared_ptr<TNode> temp = root;
	showDfs(temp, 0);
		*/

		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		displayNode(mode, head, 9);
		SetConsoleTextAttribute(hConsole, 15);
	}
};