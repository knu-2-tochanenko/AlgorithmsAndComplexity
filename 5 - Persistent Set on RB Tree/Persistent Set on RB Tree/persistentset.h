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

	class RBTree {
	private:
		/***
			nil - element which is a child for every leaf and a parent for head
			head - a head of the tree
		//*/
		Node* nil = new Node(black, true, NULL, NULL, NULL, NULL);
		Node* head = new Node(black, false, NULL, nil, nil, nil);

		int mode = 0;		//	1	:	Name
							//	2	:	Days till expired
							//	3	:	Weight
							//	0..	:	Price

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

		/***
			Actually, rotates right
		//*/
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

		/****
			Function which saves current tree with new node as next iteration
		//*/
		void saveNext(Node * node) {
			if (node == head)
				return;
			Node *parent = node->parent;
			if (node->parent == nil)
				return;
			Node* newParent = new Node(node->parent);
			if (parent->left) {
				parent->left->parent = newParent;
				newParent->left = parent->left;
			}
			if (parent->right) {
				parent->right->parent = newParent;
				newParent->right = parent->right;
			}

			if (node == parent->left)
				parent->left = nil;
			else if (node == parent->right)
				parent->right = nil;

			while (parent != head) {
				Node *grandparent = parent->parent;
				Node *newGrandparent = new Node(grandparent);

				if (grandparent->left == parent) {
					newGrandparent->left = newParent;
					if (grandparent->right != nil) {
						newGrandparent->right = grandparent->right;
						newGrandparent->right->parent = newGrandparent;
					}
				}

				else {
					newGrandparent->right = newParent;
					if (grandparent->left != nil) {
						newGrandparent->left = grandparent->left;
						newGrandparent->left->parent = newGrandparent;
					}
				}

				newParent->parent = newGrandparent;
				parent = parent->parent;
				newParent = newParent->parent;
			}
			this->head = newParent;
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
			Displays individual node (using fancy colors) (and fancy structure)
		//*/
		void displayNodeFancy(Node* node, int tabs, int colorMode) {
			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hConsole, colorMode);
			if (node->right != NULL)
				displayNodeFancy(node->right, tabs + 1, colorMode == 15 ? 9 : colorMode + 1);
			else cout << endl;
			SetConsoleTextAttribute(hConsole, colorMode);
			for (int i = 0; i < tabs; i++)
				cout << "\t";
			cout << node->key->name << " - ";

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

			if (node->left != NULL)
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

	vector<RBTree*> iterations;
	int mode = 0;		//	1	:	Name
						//	2	:	Days till expired
						//	3	:	Weight
						//	0..	:	Price
public:

	PersistentSet(int mode) {
		this->mode = mode;
	}

	/***
		Displays full tree
	//*/
	void displayTree() {

	}

	/***
		Method which is used to get element with specific key (by rank)
	//*/
	Product* getElement(int iteration, Product* element) {

	}

	/***
		Methods which is used to add new node to the tree
	//*/
	void addElement(Product* product) {

	}
};