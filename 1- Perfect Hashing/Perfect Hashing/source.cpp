#include "grocerystore.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {

	Store store("grocerystore.txt");

	store.printCategory("fruits");
	store.printCategory("soda");
	store.printCategory("toys");
	
	system("pause");
	return 0;
}