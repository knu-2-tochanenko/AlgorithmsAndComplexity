#pragma once

#include "store.h"

#include <iostream>
#include <string>
#include <windows.h>

using namespace std;

class CommandLine {
public:
	void run() {
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		string fileName;
		SetConsoleTextAttribute(hConsole, 14);
		cout << "Give me the name of the file with database : ";
		SetConsoleTextAttribute(hConsole, 15);
		cin >> fileName;
//		fileName = "small.csv";

		SetConsoleTextAttribute(hConsole, 14);
		cout << "Do you want to see export progress? Y/N : ";
		SetConsoleTextAttribute(hConsole, 15);
		char yn;
		cin >> yn;
//		yn = 'Y';

		bool displayData = false;
		if (yn == 'Y')
			displayData = true;

		Store store(fileName, displayData);

		if (displayData)
			store.printTrees();

		SetConsoleTextAttribute(hConsole, 14);
		cout << "\n\n--------------------------------------------------\n";
		while (1) {
			SetConsoleTextAttribute(hConsole, 14);
			cout << "SEARCH\n";
			SetConsoleTextAttribute(hConsole, 15);
			char category[100];
			cout << "category  :  ";
			cin >> category;
			cout << "product   :  ";
			int rang;
			cin >> rang;
			store.printProduct(category, rang);
		}
	}
};