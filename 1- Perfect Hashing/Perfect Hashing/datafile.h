#pragma once

#include "hash.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

//	Temporary structure to get values
struct Category_Product {
	string category;
	Product* product;
};

class DataFile {
private:
	ifstream file;
	vector<Category_Product*> products;
	vector<string> categories;

	int pointer;

	/***
		Breaks string into words
	//*/
	vector<string> breakIntoWords(string str) {
		vector<string> res;
		string substr;
		for (unsigned int i = 0; i < str.length(); i++) {
			if (str[i] == ',') {
				if (substr != "") {
					res.push_back(substr);
					substr = "";
				}
			}
			else if (str[i] == '\n') {
				if (substr != "")
					res.push_back(substr);
			}
			else {
				substr += str[i];
			}
		}
		if (substr != "")
			res.push_back(substr);
		return res;
	}

	/***
		Returns Category_Product from string, which includes category name
		and product properties.
	//*/
	Category_Product* getProduct(string str, bool displayData) {
		pointer = 0;
		Category_Product* product = new Category_Product();
		product->product = new Product();

		vector<string> values = breakIntoWords(str);

		if (displayData) {
			for (unsigned int i = 0; i < values.size(); i++)
				cout << values[i] << " ";
			cout << endl;
		}
		if (values.size() == 5) {
			product->category = values[0];

			//	Add category to category list
			if (find(this->categories.begin(), this->categories.end(), product->category) == this->categories.end())
				this->categories.push_back(product->category);

			product->product->name = values[1];
			product->product->daysTillExpired = stoi(values[2]);
			product->product->weight = stod(values[3]);
			product->product->cost = stod(values[4]);
		}
		else {
			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hConsole, 12);
			cerr << "E001 : PROBLEM IN PARSING STRING\n";
			SetConsoleTextAttribute(hConsole, 15);
		}
		return product;
	}

public:
	DataFile(string fileName, bool displayData) {
		this->file.open(fileName, ifstream::in);

		if (file.fail()) {
			cout << "ERROR!\n";
			system("pause");
			exit(0);
		}

		string line;
		getline(file, line);			//	Get header

		Category_Product* currentProduct;
		while (getline(file, line)) {
			currentProduct = getProduct(line, displayData);
			if (currentProduct->category != "\n")
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

	vector<string> getCategories() {
		return this->categories;
	}

	/***
		Returns next product in this->products vector
	//*/
	Category_Product* getProduct(int number) {
		return this->products[number];
	}
};
