#pragma once

#include "hashtable.h"
#include "filebase.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

int MAX_NUMBER_OF_CATEGORIES = 129;
int MAX_NUMBER_OF_PRODUCTS = 129;

struct Category {
	string name;
	HashTable<Product*>* products;

	Category(string name, vector<Product*> productsList) {

	}
};

class Store {
private:
	HashTable<Category*>* categories;
public:
	Store(string fileName) {
		DataFile* file = new DataFile(fileName);
		categories = new HashTable<Category*>(MAX_NUMBER_OF_CATEGORIES);

		//	TODO Write function
	}
};

string to_string(Product& product) {
	return product.name;
}

string to_string(Category& category) {
	return category.name;
}