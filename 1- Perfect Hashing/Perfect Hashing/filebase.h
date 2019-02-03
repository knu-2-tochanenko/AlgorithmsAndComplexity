#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

struct Product {
	string name;			//	The name of product
	int daysTillExpired;	//	Number of days when product is fresh
	double weight;			//	[kg]	The regular weight of product
	double cost;			//	[$]		Recommended cost of product
};

//	Temporary structure to get values
struct Category_Product {
	string category;
	Product* product;
};

class DataFile {
private:
	ifstream file;
	vector<Category_Product*> products;

    int pointer = 0;
	
	/***
		Breaks string into words
	//*/
	vector<string> breakIntoWords(string str) {
		vector<string> res;
		string substr;
		for (int i = 0; i < str.length(); i++) {
			if (str[i] == '\t') {
				res.push_back(substr);
				substr = "";
			}
			else if (str[i] == '\n')
				res.push_back(substr);
			else {
				substr += str[i];
			}
		}
		return res;
	}

	/***
		Returns Category_Product from string, which includes category name
		and product properties.
	//*/
	Category_Product* getProduct(string str) {
		Category_Product* product = new Category_Product();
		product->product = new Product();

		vector<string> values = breakIntoWords(str);

		if (values.size() == 5) {
			product->category = values[0];
			product->product->name = values[1];
			product->product->daysTillExpired = stoi(values[2]);
			product->product->weight = stod(values[3]);
			product->product->cost = stod(values[4]);
		}
		else {
			cerr << "E001 : PROBLEM IN PARSING STRING\n";
		}
		return product;
	}
	
public:
	DataFile(string fileName) {
		this->file.open(fileName, ifstream::in);
		
		string line;
		getline(file, line);			//	Get dummy values (header)
		Category_Product* currentProduct;
		while (getline(file, line)) {
			currentProduct = getProduct(line);
			this->products.push_back(currentProduct);
		}
		this->file.close();
	}

	/***
		Returns the number of products in file.
		Useful in the combination of nextProduct method
	//*/
	int numberOfProducts() {
		return this->products.size();
	}

	/***
		Returns next product in this->products vector
	//*/
	Category_Product* nextProduct() {
		return this->products[pointer];
		pointer++;
	}
};