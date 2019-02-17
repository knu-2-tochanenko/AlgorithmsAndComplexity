#pragma once

#include "rbtree.h"
#include "datafile.h"

#include <iostream>
#include <vector>
#include <string>
#include <map>

using namespace std;

int MAX_NUMBER_OF_PRODUCTS = 3000;

struct Category {
	string name;
	RBTree* products;

	Category(string name) {
		this->name = name;
		this->products = new RBTree();
	}

	void add(Product* product) {
		this->products->addElement(product);
	}
};

class Store {
private:
	vector<Category*> catalog;
	map<string, int> category;
	vector<string> categoriesList;
public:
	Store(string fileName, bool displayData) {
		DataFile* file = new DataFile(fileName, displayData);
		Category_Product* categoryProduct;
		categoriesList = file->getCategories();

		//	Create map for easier work with categories
		//	& add all categories to catalog
		catalog.resize(categoriesList.size());
		for (unsigned int i = 0; i < categoriesList.size(); i++) {
			category[categoriesList[i]] = i;
			catalog[i] = new Category(categoriesList[i]);
		}

		unsigned int numberOfProducts = file->numberOfProducts();
		for (unsigned int i = 0; i < numberOfProducts; i++) {
			categoryProduct = file->getProduct(i);
			catalog[category[categoryProduct->category]]
				->add(categoryProduct->product);
		}
	}

	void printProduct(string categoryName, int rang) {
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		int selectedCategory = category[categoryName];
		if (rang == 0) {
			SetConsoleTextAttribute(hConsole, 12);
			cout << "The numeration is starts from 0!\n";
			SetConsoleTextAttribute(hConsole, 15);
			return;
		}
		Product* selectedProduct = this->catalog[selectedCategory]->products->getElement(rang);
		if ((selectedProduct != NULL) && (category.find(categoryName) != category.end())) {
			SetConsoleTextAttribute(hConsole, 10);
			cout << "------------------\nNAME     :  ";
			SetConsoleTextAttribute(hConsole, 11);
			cout << selectedProduct->name << "\n";
			SetConsoleTextAttribute(hConsole, 10);
			cout << "expdate  :  ";
			SetConsoleTextAttribute(hConsole, 15);
			cout << selectedProduct->daysTillExpired << "\n";
			SetConsoleTextAttribute(hConsole, 10);
			cout << "weight   :  ";
			SetConsoleTextAttribute(hConsole, 15);
			cout << selectedProduct->weight << " kg\n";
			SetConsoleTextAttribute(hConsole, 10);
			cout << "cost     :  ";
			SetConsoleTextAttribute(hConsole, 15);
			printf("$%.2f\n", selectedProduct->price);
			SetConsoleTextAttribute(hConsole, 10);
			cout << "------------------\n";
		}
		else {
			SetConsoleTextAttribute(hConsole, 12);
			cout << "The inputed number is too high!\n";
			SetConsoleTextAttribute(hConsole, 15);
		}
	}

	void printTrees() {
		for (int i = 0; i < catalog.size(); i++) {
			cout << catalog[i]->name << "\n";
			catalog[i]->products->displayTree();
			cout << "\n\n";
		}
	}
};
