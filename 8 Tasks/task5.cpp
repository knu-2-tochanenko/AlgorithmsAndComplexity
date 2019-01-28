#include <iostream>
#include <random>
#include <time.h>

using namespace std;

int const MAX = 100;
int const MAX_STRASSEN = 2;
int const MAX_POW = 7;

int** matricesDo(int **firstMatrix, int **secondMatrix, int n, bool add) {
	int **resMatrix = new int*[n];
	for (int i = 0; i < n; i++) {
		resMatrix[i] = new int[n];
		for (int j = 0; j < n; j++)
			resMatrix[i][j] = (add) ? firstMatrix[i][j] + secondMatrix[i][j]
									: firstMatrix[i][j] - secondMatrix[i][j];
	}
	return resMatrix;
}

int** normalMultiply(int **firstMatrix, int **secondMatrix, int n) {
	int **resultMatrix = new int*[n];
	for (int i = 0; i < n; i++) {
		resultMatrix[i] = new int[n];
		for (int j = 0; j < n; j++) {
			resultMatrix[i][j] = 0;
			for (int z = 0; z < n; z++)
				resultMatrix[i][j] += firstMatrix[i][z] * secondMatrix[z][j];
		}
	}
	return resultMatrix;
}

// Function to count single 
int** strassenSingle(int **firstMatrix, int **secondMatrix, int n) {

	int n2 = n / 2;
	int **p1;
	int **p2;
	int **p3;
	int **p4;
	int **p5;
	int **p6;
	int **p7;
	int **a11 = new int*[n2];
	int **a12 = new int*[n2];
	int **a21 = new int*[n2];
	int **a22 = new int*[n2];
	int **b11 = new int*[n2];
	int **b12 = new int*[n2];
	int **b21 = new int*[n2];
	int **b22 = new int*[n2];
	int **c11;
	int **c12;
	int **c21;
	int **c22;
	for (int i = 0; i < n2; i++) {
		a11[i] = new int[n2];
		a12[i] = new int[n2];
		a21[i] = new int[n2];
		a22[i] = new int[n2];
		b11[i] = new int[n2];
		b12[i] = new int[n2];
		b21[i] = new int[n2];
		b22[i] = new int[n2];
		for (int j = 0; j < n2; j++) {
			a11[i][j] = firstMatrix[i][j];
			a12[i][j] = firstMatrix[i][j + n2];
			a21[i][j] = firstMatrix[i + n2][j];
			a22[i][j] = firstMatrix[i + n2][j + n2];
			b11[i][j] = secondMatrix[i][j];
			b12[i][j] = secondMatrix[i][j + n2];
			b21[i][j] = secondMatrix[i + n2][j];
			b22[i][j] = secondMatrix[i + n2][j + n2];
		}
	}

	// Optimization
	int **s1 = matricesDo(a11, a22, n2, true);
	int **s2 = matricesDo(b11, b22, n2, true);
	int **s3 = matricesDo(a21, a22, n2, true);
	int **s4 = matricesDo(b11, b12, n2, false);
	int **s5 = matricesDo(b21, b11, n2, false);
	int **s6 = matricesDo(a11, a12, n2, true);
	int **s7 = matricesDo(a21, a11, n2, false);
	int **s8 = matricesDo(b11, b12, n2, true);
	int **s9 = matricesDo(a12, a22, n2, false);
	int **s10 = matricesDo(b21, b22, n2, true);

	p1 = (n > MAX_STRASSEN) ? strassenSingle(s1, s2, n2)
							: normalMultiply(s1, s2, n2);
	p2 = (n > MAX_STRASSEN) ? strassenSingle(s3, b11, n2)
							: normalMultiply(s3, b11, n2);
	p3 = (n > MAX_STRASSEN) ? strassenSingle(a11, s4, n2)
							: normalMultiply(a11, s4, n2);
	p4 = (n > MAX_STRASSEN) ? strassenSingle(a22, s5, n2)
							: normalMultiply(a22, s5, n2);
	p5 = (n > MAX_STRASSEN) ? strassenSingle(s6, b22, n2)
							: normalMultiply(s6, b22, n2);
	p6 = (n > MAX_STRASSEN) ? strassenSingle(s7, s8, n2)
							: normalMultiply(s7, s8, n2);
	p7 = (n > MAX_STRASSEN) ? strassenSingle(s9, s10, n2)
							: normalMultiply(s9, s10, n2);

	c11 = matricesDo(matricesDo(matricesDo(p1, p4, n2, true), p5, n2, false), p7, n2, true);
	c12 = matricesDo(p3, p5, n2, true);
	c21 = matricesDo(p2, p4, n2, true);
	c22 = matricesDo(matricesDo(matricesDo(p1, p2, n2, false), p3, n2, true), p6, n2, true);

	int **resultMatrix = new int*[n];
	for (int i = 0; i < n; i++)
		resultMatrix[i] = new int[n];
	for (int i = 0; i < n2; i++)
		for (int j = 0; j < n2; j++) {
			resultMatrix[i][j]				= c11[i][j];
			resultMatrix[i][j + n2]			= c12[i][j];
			resultMatrix[i + n2][j]			= c21[i][j];
			resultMatrix[i + n2][j + n2]	= c22[i][j];
		}

	// Delete unused memory
	delete[] a11;
	delete[] a12;
	delete[] a21;
	delete[] a22;
	delete[] b11;
	delete[] b12;
	delete[] b21;
	delete[] b22;
	delete[] c11;
	delete[] c12;
	delete[] c21;
	delete[] c22;
	delete[] p1;
	delete[] p2;
	delete[] p3;
	delete[] p4;
	delete[] p5;
	delete[] p6;
	delete[] p7;
	delete[] s1;
	delete[] s2;
	delete[] s3;
	delete[] s4;
	delete[] s5;
	delete[] s6;
	delete[] s7;
	delete[] s8;
	delete[] s9;
	delete[] s10;

	return resultMatrix;
}

// Function to init Strassen algorithm
int** strassenMultiply(int **firstMatrix, int **secondMatrix, int n) {
	int **resultMatrix = new int*[n];
	for (int i = 0; i < n; i++)
		resultMatrix[i] = new int[n];

	strassenSingle(firstMatrix, secondMatrix, n);

	return resultMatrix;
}

void test(int n) {
	// Generation matrices
	int initTime = clock();
	int **resultStrassen;
	int **resultNormal;
	int **firstMatrix = new int*[n];
	for (int i = 0; i < n; i++) {
		firstMatrix[i] = new int[n];
		for (int j = 0; j < n; j++)
			firstMatrix[i][j] = rand() % MAX;
	}
	int **secondMatrix = new int*[n];
	for (int i = 0; i < n; i++) {
		secondMatrix[i] = new int[n];
		for (int j = 0; j < n; j++)
			secondMatrix[i][j] = rand() % MAX;
	}
	int endTime = clock();
	cout << "Matrices are generated in : " << (endTime - initTime) << "\n";

	// Test Strassen
	initTime = clock();
	resultStrassen = strassenMultiply(firstMatrix, secondMatrix, n);
	endTime = clock();
	cout << "Strassen time : " << (endTime - initTime) << "\n";

	// Test normal
	initTime = clock();
	resultNormal = normalMultiply(firstMatrix, secondMatrix, n);
	endTime = clock();
	cout << "Normal time : " << (endTime - initTime) << "\n";
}

int main() {

	cout << "Input the pow of 2 in range [1 - " << MAX_POW << "]: ";
	int n;
	cin >> n;
	n = n % (MAX_POW + 1);

	test(pow(2,n));

	system("pause");
	return 0;
}