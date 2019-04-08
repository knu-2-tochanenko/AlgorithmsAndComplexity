#pragma once

#include "store.h"

#include <iostream>
#include <string>
#include <windows.h>

#include <typeinfo>

using namespace std;

class CommandLine {
public:
	void run() {
		int mode;
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		string fileName;
		SetConsoleTextAttribute(hConsole, 14);
		cout << "Give me the name of the file with database : ";
		SetConsoleTextAttribute(hConsole, 15);
		cin >> fileName;

		SetConsoleTextAttribute(hConsole, 14);
		cout << "What mode do you want to use? :\n";
		SetConsoleTextAttribute(hConsole, 12);
		cout << "Please note, if two elements have the same\nfield value, only one of them will be used!\n";
		SetConsoleTextAttribute(hConsole, 14);
		cout << "1\t:\tName\n";
		cout << "2\t:\tDays till expired\n";
		cout << "3\t:\tWeight\n";
		cout << "0..\t:\tPrice\n";
		SetConsoleTextAttribute(hConsole, 15);
		cin >> mode;


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
			store.printTrees(mode);

		SetConsoleTextAttribute(hConsole, 14);
		cout << "\n\n--------------------------------------------------\n";
		cout << "Splay Tree Mode: ";
		SetConsoleTextAttribute(hConsole, 10);
		switch (mode) {
		case 1:
			cout << "Name";
			break;
		case 2:
			cout << "Days till expired";
			break;
		case 3:
			cout << "Weight";
			break;
		default:
			cout << "Price";
			break;
		}
		SetConsoleTextAttribute(hConsole, 14);
		cout << "\n--------------------------------------------------\n";
		while (1) {
			SetConsoleTextAttribute(hConsole, 14);
			cout << "SEARCH\n";
			SetConsoleTextAttribute(hConsole, 15);
			char category[100];
			cout << "category  :  ";
			cin >> category;

			string command;
			cout << "command [print, find] :  ";
			cin >> command;

			if (command == "printall") {
				store.printTree(category, mode);
			}
			else {

				if (command == "print") {
					store.printTree(category, mode);
				}
				else if (command == "find") {
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
						if (!store.printProduct(category, new Product(str, 0, 0, 0))) {
							break;
						}
					}
					else if (mode == 2) {
						cin >> integer;
						if (!store.printProduct(category, new Product("", integer, 0, 0)))
							continue;
					}
					else if (mode == 3) {
						cin >> dbl;
						if (!store.printProduct(category, new Product("", 0, dbl, 0)))
							continue;
					}
					else {
						cin >> dbl;
						if (!store.printProduct(category, new Product("", 0, 0, dbl)))
							continue;
					}
				}
				else {
					cout << "U r dumbo\nThere is no command " << command << endl;
				}
			}
		}
	}
};