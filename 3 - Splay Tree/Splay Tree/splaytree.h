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

	int treeSize = 0;

	//	TODO :: Create compare funciton
	int compare(product* )

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

	void replace(Node *node, Node *second) {
		if (!node->parent)
			head = second;
		else if (node == node->parent->left)
			node->parent->left = second;
		else
			node->parent->right = second;
		if (second != NULL)
			second->parent = node->parent;
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
	SplayTree() { }

	void addElement(Product *key) {
		Node *sub = head;
		Node *sub2 = NULL;

		while (sub) {
			sub2 = sub;
			if (sub->price < key->price)
				sub = sub->right;
			else
				sub = sub->left;
		}

		sub = new Node(key, NULL, NULL, NULL);
		sub->parent = sub2;

		if (sub2 == NULL)
			head = sub;
		else if (sub2->key->price < sub->key->price)
			sub2->right = sub;
		else
			sub2->left = sub;

		splay(sub);
		treeSize++;
	}

	Node* getElement(Product* key) {
		Node *sub = head;
		while (sub != NULL) {
			if (sub->key->price < key->price)
				sub = sub->right;
			else if (key->price < sub->key->price)
				sub = sub->left;
			else
				return sub;
		}
		return nullptr;
	}

	void erase(Product* key) {
		Node *z = getElement(key);
		if (!z) return;

		splay(z);

		if (!z->left)
			replace(z, z->right);
		else if (!z->right)
			replace(z, z->left);
		else {
			Node *y = subMinimum(z->right);
			if (y->parent != z) {
				replace(y, y->right);
				y->right = z->right;
				y->right->parent = y;
			}
			replace(z, y);
			y->left = z->left;
			y->left->parent = y;
		}

		delete z;
		treeSize--;
	}

	/***
		Displays full tree
	//*/
	void displayTree() {
		
	}

	Product* minimum() {
		return subMinimum(head)->key;
	}

	Product* maximum() {
		return subMaximum(head)->key;
	}
};