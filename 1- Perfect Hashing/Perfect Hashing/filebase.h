#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class DataFile {
private:
	ifstream file;
	vector<string> categories;

    int pointer = 0;
	
public:
	DataFile(string fileName) {
		this->file.open(fileName, ifstream::in);
		
		//	Get number of categories
		string line;
		int numberOfCategories = stoi(line);
        for (int i = 0; i < numberOfCategories; i++) {
            getline(this->file, line);
            this->categories.push_back(line);
        }
	}

	vector<string> nextCategory() {
        vector<string> res;
        if (pointer == this->categories.size() - 1)
            return res;

        //  Push category name as first element
        res.push_back(this->categories[pointer]);
        string line;
		int numberOfElements = stoi(line);
        for (int i = 0; i < numberOfElements; i++) {
            getline(this->file, line);
            res.push_back(line);
        }
        pointer++;
        return res;
	}

    ~DataFile() {
        this->file.close();
    }
};