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

	Category(string name) {
		this->name = name;
		this->products = new HashTable<Product*>(MAX_NUMBER_OF_PRODUCTS);
	}

	void add(Product* product) {
		this->products->addElement(product);
	}
};

string to_string(Product product) {
	return product.name;
}

string to_string(Category category) {
	return category.name;
}

string to_string(Product* product) {
	return product->name;
}

string to_string(Category* category) {
	return category->name;
}

class Store {
private:
	HashTable<Category*>* categories;
	vector<string> categoriesNames;
public:
	Store(string fileName) {
		DataFile* file = new DataFile(fileName);
		categories = new HashTable<Category*>(MAX_NUMBER_OF_CATEGORIES);

		int numberOfProducts = file->numberOfProducts();
		Category_Product* currentProduct;
		Product* productValues;
		Category* currentCategory;
		for (int i = 0; i < numberOfProducts; i++) {
			currentProduct = file->nextProduct();
			productValues = currentProduct->product;

			//	Add product to category if category exists
			if (find(categoriesNames.begin(), categoriesNames.end(), currentProduct->category) != categoriesNames.end()) {
				currentCategory = this->categories->getElement(currentProduct->category);
				currentCategory->add(productValues);
			}
			//	Add element to new category
			else {
				currentCategory = new Category(currentProduct->category);
				currentCategory->add(productValues);
				this->categories->addElement(currentCategory);
				categoriesNames.push_back(currentProduct->category);
			}
		}

		for (int i = 0; i < this->categoriesNames.size(); i++) {
			this->categories->getElement(this->categoriesNames[i])->products->normalize();
		}
		this->categories->normalize();
	}

	void printCategory(string category) {
		if (this->categories->hasElement(category))
			cout << "true\n";
		else
			cout << "false\n";
	}
};