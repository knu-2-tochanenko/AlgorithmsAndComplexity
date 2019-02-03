#pragma once

#include "hash.h"

#include <iostream>
#include <vector>
#include <string>

using namespace std;

struct Cell {
    int elements;
    vector<Product*> productsList;

    Product* operator[](int n) {
        if (n >= productsList.size())
            return NULL;
        return productsList[n];
    }
};

class HashTable {
private:
    vector<Cell*> table;        //	Main table of products
    int numberOfElements;       //	Number of elements in first list (does not contain
									//	the number of elements in sublists
public:
    /***
		Default constructor.
		Generates vector with initializes values.
	//*/
    HashTable(int numberOfElements) {
        this->numberOfElements = numberOfElements;
        this->table.resize(numberOfElements);
        for (int i = 0; i < numberOfElements; i++) {
            this->table[i] = new Cell();
            this->table[i]->elements = 0;
        }
    }

	/***
		Adds element to Hash Table.
	//*/
    void addElement(Product* element) {
        //  Get first hash
        unsigned int firstHashValue = firstHash(element->name, numberOfElements);
        Cell* cell = this->table[firstHashValue];

        //  Copy previous entries to separate list
        vector<Product*> tempList;
        for (int i = 0; i < cell->elements; i++)
            if (cell->productsList[i] != NULL)
                tempList.push_back(cell->productsList[i]);
		tempList.push_back(element);	//	Add new element
        
        //  Clear main list and resize it to fit n*n values
        cell->productsList.clear();
        cell->elements++;
        int squareSize = cell->elements * cell->elements;
        cell->productsList.resize(squareSize);
        
        //  Generate second firstHashValue for each element
        unsigned int secondHashValue;
        for (int i = 0; i < tempList.size(); i++) {
            secondHashValue = secondHash(tempList[i]->name, squareSize);
            cell->productsList[secondHashValue] = tempList[i];
        }
    }

    /***
		Checks if there is an element with given name
	//*/
    bool hasElement(string name) {
        unsigned int firstHashValue, secondHashValue;
        firstHashValue = firstHash(name, numberOfElements);
        if (this->table[firstHashValue] != NULL) {
            Cell* cell = this->table[firstHashValue];
            secondHashValue = secondHash(name, cell->elements * cell->elements);
            if (cell->productsList[secondHashValue] != NULL)
                return true;
        }
        else
            return false;
    }

	/***
		Gets element from HashTable.
	//*/
    Product* getElement(string name) {
		unsigned int firstHashValue, secondHashValue;
		firstHashValue = firstHash(name, numberOfElements);
		if (this->table[firstHashValue] != NULL) {
			Cell* cell = this->table[firstHashValue];
			secondHashValue = secondHash(name, cell->elements * cell->elements);
			if ((cell->elements != 0) && (cell->productsList[secondHashValue] != NULL))
				return cell->productsList[secondHashValue];
		}
		else
			return NULL;
    }
};
