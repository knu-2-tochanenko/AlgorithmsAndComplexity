#pragma once

#include "datafile.h"
#include <iostream>

using namespace std;

/***
	Struct which has all needed fields for Order Statistics Tree
//*/
struct Node {
	Node* parent = NULL;

	vector<Node*> children;

	bool lefaa = false;
	vector<Product*> products;

	Node() = default;
	Node(Product product, Node* parent, bool lefaa) {
		this->products.push_back(&product);
		this->parent = parent;
		this->lefaa = lefaa;
	}
};

class BPlusTree {
private:
	int treeLimiter = 4;
	int mode = 0;
	Node* head;
	int minTree = 0;

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

	void printSingle(Product* product) {
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

	/***
		Displays individual node (using fancy colors) (and fancy structure)
	//*/
	void displayNodeFancy(Node* node, int tabs, int colorMode) {
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		for (int i = 0; i < node->children.size(); i++) {
			SetConsoleTextAttribute(hConsole, colorMode);
			displayNodeFancy(node->children[i], tabs + 1, colorMode == 15 ? 9 : colorMode + 1);
			SetConsoleTextAttribute(hConsole, colorMode);
			cout << '\n';
			for (int j = 0; j < tabs; j++)
				cout << "\t";
			if (i + 1 < node->children.size())
				printSingle(node->products[i]);
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

	/***
		Finds position of element in leaf
	//*/
	int binarySearch(Node* node, Product* value) {
		int left = -1, right = node->products.size();
		while (right - left > 1) {
			int middle = (right + left) / 2;
			if (compare(node->products[middle], value) == 1)
				right = middle;
			else
				left = middle;
		}
		return left;
	}

	Node* findElement(Node* node, Product* product) {
		if (node == NULL || node->lefaa)
			return node;
		int position = binarySearch(node, product);
		return
			findElement(node->children[position + 1], product);
	}

	/***
		Splits tree if there are more than t - 1 keys
	//*/
	void splitNode(Node* node, Product* value) {
		Node* father = node->parent;
		if (father == NULL) {
			head = new Node();
			node->parent = head;
			father = head;
		}
		if (father->children.size() == treeLimiter)
			splitNode(father, value);
		father = node->parent;
		int middle = treeLimiter / 2 - 1;
		if (node->lefaa)
			middle++;
		Node* newNode = new Node();
		if (father->products.size() > 0) {
			int newPosition = binarySearch(father, node->products[middle]);
			father->products.push_back(NULL);
			father->children.push_back(NULL);
			for (int i = father->products.size() - 1; i > newPosition + 1; i--) {
				father->products[i] = father->products[i - 1];
				father->children[i + 1] = father->children[i];
			}
			father->products[newPosition + 1] = node->products[middle];
			father->children[newPosition + 2] = newNode;
		}
		else {
			father->products.push_back(node->products[middle]);
			father->children.push_back(node);
			father->children.push_back(newNode);
		}
		newNode->parent = father;

		if (node->lefaa) {
			newNode->lefaa = true;
			for (int i = middle; i < treeLimiter; i++)
				newNode->products.push_back(node->products[i]);
			for (int i = treeLimiter - 1; i >= middle; i--)
				node->products.pop_back();
		}
		else {
			for (int i = middle + 1; i < treeLimiter - 1; i++) {
				newNode->products.push_back(node->products[i]);
				node->children[i]->parent = newNode;
				newNode->children.push_back(node->children[i]);
			}
			node->children[treeLimiter - 1]->parent = newNode;
			newNode->children.push_back(node->children[treeLimiter - 1]);
			for (int i = treeLimiter - 2; i >= middle; i--) {
				node->products.pop_back();
				node->children.pop_back();
			}
		}
	}

public:
	BPlusTree(int mode) {
		this->mode = mode;
	}

	/***
		Method which is used to get element
	//*/
	Product* getElement(Product* product) {
		Node* node = findElement(head, product);
		if (node != NULL) {
			int needPos = binarySearch(node, product);
			if (needPos < 0)
				return NULL;
			if (compare(node->products[needPos], product) == 0)
				return node->products[needPos];
			else
				return false;
		}
		return NULL;
	}

	/***
		Method which is used to add new node to the tree
	//*/
	void addElement(Product* product) {
		if (head == NULL) {
			head = new Node();
			head->products.push_back(product);
			head->lefaa = true;
			return;
		}

		Node* leafNode = findElement(head, product);
		if (leafNode->products.size() == treeLimiter) {
			splitNode(leafNode, product);
			leafNode = findElement(head, product);
		}
		int position = binarySearch(leafNode, product);
		leafNode->products.push_back(NULL);

		for (int i = leafNode->products.size() - 1; i > position + 1; i--)
			leafNode->products[i] = leafNode->products[i - 1];
		leafNode->products[position + 1] = product;
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