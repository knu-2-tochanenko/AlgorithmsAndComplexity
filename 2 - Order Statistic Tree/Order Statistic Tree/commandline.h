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

		SetConsoleTextAttribute(hConsole, 14);
		cout << "Do you want to see export process? Y/N : ";
		SetConsoleTextAttribute(hConsole, 15);
		char yn;
		cin >> yn;

		SetConsoleTextAttribute(hConsole, 14);
		cout << "Do you want to see trees? Y/N : ";
		SetConsoleTextAttribute(hConsole, 15);
		char yn2;
		cin >> yn2;

		bool displayImport = false;
		if (yn == 'Y')
			displayImport = true;
		bool displayTrees = false;
		if (yn2 == 'Y')
			displayTrees = true;

		Store store(fileName, displayImport);

		if (displayTrees)
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