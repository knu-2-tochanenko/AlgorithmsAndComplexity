#pragma once

#include "datafile.h"
#include <iostream>

using namespace std;

enum Color { black, red };

/***
	Struct which has all needed fields for Order Statistics Tree
//*/
struct Node {
	Color color;
	bool isNil;
	Product* key;

	Node* left;
	Node* right;
	Node* parent;

	Node() = default;
	Node(Color color, bool isNil, Product* key, Node* left, Node* right, Node* parent) {
		this->color = color;
		this->isNil = isNil;
		this->key = key;
		this->left = left;
		this->right = right;
		this->parent = parent;
	}
	Node(Node* node) {
		this->color = node->color;
		this->key = node->key;
		this->left = node->left;
		this->right = node->right;
		this->parent = node->parent;
		this->isNil = node->isNil;
	}
};

class PersistentSet {
private:
	/***
		nil - element which is a child for every leaf and a parent for head
		head - a head of the tree
	//*/
	Node* nil = new Node(black, true, NULL, NULL, NULL, NULL);
	Node* head = new Node(black, false, NULL, nil, nil, nil);

	int mode = 0;

	//	A vector of head for different iterations of set
	vector<Node*> iterations;

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

	/***
		Actually, rotates left
	//*/
	void rotateLeft(int iteration, Node* node) {
		Node* sub = node->right;
		node->right = sub->left;

		if (sub->left != nil)
			sub->left->parent = node;

		sub->parent = node->parent;

		if (node->parent == nil)
			this->iterations[iteration] = sub;
		else if (node == node->parent->left)
			node->parent->left = sub;
		else node->parent->right = sub;

		sub->left = node;
		node->parent = sub;
	}

	/***
		Actually, rotates right
	//*/
	void rotateRight(int iteration, Node* node) {
		Node* sub = node->left;
		node->left = sub->right;

		if (sub->right != nil)
			sub->right->parent = node;

		sub->parent = node->parent;

		if (node->parent == nil)
			this->iterations[iteration] = sub;
		else if (node == node->parent->right)
			node->parent->right = sub;
		else node->parent->left = sub;

		sub->right = node;
		node->parent = sub;
	}

	/***
		The method which NEEDS to be executed after EACH element adding
	//*/
	void addNormalize(int iteration, Node * node) {
		Node *sub;
		while (node->parent->color == red) {
			if (node->parent->parent == nil)
				break;
			if (node->parent == node->parent->parent->left) {
				sub = node->parent->parent->right;
				if (sub->color == red) {
					node->parent->color = black;
					sub->color = black;
					node->color = red;
					node->parent->parent->color = red;
					node = node->parent->parent;
				}
				else {
					if (node == node->parent->right) {
						node = node->parent;
						rotateLeft(iteration, node);
					}
					node->parent->color = black;
					node->parent->parent->color = red;
					rotateRight(iteration, node->parent->parent);
				}
			}
			else {
				sub = node->parent->parent->left;
				if (sub->color == red) {
					node->parent->color = black;
					sub->color = black;
					node->color = red;
					node->parent->parent->color = red;
					node = node->parent->parent;
				}
				else {
					if (node == node->parent->left) {
						node = node->parent;
						rotateRight(iteration, node);
					}
					node->parent->color = black;
					node->parent->parent->color = red;
					rotateLeft(iteration, node->parent->parent);
				}
			}
		}
		this->iterations[iteration]->color = black;
	}

	/***
		Displays individual node (using fancy colors) (and fancy structure)
	//*/
	void displayNodeFancy(Node* node, int tabs, int colorMode) {
		if (node == nil)
			return;
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, colorMode);
		if (node->right != nil)
			displayNodeFancy(node->right, tabs + 1, colorMode == 15 ? 9 : colorMode + 1);
		else
			cout << endl;
		SetConsoleTextAttribute(hConsole, colorMode);
		for (int i = 0; i < tabs; i++)
			cout << "\t";
		cout << node->key->name;
		
		switch (mode) {
		case 1:
			break;
		case 2:
			cout << " - " << node->key->daysTillExpired;
			break;
		case 3:
			cout << " - " << node->key->weight;
			break;
		default:
			cout << " - " << node->key->price;
			break;
		}

