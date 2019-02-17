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
	Node(Color color, bool isNil, Product* key, Node* left, Node* right, Node* parent) {
		this->color = color;
		this->isNil = isNil;
		this->key = key;
		this->left = left;
		this->right = right;
		this->parent = parent;
	}
};

class RBTree {
private:

	Node* nil = new Node(black, true, NULL, NULL, NULL, NULL);
	Node* head = new Node(black, false, NULL, NULL, NULL, nil);

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
		Node* sub = node->left;
		node->left = sub->right;

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

public:

	// TODO :: getElement()
	void addElement(Product* product) {
		Node* node = new Node(red, false, product, NULL, NULL, NULL);
		
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
	}
};