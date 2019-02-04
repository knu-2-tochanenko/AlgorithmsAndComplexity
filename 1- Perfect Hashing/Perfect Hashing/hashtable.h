#pragma once

#include "hash.h"

#include <iostream>
#include <vector>
#include <string>
#include <windows.h>

using namespace std;

int prime[58] = { 17, 19, 23, 29, 31, 37, 41, 43, 47,  53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 283, 293, 307, 311 };

struct Cell {
    int elements;
	int multiplier;
    vector<Product*> productsList;

    Product* operator[](unsigned int n) {
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
    void addElement(Product* element, bool displayDebug) {
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        //  Get first hash
        unsigned int firstHashValue = firstHash(element->name, numberOfElements);
        Cell* cell = this->table[firstHashValue];

        //  Copy previous entries to separate list
        vector<Product*> tempList;
        for (int i = 0; i < cell->elements; i++)
            if (cell->productsList[i] != NULL)
                tempList.push_back(cell->productsList[i]);
		tempList.push_back(element);	//	Add new element
        
        //  Additional values for new list
        cell->elements++;
        int squareSize = cell->elements * cell->elements;
        
        //  Generate second firstHashValue for each element
		bool hasCollision = true;
		bool hadCollision = false;
		//	Debug information
		int numberOfIterations = 0;
		int currentPrime = 0;
		while (hasCollision) {
			hasCollision = false;
			//  Clear main list and resize it to fit n*n values
			cell->productsList.clear();
			cell->productsList.resize(squareSize);

			//	Attempting to use second hash
			unsigned int secondHashValue;
			for (unsigned int i = 0; i < tempList.size(); i++) {
				secondHashValue = secondHash(tempList[i]->name, squareSize, prime[currentPrime]);
				if (cell->productsList[secondHashValue] != NULL) {
					//	Check another prime number
					currentPrime++;
					numberOfIterations++;
					//	Display debug information
					if (displayDebug) {
						SetConsoleTextAttribute(hConsole, 12);
						cout << "COLLISION IN   ";
						cout << tempList[i]->name << "   AND   " << cell->productsList[secondHashValue]->name;
						SetConsoleTextAttribute(hConsole, 15);
						cout << "  Attempting value " << prime[currentPrime] << endl;
					}
					hasCollision = true;
					hadCollision = true;
					break;
				}
				cell->productsList[secondHashValue] = tempList[i];
			}
			if (!hasCollision && hadCollision && displayDebug) {
				SetConsoleTextAttribute(hConsole, 14);
				cout << "Number of takes : " << numberOfIterations << endl;
				SetConsoleTextAttribute(hConsole, 15);
			}
        }
		cell->multiplier = prime[currentPrime];
    }

    /***
		Checks if there is an element with given name
	//*/
    bool hasElement(string name) {
        unsigned int firstHashValue, secondHashValue;
        firstHashValue = firstHash(name, numberOfElements);
        if (this->table[firstHashValue] != NULL) {
            Cell* cell = this->table[firstHashValue];
            secondHashValue = secondHash(name, cell->elements * cell->elements, cell->multiplier);
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
			secondHashValue = secondHash(name, cell->elements * cell->elements, cell->multiplier);
			if ((cell->elements != 0) && (cell->productsList[secondHashValue] != NULL))
				return cell->productsList[secondHashValue];
		}
		else
			return NULL;
		return NULL;
    }
};
