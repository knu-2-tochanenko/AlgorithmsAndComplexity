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
		cout << "Do you want to see export progress? Y/N : ";
		SetConsoleTextAttribute(hConsole, 15);
		char yn;
		cin >> yn;

		SetConsoleTextAttribute(hConsole, 14);
		cout << "Do you want to see debug information? Y/N : ";
		SetConsoleTextAttribute(hConsole, 15);
		char yn2;
		cin >> yn2;

		bool displayData = false;
		if (yn == 'Y')
			displayData = true;
		bool displayDebug = false;
		if (yn2 == 'Y')
			displayDebug = true;

		Store store(fileName, displayData, displayDebug);
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
			char name[100];
			cin.getline(name, sizeof(name));
			cin.getline(name, sizeof(name));
			store.printProduct(category, name);
		}
	}
};