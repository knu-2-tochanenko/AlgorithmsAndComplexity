#pragma once

#include "hashtable.h"
#include "datafile.h"

#include <iostream>
#include <vector>
#include <string>
#include <map>

using namespace std;

int MAX_NUMBER_OF_PRODUCTS = 3000;

struct Category {
    string name;
    HashTable* products;

	Category(string name) {
		this->name = name;
		this->products = new HashTable(MAX_NUMBER_OF_PRODUCTS);
	}

	void add(Product* product, bool displayDebug) {
		this->products->addElement(product, displayDebug);
	}
};

class Store {
private:
    vector<Category*> catalog;
	map<string, int> category;
	vector<string> categoriesList;
public:
	Store(string fileName, bool displayData, bool displayDebug) {
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
				->add(categoryProduct->product, displayDebug);
		}
	}

	void printProduct(string categoryName, string productName) {
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		int selectedCategory = category[categoryName];
		Product* selectedProduct = this->catalog[selectedCategory]->products->getElement(productName);
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
			cout << "$" << selectedProduct->cost << "\n";
			SetConsoleTextAttribute(hConsole, 10);
			cout << "------------------\n";
		}
		else {
			SetConsoleTextAttribute(hConsole, 12);
			cout << "There is no ";
			SetConsoleTextAttribute(hConsole, 15);
			cout << productName;
			SetConsoleTextAttribute(hConsole, 12);
			cout << " in ";
			SetConsoleTextAttribute(hConsole, 15);
			cout << categoryName << endl;
		}
	}
};
