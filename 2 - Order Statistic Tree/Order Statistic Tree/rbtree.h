#pragma once

#include "datafile.h"

#include <iostream>

using namespace std;

enum Color { black, red };

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
	Node(Color color, bool isNil, Product* key, Node* left, Node* right) {
		this->color = color;
		this->isNil = isNil;
		this->key = key;
		this->left = left;
		this->right = right;
	}
};

class RBTree {
private:

	Node* head;
	Node* nil = new Node(black, true, 0, NULL, NULL);

	void rotateLeft(Node* node) {
		Node* sub = node->right;
		node->right = sub->left;

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

	void rotateRight(Node* node) {

	}

	void deleteNormalize() {

	}

	void addNormalize() {

	}

public:
	// TODO :: getElement()
	// TODO :: addElement()
};