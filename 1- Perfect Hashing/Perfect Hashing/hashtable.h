#pragma once

#include "hashing.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

template <typename T>
class HashTable {
private:
	vector<Cell<T>*> table;			//	Main table
	int numberOfElements;			//	Number of elements in first list (does not contain
									//	the number of elements in sublists

public:
	/***
		Default constructor.
		Generates vector with initializes values.
	//*/
	HashTable(int numberOfElements) {
		this->numberOfElements = numberOfElements;
		this->table.reserve(numberOfElements);
		for (int i = 0; i < numberOfElements; i++)
			this->table.at(i)->elements = 0;
	}

	/***
		Adds element to Hash Table.
		Does NOT generates second hash. Call normalize()
		to create perfect hashing.
	//*/
	void addElement(T element) {
		unsigned int hash = firstHash(to_string(element), numberOfElements);
		this->table.at(hash)->elements++;
		this->table.at(hash)->elementsList.add(element);
	}

	/***
		Gets element from HashTable. Return type is specified
		when initializing HashTable.
	//*/
	T getElement(string element) {
		int firstHashValue = firstHash(element, numberOfElements);
		Cell<T>* currentCell;
		currentCell = this->table[firstHashValue];
		int secondHashValue = secondHash(element, currentCell->elements);
		return currentCell[secondHashValue];
	}

	bool hasElement(string element) {
		int firstHashValue = firstHash(element, numberOfElements);
		if (this->table.at(firstHash) == NULL)
			return false;
		Cell<T>* currentCell;
		currentCell = this->table[firstHashValue];
		int secondHashValue = secondHash(element, currentCell->elements);
		if (currentCell[secondHash] == NULL)
			return false;
		return true;
	}

	//	CALL THIS FUNCTION TO GENERATE SECOND HASHES!
	void normalize() {
		vector<T> subElementsList;	//	List for temporary list
		Cell<T>* currentCell;			//	Pointer to current cell
		for (int i = 0; i < numberOfElements; i++) {
			subElementsList.clear();
			currentCell = this->table[i];
			subElementsList = currentCell->elementsList;
			currentCell->elementsList.clear();
			currentCell->elementsList.reserve(currentCell->elements);

			//	Generate second hashes
			for (int j = 0; j < currentCell->elements; j++) {
				currentCell.elementsList[
					secondHash(to_string(subElementsList[j]), currentCell->elements)]
					= subElementsList[j];
			}
		}
	}
};