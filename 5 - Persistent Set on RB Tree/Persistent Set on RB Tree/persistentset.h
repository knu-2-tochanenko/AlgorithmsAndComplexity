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
		1	- The first value is begger than second
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
			if (node->parent == node->parent->parent->left) {
				sub = node->parent->parent->right;
				if (sub->color == red) {
					node->parent->color = black;
					sub->color = black;
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
				if (sub == nil || sub == NULL)
					return;
				if (sub->color == red) {
					node->parent->color = black;
					sub->color = black;
					node->parent->parent->color = red;
					node = node->parent->parent;
				}
				else {
					if (node == node->parent->left) {
						node = node->parent;
						rotateLeft(iteration, node);
					}
					node->parent->color = black;
					node->parent->parent->color = red;
					rotateRight(iteration, node->parent->parent);
				}
			}
		}
		this->iterations[iteration]->color = black;
	}

	/***
		Method which is used to go through the whole tree
	//*/
	Node* getElement(Node* node, Product* product) {
		int compareResult = compare(node->key, product);
		if (compareResult == 0)
			return node;
		else if (compareResult > 0 && node->left != nil)
			return getElement(node->left, product);
		else if (compareResult < 0 && node->right != nil)
			return getElement(node->right, product);
		else
			return new Node(black, false, new Product("UNDEFINED", 0, 0.0, 0.0), nil, nil, nil);
	}

	/***
		Displays individual node (using fancy colors) (and fancy structure)
	//*/
	void displayNodeFancy(Node* node, int tabs, int colorMode) {
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, colorMode);
		if (node->right != nil)
			displayNodeFancy(node->right, tabs + 1, colorMode == 15 ? 9 : colorMode + 1);
		else cout << endl;
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
		else cout << endl;
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
	void copySons(Node* copyTo, Node* copyFrom) {
		if (copyFrom->left != nil) {
			copyTo = new Node(copyFrom->left);
			copySons(copyTo->left, copyFrom->left);
		}
		if (copyFrom->right != nil) {
			copyTo = new Node(copyFrom->right);
			copySons(copyTo->right, copyFrom->right);
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

		//	Find place to insert new node
		Node *sub = nil, *point = this->iterations[iteration];
		while (point != nil) {
			sub = point;
			if (compare(node->key, point->key) < 0)
				point = point->left;
			else
				point = point->right;
		}

		/***
			Now we need to copy current node (sub),
			all of it's sons and it's father, grandfather,
			and other grand-grand fathers to the head of the tree
		//*/

		//	Copy nodes from current node to the head
		Node* NEWCurrentNode = new Node(sub);
		Node* NEWcopiedNode = NEWCurrentNode;
		Node* nodeFromPreviousTree = sub;
		while (nodeFromPreviousTree->parent != nil) {
			NEWcopiedNode->parent = new Node(nodeFromPreviousTree->parent);
			
			//	Connect parent to current node
			if (nodeFromPreviousTree->parent->right == nodeFromPreviousTree)
				NEWcopiedNode->parent->right = NEWcopiedNode;
			else
				NEWcopiedNode->parent->left = NEWcopiedNode;

			NEWcopiedNode = NEWcopiedNode->parent;
			nodeFromPreviousTree = nodeFromPreviousTree->parent;
		}

		Node* NEWhead = NEWcopiedNode;

		// Copy sons
		copySons(NEWCurrentNode, sub);

		//	Evertything is copied and we need to insert new node
		node->parent = NEWCurrentNode;

		if (NEWCurrentNode == nil)
			NEWhead = node;
		else if (compare(node->key, NEWCurrentNode->key) < 0)
			NEWCurrentNode->left = node;
		else
			NEWCurrentNode->right = node;

		node->left = nil;
		node->right = nil;

		this->iterations.push_back(NEWhead);
		addNormalize(iteration + 1, node);
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
		return getElement(this->iterations[iteration], product);
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
		cout << "ITERATION " << iteration << endl;
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		displayNodeFancy(this->iterations[iteration], 4, 9);
		SetConsoleTextAttribute(hConsole, 15);
	}

	int getNumberOfIterations() {
		return this->iterations.size();
	}
};