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

	Node() {
		this->product = NULL;
		this->parent = NULL;
		this->child = NULL;
		this->sibbling = NULL;
		this->degree = 0;
	}

	Node(Product* product) {
		this->product = product;
		this->parent = NULL;
		this->child = NULL;
		this->sibbling = NULL;
		this->degree = 0;
	}
};

class BinomialHeap {
private:
	int mode = 0;
	Node* minimumHead;

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

	pair<Node*, Node*> getMinimum() {
		if (minimumHead != NULL)
			return { NULL, NULL };
		Node * sub = minimumHead->sibbling,
			*foundNode = minimumHead,
			*leftSibbling = minimumHead,
			*leftFoundSibbling = NULL;
		Product * minimumElement = minimumHead->product;
		while (sub != NULL) {
			if (compare(sub->product, minimumElement) == -1) {
				minimumElement = sub->product;
				leftFoundSibbling = leftSibbling;
				foundNode = sub;
			}
			leftSibbling = sub;
			sub = sub->sibbling;
		}
		return { foundNode, leftFoundSibbling };
	}

	Node* unite(Node* node, Node* sibbling = NULL) {
		if (node == NULL || node->sibbling == NULL)
			return node;
		Node * rightSibbling = node->sibbling;
		while ((rightSibbling != NULL) && node->degree == rightSibbling->degree) {
			if (compare(node->product, rightSibbling->product) <= 0) {
				node->sibbling = rightSibbling->sibbling;
				rightSibbling->sibbling = node->child;
				node->child = rightSibbling;
				rightSibbling->parent = node;
				(node->degree)++;
			}
			else {
				node->sibbling = rightSibbling->child;
				rightSibbling->child = node;
				node->parent = rightSibbling;
				if (sibbling != NULL)
					sibbling->sibbling = rightSibbling;
				(rightSibbling->degree)++;
				node = rightSibbling;
			}
			if (node->sibbling) {
				Node* sub = node, * leftSubSibbling = sibbling, * rightSubSibbling = sub->sibbling;
				bool ifChanged = false;
				while (rightSubSibbling && sub->degree > rightSubSibbling->degree) {
					if (leftSubSibbling != NULL)
						leftSubSibbling->sibbling = rightSubSibbling;
					if (ifChanged) {
						node = rightSubSibbling;
						ifChanged = true;
					}
					sub->sibbling = rightSubSibbling->sibbling;
					rightSubSibbling->sibbling = sub;
					leftSubSibbling = sub;
					sub = rightSubSibbling;
					rightSubSibbling = rightSubSibbling->sibbling;
				}
			}
			rightSibbling = node->sibbling;
		}
		unite(node->sibbling, node);
		return node;
	}

	/***
		Function which is needed for almost every operation on Binomial Heap
	//*/
	Node* merge(Node* first, Node* second) {
		if (first == NULL)
			return second;
		else if (second == NULL)
			return first;

		Node * newNode, *sub;
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

		newNode = unite(newNode);

		return newNode;
	}

	/***
		Displays individual node (using fancy colors) (and fancy structure)
	//*/
	void displayNodeFancy(Node* node, int tabs, int colorMode) {
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		Node* child = node->child;
		while (child) {
			SetConsoleTextAttribute(hConsole, colorMode);
			displayNodeFancy(child, tabs + 1, colorMode == 15 ? 9 : colorMode + 1);
			SetConsoleTextAttribute(hConsole, colorMode);
			child = child->sibbling;
		}
		SetConsoleTextAttribute(hConsole, colorMode);
		printSingle(node->product, tabs);
	}

	pair<Node*, Node*> findMin() {
		if (minimumHead == NULL)
			return { NULL, NULL };
		Node* sub = minimumHead->sibbling,
			* foundNode = minimumHead,
			* leftSibbling = minimumHead,
			* leftSubSibbling = NULL;
		Product* minimumElement = minimumHead->product;
		while (sub != NULL) {
			if (compare(sub->product, minimumElement) == -1) {
				minimumElement = sub->product;
				leftSubSibbling = leftSibbling;
				foundNode = sub;
			}
			leftSibbling = sub;
			sub = sub->sibbling;
		}
		return { foundNode, leftSubSibbling };
	}

public:
	BinomialHeap(int mode) {
		this->mode = mode;
	}

	/***
		Method which is used to get element
	//*/
	Product* getElement() {
		Node* minNode = findMin().first;
		if (minNode != NULL)
			return minNode->product;
		else
			return NULL;
	}

	void removeElement() {
		if (!minimumHead)
			return;
		pair<Node*, Node*> minNode = findMin();
		Node* foundNode = minNode.first,
			* leftSibbling = minNode.second;

		if (minimumHead == foundNode)
			minimumHead = minimumHead->sibbling;
		else
			leftSibbling->sibbling = foundNode->sibbling;

		Node* newTree = NULL,
			* sub = NULL,
			* subsub = foundNode->child;
		while (subsub) {
			subsub->parent = NULL;
			sub = subsub->sibbling;
			if (newTree)
				subsub->sibbling = newTree;
			else
				subsub->sibbling = NULL;
			newTree = subsub;
			subsub = sub;
		}
		minimumHead = merge(minimumHead, newTree);
	}

	/***
		Method which is used to add new node to the tree
	//*/
	void addElement(Product * product) {
		Node* newNode = new Node(product);
		minimumHead = merge(minimumHead, newNode);
	}

	/***
		Displays single tree iteration
	//*/
	void displayTree(int mode) {
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		Node* sub = minimumHead;
		while (sub != NULL) {
			displayNodeFancy(sub, 1, 9);
			sub = sub->sibbling;
			cout << endl;
		}
		SetConsoleTextAttribute(hConsole, 15);
	}
};