		if (node->left != nil)
			displayNodeFancy(node->left, tabs + 1, colorMode == 15 ? 9 : colorMode + 1);
		else
			cout << endl;
		SetConsoleTextAttribute(hConsole, colorMode);
	}

	/***
		Displays individual node (using fancy colors)
	//*/
	void displayNode(Node* node, int colorMode) {
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, colorMode);
		cout << node->key->name;
		if (node->left != nil || node->right != nil)
			cout << " { ";
		if (node->left != nil)
			displayNode(node->left, colorMode == 15 ? 9 : colorMode + 1);
		SetConsoleTextAttribute(hConsole, colorMode);
		if (node->left != nil || node->right != nil)
			cout << " | ";
		if (node->right != nil)
			displayNode(node->right, colorMode == 15 ? 9 : colorMode + 1);
		SetConsoleTextAttribute(hConsole, colorMode);
		if (node->left != nil || node->right != nil)
			cout << " } ";
	}

	/***
		Methods which is used to copy all sons for current node
	//*/
	void copySons(Node* node) {
		copyLeft(node);
		copyRight(node);
	}

	void copyLeft(Node* node) {
		if (node->left != nil) {
			Node* newLeft = new Node(node->left);
			newLeft->parent = node;
			node->left = newLeft;
		}
	}

	void copyRight(Node * node) {
		if (node->right != nil) {
			Node* newRight = new Node(node->right);
			newRight->parent = node;
			node->right = newRight;
		}
	}

	/***
		Methods which is used to add new node to the tree
	//*/
	void addElement(int iteration, Product* product) {
		Node* node = new Node(red, false, product, nil, nil, nil);

		if (this->iterations[iteration]->key == NULL) {
			this->iterations[iteration] = node;
			return;
		}

		Node* copyHead = new Node(this->iterations[iteration]);
		Node* currentNode;
		Node* currentNodeParent;
		Node* newHead;

		currentNode = copyHead;
		currentNodeParent = nil;

		while (currentNode != nil) {
			currentNodeParent = currentNode;
			
			int compareResult = compare(product, currentNode->key);

			if (compareResult < 1) {
				if (currentNode->left == nil)
					break;

				Node* copyLeft = new Node(currentNode->left);
				currentNode->left = copyLeft;
				copyLeft->parent = currentNode;
				currentNode = copyLeft;
			}
			else {
				if (currentNode->right == nil)
					break;

				Node* copyRight = new Node(currentNode->right);
				copyRight->parent = currentNode;
				currentNode->right = copyRight;
				currentNode = copyRight;
			}
		}

		node->parent = currentNodeParent;

		if (currentNodeParent != nil) {
			int compareResult = compare(product, currentNode->key);
			if (compareResult < 1)
				currentNodeParent->left = node;
			else
				currentNodeParent->right = node;
		}
		else {
			newHead = node;
		}
		this->iterations.push_back(newHead);

		iteration++;

		this->iterations[iteration] = copyHead;

		if (node->parent != nil) {
			if (node == node->parent->left)
				copyRight(node->parent);
			else
				copyLeft(node->parent);
			if (node->parent->parent != nil)
				copySons(node->parent->parent);
		}

		addNormalize(iteration, node);
	}

public:
	/***
		Constructor. Setting nil's is CRUICIAL for this tree to work.
		Note, that nil->size is NEEDS to be 0
	//*/
	PersistentSet(int mode) {
		nil->left = nil;
		nil->right = nil;
		nil->parent = nil;
		this->mode = mode;
		this->iterations.push_back(head);
	}

	/***
		Method which is used to get element
	//*/
	Node* getElement(int iteration, Product* product) {
		Node* currentNode = this->iterations[iteration];
		Node* perviousNode = this->iterations[iteration];
		while (currentNode != nil) {
			if (compare(product, currentNode->key) == -1) {
				perviousNode = currentNode;
				currentNode = currentNode->left;
			}
			else {
				perviousNode = currentNode;
				currentNode = currentNode->right;
			}
		}
		if (compare(perviousNode->key, product) == 0)
			return perviousNode;
		return nil;
	}

	/***
		Methods which is used to add new node to the tree
	//*/
	void addElement(Product* product) {
		int iteration = this->iterations.size() - 1;
		addElement(iteration < 0 ? 0 : iteration, product);
	}

	/***
		Displays full tree list
	//*/
	void displayTrees() {
		int numberOfIterations = this->iterations.size();
		for (int i = 0; i < numberOfIterations; i++)
			displayTree(i);
	}

	/***
		Displays single tree iteration
	//*/
	void displayTree(int iteration) {
		// TODO :: Number of iterations!!!
		cout << "ITERATION " << iteration << "------------------------------\n";
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		displayNodeFancy(this->iterations[iteration], 1, 9);
		SetConsoleTextAttribute(hConsole, 15);
	}

	int getNumberOfIterations() {
		return this->iterations.size();
	}
};