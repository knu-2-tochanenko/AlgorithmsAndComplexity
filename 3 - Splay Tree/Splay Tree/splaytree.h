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
	Node* head = new Node(NULL, NULL, NULL, NULL);
	int mode = 0;		//	1	:	Name
						//	2	:	Days till expired
						//	3	:	Weight
						//	0..	:	Price
	int treeSize = 0;

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
		if (sub != NULL) {
			sub->right = sub->left;
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
		if (sub != NULL)
			sub->left = node;
		node->parent = sub;
	}

	/***
		Actually rotates right
	//*/
	void rotateRight(Node *node) {
		Node *sub = node->left;
		if (sub != NULL) {
			node->left = sub->right;
			if (sub->right)
				sub->right->parent = node;
			sub->parent = node->parent;
		}
		if (!node->parent)
			head = sub;
		else if (node == node->parent->left)
			node->parent->left = sub;
		else
			node->parent->right = sub;
		if (sub != NULL)
			sub->right = node;
		node->parent = sub;
	}

	/***
		Normalizes tree
	//*/
	void splay(Node *node) {
		while (node->parent) {
			if (!node->parent->parent) {
				if (node->parent->left == node) rotateRight(node->parent);
				else rotateLeft(node->parent);
			}
			else if (node->parent->left == node && node->parent->parent->left == node->parent) {
				rotateRight(node->parent->parent);
				rotateRight(node->parent);
			}
			else if (node->parent->right == node && node->parent->parent->right == node->parent) {
				rotateLeft(node->parent->parent);
				rotateLeft(node->parent);
			}
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

	Node* subMinimum(Node *node) {
		while (node->left) node = node->left;
		return node;
	}

	Node* subMaximum(Node *node) {
		while (node->right) node = node->right;
		return node;
	}

public:
	/***
		You NEED to chose which mode to use
	//*/
	SplayTree(int mode) { this->mode = mode;  }

	/***
		Adds element to the tree
	//*/
	void addElement(Product *key) {
		Node *sub = head;
		Node *sub2 = NULL;

		while (sub != NULL) {
			sub2 = sub;
			if (compare(sub->key, key))
				sub = sub->right;
			else
				sub = sub->left;
		}

		sub = new Node(key, NULL, NULL, NULL);
		sub->parent = sub2;

		if (sub2 == NULL)
			head = sub;
		else if (compare(sub2->key, sub->key))
			sub2->right = sub;
		else
			sub2->left = sub;

		splay(sub);
		treeSize++;
	}

	/***
		Use this function to find element with specific
		values. Note, that key will be created in the "store" class
	//*/
	Product* getElement(Product* key) {
		Node *sub = head;
		while (sub != NULL) {
			if (compare(sub->key, key))
				sub = sub->right;
			else if (compare(key, sub->key))
				sub = sub->left;
			else
				return sub->key;
		}
		return nullptr;
	}

	/***
		Displays full tree
	//*/
	void displayTree() {
		// TODO : Write function
	}

	/***
		Returns minimum value in the tree
	//*/
	Product* minimum() {
		return subMinimum(head)->key;
	}
	/***
		Returns maximum value in the tree
	//*/
	Product* maximum() {
		return subMaximum(head)->key;
	}
};