#pragma once

#include "hashtable.h"
#include "datafile.h"

#include <iostream>
#include <vector>
#include <string>
#include <map>

using namespace std;

int MAX_NUMBER_OF_PRODUCTS = 15;

struct Category {
    string name;
    HashTable* products;

	Category(string name) {
		this->name = name;
		this->products = new HashTable(MAX_NUMBER_OF_PRODUCTS);
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
	Store(string fileName) {
		DataFile* file = new DataFile(fileName);
		Category_Product* categoryProduct;
		categoriesList = file->getCategories();

		//	Create map for easier work with categories
		//	& add all categories to catalog
		catalog.resize(categoriesList.size());
		for (int i = 0; i < categoriesList.size(); i++) {
			category[categoriesList[i]] = i;
			catalog[i] = new Category(categoriesList[i]);
		}

		int numberOfProducts = file->numberOfProducts();
		for (int i = 0; i < numberOfProducts; i++) {
			categoryProduct = file->getProduct(i);
			catalog[category[categoryProduct->category]]
				->add(categoryProduct->product);
		}
	}

	void printProduct(string categoryName, string productName) {
		int selectedCategory = category[categoryName];
		Product* selectedProduct = this->catalog[selectedCategory]->products->getElement(productName);
		if (selectedProduct != NULL) {
			cout << "\tNAME\t:\t" << selectedProduct->name << "\n";
			cout << "\texpdate\t:\t" << selectedProduct->daysTillExpired << "\n";
			cout << "\tweight\t:\t" << selectedProduct->weight << " kg\n";
			cout << "\tcost\t:\t$" << selectedProduct->cost << "\n";
		}
		else {
			cout << "There is no " << productName << " in " << categoryName << endl;
		}
	}
};
