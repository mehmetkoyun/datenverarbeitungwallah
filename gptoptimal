#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>

using namespace std;

const int ROWS = 10;
const int COLS = 10;
int seating[ROWS][COLS];
int compatibility[3][3] = {
    {10, 3, 0},
	{3, 7, 9},
	{0, 9, 2}
};

void readSeatingArrangement() {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            cin >> seating[i][j];
        }
    }
}

int calculateCompatibility () {
	int totalCompatibility = 0;
	for(int i = 0; i < 9; ++i) {
		for(int j = 0; j < 9; ++j) {
			totalCompatibility += compatibility[seating[i][j]-1][seating[i][j+1]-1];
			totalCompatibility += compatibility[seating[i][j]-1][seating[i+1][j]-1];
		}
	}
	//letzte Zeile und letzte Spalte
	for (int  i = 0; i < 9; i++) {
		totalCompatibility += compatibility[seating[9][i+1]-1][seating[9][i]-1];
		totalCompatibility += compatibility[seating[i][9]-1][seating[i+1][9]-1];
	}
	
	return totalCompatibility;
}

void optimizeSeating() {
    srand(time(0));
    int currentCompatibility = calculateCompatibility();

    for (int attempt = 0; attempt < 10000; attempt++) {
        int i1 = rand() % ROWS;
        int j1 = rand() % COLS;
        int i2 = rand() % ROWS;
        int j2 = rand() % COLS;

        swap(seating[i1][j1], seating[i2][j2]);
        int newCompatibility = calculateCompatibility();

        if (newCompatibility > currentCompatibility) {
            currentCompatibility = newCompatibility;
        } else {
            // Swap back if not better
            swap(seating[i1][j1], seating[i2][j2]);
        }
    }
}

int main() {
    readSeatingArrangement();

    cout << "Initial compatibility: " << calculateCompatibility() << endl;

    optimizeSeating();

    cout << "Optimized compatibility: " << calculateCompatibility() << endl;
    cout << "Optimized seating arrangement:" << endl;
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            cout << seating[i][j] << " ";
        }
        cout << endl;
    }
    return 0;
}
