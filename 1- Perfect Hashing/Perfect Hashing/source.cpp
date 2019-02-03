#include "store.h"

#include <iostream>
#include <string>

using namespace std;

int main() {
	Store store("grocerystore.txt");

	string category, product;
	/**
	while (1) {
		cout << "What would you like to search for? [category product]\n";
		cin >> category >> product;
		store.printProduct(category, product);
	}
	//**/

	store.printProduct("vegetables", "sweet potato");
	store.printProduct("ice-creams", "vanilla");
	store.printProduct("soda", "sprite");
	store.printProduct("fruits", "banana");

	system("pause");
	return 0;
}