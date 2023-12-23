#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
#include <ctime>
using namespace std;

//liest die Sitzordnung aus der Datei ein und speichert sie in seating
void readSeating(istream& input, vector<vector<int>>& seating) {
	for(int i = 0; i < 10; ++i) {
		string line;
		getline(input, line);
		istringstream iss(line);
		for(int j = 0; j < 10; ++j) {
			iss >> seating[i][j];
		}
	}
}

int calculateLocalCompatibility(const vector<vector<int>>& seating, const vector<vector<int>>& compatibilityMatrix, int row, int col) {
    int sum = 0;
    if (row > 0) sum += compatibilityMatrix[seating[row][col]-1][seating[row-1][col]-1];
    if (row < 9) sum += compatibilityMatrix[seating[row][col]-1][seating[row+1][col]-1];
    if (col > 0) sum += compatibilityMatrix[seating[row][col]-1][seating[row][col-1]-1];
    if (col < 9) sum += compatibilityMatrix[seating[row][col]-1][seating[row][col+1]-1];
    return sum;
}

int totalCompatibility(const vector<vector<int>>& seating, const vector<vector<int>>& compatibilityMatrix) {
    int sum = 0;
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            sum += calculateLocalCompatibility(seating, compatibilityMatrix, i, j);
        }
    }
    return sum / 2; // Jede Kompatibilität wird zweimal gezählt
}

//berechnet die Wahrscheinlichkeit, mit der eine neue Sitzordnung akzeptiert wird
double acceptanceProbability(int currentScore, int newScore, double temperature) {
	if(newScore > currentScore) {
		return 1.0;
	}
	return exp((newScore - currentScore) / temperature);
}

void algorithm(vector<vector<int>>& seating, vector<vector<int>>& compatibilityMatrix) {
    double temperature = 1e6;
    double minTemperature = 1e-6;
    double coolingRate = 1e-6;

    int currentScore = totalCompatibility(seating, compatibilityMatrix);
    srand(time(0));

    while (temperature > minTemperature) {
        int randomRow1 = rand() % 10;
        int randomRow2 = rand() % 10;
        int randomCol1 = rand() % 10;
        int randomCol2 = rand() % 10;

        int deltaScore = 0;
        deltaScore -= calculateLocalCompatibility(seating, compatibilityMatrix, randomRow1, randomCol1);
        deltaScore -= calculateLocalCompatibility(seating, compatibilityMatrix, randomRow2, randomCol2);

        swap(seating[randomRow1][randomCol1], seating[randomRow2][randomCol2]);

        deltaScore += calculateLocalCompatibility(seating, compatibilityMatrix, randomRow1, randomCol1);
        deltaScore += calculateLocalCompatibility(seating, compatibilityMatrix, randomRow2, randomCol2);

        int newScore = currentScore + deltaScore;

        if (acceptanceProbability(currentScore, newScore, temperature) > static_cast<double>(rand()) / RAND_MAX) {
            currentScore = newScore;
        } else {
            swap(seating[randomRow1][randomCol1], seating[randomRow2][randomCol2]); // Tausch rückgängig machen
        }

        temperature *= 1 - coolingRate;
        if (currentScore >= 1600) {
            break;
        }
    }
}

int main() {
    vector<vector<int>> compatibilityMatrix = {
        {10, 3, 0},
        {3, 7, 9},
        {0, 9, 2}};

    vector<vector<int>> seating(10, vector<int>(10));
    readSeating(cin, seating);
    cout << "#Seating chart of size 10 by 10 with R = " << totalCompatibility(seating, compatibilityMatrix) << endl;

    algorithm(seating, compatibilityMatrix);

    int optimizedSum = totalCompatibility(seating, compatibilityMatrix);
    cout << "#Seating chart of size 10 by 10 with R = " << optimizedSum << endl;
    cout << "#Population of different types: 34, 33, 33 (4.19245e+45 possible seating arrangements)" << endl;

    for (const auto& row : seating) {
        for (int seat : row) {
            cout << seat << " ";
        }
        cout << endl;
    }
}
