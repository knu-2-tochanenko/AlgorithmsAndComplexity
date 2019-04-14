#pragma once

#include "datafile.h"
#include <iostream>

using namespace std;

/***
	Struct which has all needed fields for Binomial Heap
//*/
struct Node {
	Product* product;
	Node* parent;
	Node* child;
	Node* sibbling;
	int degree;

	Node() = default;
	Node(Product* product) {
		this->product = product;
		this->parent = NULL;
		this->child = NULL;
		this->sibbling = NULL;
		this->degree = -1;
	}
};

class BinomialHeap {
private:
	int mode = 0;
	vector<Node*> heads;
	/***
		Compares two elements using specific mode
		-1	- The first value is less than second
		1	- The first value is bigger than second
		0	- Values are even
	//*/
	int compare(Product* x, Product* y) {
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
	}

	void printSingle(Product* product, int tabs) {
		for (int i = 0; i < tabs; i++)
			cout << "\t";
		cout << product->name << " - ";
		switch (mode) {
		case 1:
			break;
		case 2:
			cout << " - " << product->daysTillExpired;
			break;
		case 3:
			cout << " - " << product->weight;
			break;
		default:
			cout << " - " << product->price;
			break;
		}
		cout << endl;
	}

	Node* getMinimum() {
		int headsCount = this->heads.size();
		if (headsCount == 0)
			return NULL;
		Node* min = this->heads[0];
		for (int i = 1; i < headsCount; i++)
			if (compare(this->heads[i]->product, min->product) == -1)
				min = this->heads[i];
		return min;
	}

	Node* removeMinimum(Node* head) {
		//	TODO :: Write a function
		Node* min = getMinimum();
		
		Node* x = NULL;
		Node* beforeX = NULL;
		Node* curX = head;
		Node* beforeCurX = NULL;

		while (curX != NULL) {
			if (compare(curX->product, min->product) == -1) {

			}
		}
	}

	Node* unite(Node* node, Node* sibbling) {

	}

	/***
		Function which is needed for almost every operation on Binomial Heap
	//*/
	Node* merge(Node* first, Node* second) {
		if (first == NULL)
			return second;
		else if (second == NULL)
			return first;

		Node* newNode, *sub;
		if (first->degree > second->degree) {
			newNode = second;
			sub = newNode;
			second = second->sibbling;
		}
		else {
			sub = newNode = first;
			first = first->sibbling;
		}

		while (first != NULL && second != NULL)
			if (first->degree < second->degree) {
				sub->sibbling = first;
				sub = first;
				first = first->sibbling;
			}
			else {
				sub->sibbling = second;
				sub = second;
				second = second->sibbling;
			}

		while (first != NULL) {
			sub->sibbling = first;
			sub = first;
			first = first->sibbling;
		}
		while (second != NULL) {
			sub->sibbling = second;
			sub = second;
			second = second->sibbling;
		}

		newNode = makeUnion(newNode);

		return newNode;
	}

	/***
		Displays individual node (using fancy colors) (and fancy structure)
	//*/
	void displayNodeFancy(Node* node, int tabs, int colorMode) {
		//	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		//	colorMode == 15 ? 9 : colorMode + 1
		//	SetConsoleTextAttribute(hConsole, colorMode);
		Node* child = node->child;
		if (node->child != NULL) {
			SetConsoleTextAttribute(hConsole, colorMode);
			displayNodeFancy(node->child, tabs + 1, colorMode == 15 ? 9 : colorMode + 1);
			SetConsoleTextAttribute(hConsole, colorMode);
			cout << '\n';
			printSingle(node->product, tabs);
		}
		if (node->lefaa) {
			SetConsoleTextAttribute(hConsole, colorMode);
			for (int i = 0; i < node->products.size(); i++) {
				for (int j = 0; j < tabs; j++)
					cout << "\t";
				printSingle(node->products[i]);
			}
		}
		SetConsoleTextAttribute(hConsole, colorMode);
	}

public:
	BinomialHeap(int mode) {
		this->mode = mode;
	}

	/***
		Method which is used to get element
	//*/
	Product* getElement(Product* product) {

	}

	/***
		Method which is used to add new node to the tree
	//*/
	void addElement(Product* product) {

	}

	/***
		Displays single tree iteration
	//*/
	void displayTree(int mode) {
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		displayNodeFancy(head, 1, 9);
		SetConsoleTextAttribute(hConsole, 15);
	}
};