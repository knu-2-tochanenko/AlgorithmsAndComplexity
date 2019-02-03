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
		this->table.resize(numberOfElements);
		for (int i = 0; i < numberOfElements; i++) {
			this->table[i] = new Cell<T>();
			this->table[i]->elements = 0;
		}
	}

	//	CALL THIS FUNCTION TO GENERATE SECOND HASHES!
	void normalize() {
		vector<T> subElementsList;	//	List for temporary list
		Cell<T>* currentCell;			//	Pointer to current cell
		for (int i = 0; i < numberOfElements; i++) {
			subElementsList.clear();
			currentCell = this->table[i];
			subElementsList.resize(currentCell->elements);
			for (int j = 0; j < currentCell->elementsList.size(); j++)
				if (currentCell->elementsList[j] != NULL)
					subElementsList.push_back(currentCell->elementsList[j]);
			currentCell->elementsList.clear();
			currentCell->elementsList.resize(currentCell->elements * currentCell->elements);

			//	Generate second hashes
			for (int j = 0; j < currentCell->elements; j++) {
				currentCell->elementsList[
					secondHash(to_string(subElementsList[j]), currentCell->elements * currentCell->elements)]
					= subElementsList[j];
			}
		}
	}

	void normalize(int hash) {
		vector<T> subElementsList;	//	List for temporary list
		Cell<T>* currentCell;			//	Pointer to current cell
		subElementsList.clear();
		currentCell = this->table[hash];
		for (int j = 0; j < currentCell->elementsList.size(); j++)
			if (currentCell->elementsList[j] != NULL)
				subElementsList.push_back(currentCell->elementsList[j]);
		currentCell->elementsList.clear();
		currentCell->elementsList.resize(currentCell->elements * currentCell->elements);

		//	Generate second hashes
		for (int j = 0; j < currentCell->elements; j++) {
			unsigned int secondHashValue = secondHash(to_string(subElementsList[j]), currentCell->elements * currentCell->elements);
			currentCell->elementsList[secondHashValue] = subElementsList[j];
		}
	}

	/***
		Adds element to Hash Table.
		Does NOT generates second hash. Call normalize()
		to create perfect hashing.
	//*/
	void addElement(T element) {
		unsigned int hash = firstHash(to_string(element), numberOfElements);
		this->table[hash]->elements++;
		this->table[hash]->elementsList.push_back(element);

		normalize(hash);
	}

	/***
		Gets element from HashTable. Return type is specified
		when initializing HashTable.
	//*/
	T getElement(string element) {
		int firstHashValue = firstHash(element, numberOfElements);
		Cell<T>* currentCell;
		currentCell = this->table[firstHashValue];
		int secondHashValue = secondHash(element, currentCell->elements * currentCell->elements);
		return currentCell->elementsList[secondHashValue];
	}

	bool hasElement(string element) {
		int firstHashValue = firstHash(element, numberOfElements);
		if (this->table[firstHashValue] == NULL)
			return false;
		Cell<T>* currentCell;
		currentCell = this->table[firstHashValue];
		int secondHashValue = secondHash(element, currentCell->elements * currentCell->elements);
		if (currentCell->elementsList[secondHashValue] == NULL)
			return false;
		return true;
	}

	Cell<T*> at(int n) {
		return this->table[n];
	}

	int size() {
		return this->numberOfElements;
	}
};