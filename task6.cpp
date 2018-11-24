#include <iostream>
#include <string>

using namespace std;

bool isShifted(string input, string shifted) {
	int shift = -1;
	int size = input.size();
	if (shifted.size() != size)
		return false;
	
	while (shift < size) {
		shift++;
		// Find entry of the first symbol
		for (int i = shift; i < size; i++)
			if (input[0] == shifted[i]) {
				shift = i;
				break;
			}

		// Check for match
		for (int i = shift; i < size; i++)
			if (input[i - shift] != shifted[i])
				goto end;
		for (int i = 0; i < shift; i++)
			if (input[size - shift + i] != shifted[i])
				goto end;
		return true;
	end:;
	}
	return false;
}

int main() {
	string input;
	string shifted;
	cin >> input >> shifted;

	cout << isShifted(input, shifted) << endl;

	system("pause");
	return 0;
}