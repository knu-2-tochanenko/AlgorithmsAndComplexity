#pragma once

#include "datafile.h"
#include <iostream>

using namespace std;

/***
	Struct which has all needed fields for Splay Tree
//*/
struct Node {
	Product* key;
	Node* left;
	Node* right;
	Node* parent;

	double price() {
		return this->key->price;
	}

	Node() = default;
	Node(Product* key, Node* left, Node* right, Node* parent) {
		this->key = key;
		this->left = left;
		this->right = right;
		this->parent = parent;
	}
};

class SplayTree {
private:
	Node* head = NULL;
	int mode = 0;		//	1	:	Name
						//	2	:	Days till expired
						//	3	:	Weight
						//	0..	:	Price

	/***
		Compares two elements using specific mode
	//*/
	bool compare(Product* x, Product* y) {
		if (x == NULL || y == NULL)
			return false;
		switch (mode) {
		case 1:
			return (x->name < y->name);
		case 2:
			return (x->daysTillExpired < y->daysTillExpired);
		case 3:
			return (x->weight < y->weight);
		default:
			return (x->price < y->price);
		}
	}

	/***
		Rotates left
	//*/
	void rotateLeft(Node *node) {
		Node *sub = node->right;
		if (sub) {
			node->right = sub->left;
			if (sub->left)
				sub->left->parent = node;
			sub->parent = node->parent;
		}

		if (!node->parent)
			head = sub;
		else if (node == node->parent->left)
			node->parent->left = sub;
		else
			node->parent->right = sub;
		if (sub)
			sub->left = node;
		node->parent = sub;
	}

	/***
		Actually rotates right
	//*/
	void rotateRight(Node *node) {
		Node *sub = node->left;
		if (sub) {
			node->left = sub->right;
			if (sub->right)
				sub->right->parent = node;
			sub->parent = node->parent;
		}
		if (!node->parent) head = sub;
		else if (node == node->parent->left)
			node->parent->left = sub;
		else
			node->parent->right = sub;
		if (sub)
			sub->right = node;
		node->parent = sub;
	}

	/***
		Normalizes tree
	//*/
	void splay(Node *node) {
		while (node->parent) {
			// Zig
			if (!node->parent->parent) {
				if (node->parent->left == node)
					rotateRight(node->parent);
				else
					rotateLeft(node->parent);
			}
			// Zig-Zig
			else if (node->parent->left == node && node->parent->parent->left == node->parent) {
				rotateRight(node->parent->parent);
				rotateRight(node->parent);
			}
			else if (node->parent->right == node && node->parent->parent->right == node->parent) {
				rotateLeft(node->parent->parent);
				rotateLeft(node->parent);
			}
			// Zig-Zag
			else if (node->parent->left == node && node->parent->parent->right == node->parent) {
				rotateRight(node->parent);
				rotateLeft(node->parent);
			}
			else {
				rotateLeft(node->parent);
				rotateRight(node->parent);
			}
		}
	}

	Node* minimumSubtree(Node *node) {
		while (node->left)
			node = node->left;
		return node;
	}

	Node* maximumSubtree(Node *node) {
		while (node->right)
			node = node->right;
		return node;
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
		if (node->left != nullptr || node->right != nullptr)
			cout << " { ";
		if (node->left != nullptr)
			displayNode(mode, node->left, colorMode == 15 ? 9 : colorMode + 1);
		SetConsoleTextAttribute(hConsole, colorMode);
		if (node->left != nullptr || node->right != nullptr)
			cout << " | ";
		if (node->right != nullptr)
			displayNode(mode, node->right, colorMode == 15 ? 9 : colorMode + 1);
		SetConsoleTextAttribute(hConsole, colorMode);
		if (node->left != nullptr || node->right != nullptr)
			cout << " } ";
	}

public:
	/***
		You NEED to chose which mode to use
	//*/
	SplayTree(int mode) { this->mode = mode;  }

	/***
		Adds element to the tree
	//*/
	void addElement(Product *node) {
		Node *current = head;
		Node *subParent = nullptr;

		while (current) {
			subParent = current;
			if (compare(current->key, node))
				current = current->right;
			else
				current = current->left;
		}

		current = new Node(node, NULL, NULL, NULL);
		current->parent = subParent;

		if (!subParent)
			head = current;
		else if (compare(subParent->key, current->key))
			subParent->right = current;
		else
			subParent->left = current;

		splay(current);
	}

	/***
		Use this function to find element with specific
		values. Note, that key will be created in the "store" class
	//*/
	Node* getElement(Product* node) {
		Node *sub = head;
		while (sub) {
			if (compare(sub->key, node))
				sub = sub->right;
			else if (compare(node, sub->key))
				sub = sub->left;
			else {
				splay(sub);
				return sub;
			}
		}
		return nullptr;
	}

	/***
		Displays full tree
	//*/
	void displayTree(int mode) {
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		displayNode(mode, head, 9);
		SetConsoleTextAttribute(hConsole, 15);
	}

	Product* minimum() {
		return minimumSubtree(head)->key;
	}
	Product* maximum() {
		return maximumSubtree(head)->key;
	}
};