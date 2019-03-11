#pragma once

#include "splaytree.h"
#include "datafile.h"

#include <iostream>
#include <vector>
#include <string>
#include <map>

using namespace std;

int MAX_NUMBER_OF_PRODUCTS = 3000;

struct Category {
	string name;
	SplayTree* products;

	Category(string name, int mode) {
		this->name = name;
		this->products = new SplayTree(mode);
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
	Store(string fileName, int mode, bool displayData) {
		DataFile* file = new DataFile(fileName, displayData);
		Category_Product* categoryProduct;
		categoriesList = file->getCategories();

		//	Create map for easier work with categories
		//	& add all categories to catalog
		catalog.resize(categoriesList.size());
		for (unsigned int i = 0; i < categoriesList.size(); i++) {
			category[categoriesList[i]] = i;
			catalog[i] = new Category(categoriesList[i], mode);
		}

		unsigned int numberOfProducts = file->numberOfProducts();
		for (unsigned int i = 0; i < numberOfProducts; i++) {
			categoryProduct = file->getProduct(i);
			catalog[category[categoryProduct->category]]
				->add(categoryProduct->product);
		}
	}

	bool printProduct(string categoryName, Product* productToFind) {
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		int selectedCategory = category[categoryName];
		
		Node* res = this->catalog[selectedCategory]->products->getElement(productToFind);
		if (!res) {
			cout << "Oopsie\n";
			return false;
		}
		Product* selectedProduct = res->key;
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

		return true;
	}

	bool printMin(string categoryName) {
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		int selectedCategory = category[categoryName];

		Product* res = this->catalog[selectedCategory]->products->minimum();
		if ((res != NULL) && (category.find(categoryName) != category.end())) {
			SetConsoleTextAttribute(hConsole, 10);
			cout << "------------------\nNAME     :  ";
			SetConsoleTextAttribute(hConsole, 11);
			cout << res->name << "\n";
			SetConsoleTextAttribute(hConsole, 10);
			cout << "expdate  :  ";
			SetConsoleTextAttribute(hConsole, 15);
			cout << res->daysTillExpired << "\n";
			SetConsoleTextAttribute(hConsole, 10);
			cout << "weight   :  ";
			SetConsoleTextAttribute(hConsole, 15);
			cout << res->weight << " kg\n";
			SetConsoleTextAttribute(hConsole, 10);
			cout << "cost     :  ";
			SetConsoleTextAttribute(hConsole, 15);
			printf("$%.2f\n", res->price);
			SetConsoleTextAttribute(hConsole, 10);
			cout << "------------------\n";
		}
		else {
			SetConsoleTextAttribute(hConsole, 12);
			cout << "The inputed number is too high!\n";
			SetConsoleTextAttribute(hConsole, 15);
		}
		return true;
	}

	bool printMax(string categoryName) {
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		int selectedCategory = category[categoryName];

		Product* res = this->catalog[selectedCategory]->products->maximum();
		if ((res != NULL) && (category.find(categoryName) != category.end())) {
			SetConsoleTextAttribute(hConsole, 10);
			cout << "------------------\nNAME     :  ";
			SetConsoleTextAttribute(hConsole, 11);
			cout << res->name << "\n";
			SetConsoleTextAttribute(hConsole, 10);
			cout << "expdate  :  ";
			SetConsoleTextAttribute(hConsole, 15);
			cout << res->daysTillExpired << "\n";
			SetConsoleTextAttribute(hConsole, 10);
			cout << "weight   :  ";
			SetConsoleTextAttribute(hConsole, 15);
			cout << res->weight << " kg\n";
			SetConsoleTextAttribute(hConsole, 10);
			cout << "cost     :  ";
			SetConsoleTextAttribute(hConsole, 15);
			printf("$%.2f\n", res->price);
			SetConsoleTextAttribute(hConsole, 10);
			cout << "------------------\n";
		}
		else {
			SetConsoleTextAttribute(hConsole, 12);
			cout << "The inputed number is too high!\n";
			SetConsoleTextAttribute(hConsole, 15);
		}
		return true;
	}

	void printTrees(int mode) {
		for (int i = 0; i < catalog.size(); i++) {
			cout << catalog[i]->name << "\n";
			catalog[i]->products->displayTree(mode);
			cout << "\n\n";
		}
	}

	void printTree(string categoryName, int mode) {

		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		int selectedCategory = category[categoryName];

		if (category.find(categoryName) != category.end()) {
			cout << catalog[selectedCategory]->name << "\n";
			catalog[selectedCategory]->products->displayTree(mode);
			cout << "\n\n";
		}
	}
};
