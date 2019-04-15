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
	Node* leftBrother;
	Node* rightBrother;
	int degree;

	Node() {
		this->product = NULL;
		this->parent = NULL;
		this->child = NULL;
		this->leftBrother = NULL;
		this->rightBrother = NULL;
		this->degree = 0;
	}

	Node(Product* product) {
		this->product = product;
		this->parent = NULL;
		this->child = NULL;
		this->leftBrother = NULL;
		this->rightBrother = NULL;
		this->degree = 0;
	}
};

class FibonacciHeap {
private:
	int mode = 0;
	int size;
	Node* head;

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
		cout << product->name;
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

	Node* merge(Node* first, Node* second) {
		if (first == NULL)
			return second;
		else if (second == NULL)
			return first;

		Node* newNode = first;
		Node* subLeft = second->leftBrother;

		subLeft->rightBrother = newNode->rightBrother;
		subLeft->rightBrother->leftBrother = subLeft;
		second->leftBrother = newNode;
		newNode->rightBrother = second;

		if (compare(first->product, second->product) == -1)
			newNode = first;
		else
			newNode = second;

		return newNode;
	}

	void normalize() {
		if (!head || head == head->leftBrother)
			return;

		vector<Node*> headList(int(log(size) / log(2)) + 2, NULL);
		Node* sub = head;
		while (sub != headList[sub->degree]) {
			while (headList[sub->degree] != NULL) {
				Node* inList = headList[sub->degree];
				headList[sub->degree] = NULL;
				inList->rightBrother->leftBrother = inList->leftBrother;
				inList->leftBrother->rightBrother = inList->rightBrother;
				if (compare(sub->product, inList->product) <= 0) {
					inList->parent = sub;
					inList->leftBrother = inList->rightBrother = inList;
					Node* son = sub->child;
					son = merge(son, inList);
					sub->child = son;
				}
				else {
					sub->parent = inList;
					inList->leftBrother = sub->leftBrother;
					inList->leftBrother->rightBrother = inList;
					inList->rightBrother = sub->rightBrother;
					inList->rightBrother->leftBrother = inList;
					sub->rightBrother = sub->leftBrother = sub;
					Node* son = inList->child;
					son = merge(son, sub);
					inList->child = son;
					sub = inList;
				}
				sub->degree++;
			}
			headList[sub->degree] = sub;
			sub = sub->rightBrother;
		}
		Node* last;
		head = sub;
		last = head;
		do {
			if (compare(sub->product, head->product) < 0)
				head = sub;
			sub = sub->rightBrother;
		} while (sub != last);
	}

	/***
		Displays individual node (using fancy colors) (and fancy structure)
	//*/
	void displayNodeFancy(Node * node, int tabs, int colorMode) {
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		if (node->child != NULL) {
			Node* child = node->child,
				* prevChild = child->leftBrother;
			while (child != prevChild) {
				SetConsoleTextAttribute(hConsole, colorMode);
				displayNodeFancy(child, tabs + 1, colorMode == 15 ? 9 : colorMode + 1);
				SetConsoleTextAttribute(hConsole, colorMode);
				child = child->rightBrother;
			}
			SetConsoleTextAttribute(hConsole, colorMode);
			displayNodeFancy(child, tabs + 1, colorMode == 15 ? 9 : colorMode + 1);
			SetConsoleTextAttribute(hConsole, colorMode);
		}
		SetConsoleTextAttribute(hConsole, colorMode);
		printSingle(node->product, tabs);
	}

public:
	FibonacciHeap(int mode) {
		this->mode = mode;
		this->size = 0;
		this->head = NULL;
	}

	/***
		Method which is used to get element
	//*/
	Product* getElement() {
		if (head != NULL)
			return head->product;
		else
			return NULL;
	}

	void removeElement() {
		if (!head)
			return;

		Node* toExtract = head;
		if (head == head->leftBrother)
			head = NULL;
		else
			head = head->leftBrother;
		toExtract->leftBrother->rightBrother = toExtract->rightBrother;
		toExtract->rightBrother->leftBrother = toExtract->leftBrother;

		Node* newHeap = toExtract->child;
		if (newHeap != NULL) {
			Node* sub = toExtract->child,
				* prevSub = sub->leftBrother;
			while (sub != prevSub) {
				sub->parent = NULL;
				if (compare(sub->product, newHeap->product) == -1)
					newHeap = sub;
				sub = sub->rightBrother;
			}
			sub->parent = NULL;
			if (compare(sub->product, newHeap->product) == -1)
				newHeap = sub;
		}
		head = merge(head, newHeap);
		normalize();
	}

	/***
		Method which is used to add new node to the tree
	//*/
	void addElement(Product * product) {
		Node* newNode = new Node(product);
		newNode->leftBrother = newNode->rightBrother = newNode;
		head = merge(head, newNode);
		this->size++;
	}

	void finalize() {
		normalize();
	}

	/***
		Displays single tree iteration
	//*/
	void displayTree(int mode) {
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		if (head == NULL)
			return;
		Node* sub = head,
			* prev = head->leftBrother;
		while (sub != prev) {
			displayNodeFancy(sub, 0, 9);
			sub = sub->rightBrother;
		}
		displayNodeFancy(sub, 0, 9);
		cout << '\n';
		SetConsoleTextAttribute(hConsole, 15);
	}
};