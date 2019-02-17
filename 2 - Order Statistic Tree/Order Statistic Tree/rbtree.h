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

	int size;

	Node* left;
	Node* right;
	Node* parent;

	double price() {
		return this->key->price;
	}

	Node() = default;
	Node(Color color, bool isNil, Product* key, Node* left, Node* right, Node* parent) {
		this->color = color;
		this->isNil = isNil;
		this->key = key;
		this->left = left;
		this->right = right;
		this->parent = parent;

		size = 1;
	}
};

class RBTree {
private:
	/***
		nil - element which is a child for every leaf and a parent for head
		head - a head of the tree
	//*/
	Node* nil = new Node(black, true, NULL, NULL, NULL, NULL);
	Node* head = new Node(black, false, NULL, nil, nil, nil);

	/***
		Actually, rotates left
	//*/
	void rotateLeft(Node* node) {
		Node* sub = node->right;
		node->right = sub->left;

		sub->size = node->size;
		node->size = node->left->size + node->right->size + 1;

		if (sub->left != nil)
			sub->left->parent = node;

		sub->parent = node->parent;

		if (node->parent == nil)
			head = sub;
		else if (node == node->parent->left)
			node->parent->left = sub;
		else node->parent->right = sub;

		sub->left = node;
		node->parent = sub;
	}

	/***
		Actually, rotates right
	//*/
	void rotateRight(Node* node) {
		Node* sub = node->left;
		node->left = sub->right;

		sub->size = node->size;
		node->size = node->left->size + node->right->size + 1;

		if (sub->right != nil)
			sub->right->parent = node;
		
		sub->parent = node->parent;

		if (node->parent == nil)
			head = sub;
		else if (node == node->parent->right)
			node->parent->right = sub;
		else node->parent->left = sub;

		sub->right = node;
		node->parent = sub;
	}

	/***
		The method which NEEDS to be executed after EACH element adding
	//*/
	void addNormalize(Node * node) {
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
						rotateLeft(node);
					}
					node->parent->color = black;
					node->parent->parent->color = red;
					rotateRight(node->parent->parent);
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
						rotateLeft(node);
					}
					node->parent->color = black;
					node->parent->parent->color = red;
					rotateRight(node->parent->parent);
				}
			}
		}
		head->color = black;
	}

	/***
		Method which is used to go through the whole tree
	//*/
	Node* getElement(Node* node, int rang) {
		if (node == nil)
			return node;
		int sub = (node->left->size + 1);
		if (rang == sub)
			return node;
		else if (rang < sub)
			return getElement(node->left, rang);
		else
			return getElement(node->right, rang - sub);
	}

	/***
		Displays individual node (using fancy colors)
	//*/
	void displayNode(Node* node, int colorMode) {
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, colorMode);
		printf("%.2f", node->price());
		cout << "-" << node->size;
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

public:
	/***
		Constructor. Setting nil's is CRUICIAL for this tree to work.
		Note, that nil->size is NEEDS to be 0
	//*/
	RBTree() {
		nil->left = nil;
		nil->right = nil;
		nil->parent = nil;
		nil->size = 0;
	}

	/***
		Method which is used to get element with specific key (by rank)
	//*/
	Product* getElement(int rang) {
		return getElement(head, rang)->key;
	}

	/***
		Methods which is used to add new node to the tree
	//*/
	void addElement(Product* product) {
		Node* node = new Node(red, false, product, nil, nil, nil);

		if (head->key == NULL) {
			head = node;
			return;
		}
		
		Node *sub = nil, *point = head;
		while (point != nil) {
			sub = point;
			if (node->price() < point->price())
				point = point->left;
			else
				point = point->right;
		}

		node->parent = sub;

		if (sub == nil)
			head = node;
		else if (node->price() < sub->price())
			sub->left = node;
		else
			sub->right = node;

		node->left = nil;
		node->right = nil;

		addNormalize(node);

		Node* added = node->parent;
		while (added != nil) {
			added->size++;
			added = added->parent;
		}
	}

	/***
		Displays full tree
	//*/
	void displayTree() {
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		displayNode(head, 9);
		SetConsoleTextAttribute(hConsole, 15);
	}
};