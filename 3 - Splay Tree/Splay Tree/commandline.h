#pragma once

#include "store.h"

#include <iostream>
#include <string>
#include <windows.h>

#include <typeinfo>

using namespace std;

class CommandLine {
public:
	void run(int mode) {
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

		Store store(fileName, mode, displayImport);

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
			cout << "value   :  ";
			int integer;
			double dbl;
			string str;

						//	1	:	Name
						//	2	:	Days till expired
						//	3	:	Weight
						//	0..	:	Price

			if (mode == 1) {
				cin >> str;
				store.printProduct(category, new Product(str, 0, 0, 0));
			}
			else if (mode == 2) {
				cin >> integer;
				store.printProduct(category, new Product("", integer, 0, 0));
			}
			else if (mode == 3) {
				cin >> dbl;
				store.printProduct(category, new Product("", 0, dbl, 0));
			}
			else {
				cin >> dbl;
				store.printProduct(category, new Product("", 0, 0, dbl));
			}
		}
	}
};