#include <iostream>
#include <random>
#include <time.h>

using namespace std;

void merge(int *arr, int begin, int middle, int end) {
	int *subArray;
	int subPointer = 0;
	subArray = new int[end - begin];

	int leftPointer = begin;
	int rightPointer = middle + 1;

	for (leftPointer, rightPointer; (leftPointer <= middle) && (rightPointer <= end);) {
		if (arr[leftPointer] < arr[rightPointer]) {
			subArray[subPointer++] = arr[leftPointer++];
		}
		else {
			subArray[subPointer++] = arr[rightPointer++];
		}
	}

	for (; leftPointer <= middle; leftPointer++)
		subArray[subPointer++] = arr[leftPointer];
	for (; rightPointer <= end; rightPointer++)
		subArray[subPointer++] = arr[rightPointer];

	for (int i = begin, j = 0; i <= end; i++, j++)
		arr[i] = subArray[j];
}

void mergeSort(int *arr, int begin, int end) {
	if (end > begin) {
		int middle = (begin + end) / 2;
		mergeSort(arr, begin, middle);
		mergeSort(arr, middle + 1, end);
		merge(arr, begin, middle, end);
	}
}

void mergeSort(int *arr, int end) {
	mergeSort(arr, 0, end - 1);
}

void testArray(int *arr, int number) {
	for (int i = 0; i < number; i++)
		cout << arr[i] << " ";
	cout << endl;
}

int main() {
	srand(clock());
	
	int numberOfElements;
	int *arr;

	cin >> numberOfElements;
	arr = new int[numberOfElements];
	for (int i = 0; i < numberOfElements; i++)
		arr[i] = rand() % 100;

	testArray(arr, numberOfElements);
	mergeSort(arr, numberOfElements);
	testArray(arr, numberOfElements);

	system("pause");
	return 0;
}