#pragma once

#include "hashtable.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct Product {
	string name;
	time_t shelfTime;
	double weight;
	double cost;
};

struct Category {
	string name;
	HashTable<Product> products;
};

class Store {
private:
	HashTable<Category> categories;
public:
	
};

string to_string(Product& product) {
	return product.name;
}

string to_string(Category& category) {
	return category.name;
}