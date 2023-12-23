#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
#include <ctime>
#include <chrono>

using namespace std;

const int rows = 10;
const int cols = 10;

vector<vector<int>> compatibilityMatrix = {
	{10, 3, 0},
	{3, 7, 9},
	{0, 9, 2}};

//liest eine Matrix aus einem Inputstream ein und speichert sie in einer Array ab
void readMatrix(istream& input, vector<vector<int>>& matrix) {
	for(int i = 0; i < rows; ++i) {
		for(int j = 0; j < cols; ++j) {
			input >> matrix[i][j];
		}
	}
}

//berechnet die Kompatibilität der Sitzordnung
int totalCompatibility(vector<vector<int>>& matrix, vector<vector<int>>& compatibilityMatrix) {
	int sum = 0;
	for(int i = 0; i < 9; ++i) {
		for(int j = 0; j < 9; ++j) {
			sum += compatibilityMatrix[matrix[i][j]-1][matrix[i][j+1]-1];
			sum += compatibilityMatrix[matrix[i][j]-1][matrix[i+1][j]-1];
		}
	}
	//letzte Zeile und letzte Spalte
	for (int  i = 0; i < 9; i++) {
		sum += compatibilityMatrix[matrix[9][i+1]-1][matrix[9][i]-1];
		sum += compatibilityMatrix[matrix[i][9]-1][matrix[i+1][9]-1];
	}
	
	return sum;
}

int localCompatibility(vector<vector<int>>& matrix, vector<vector<int>>& compatibilityMatrix, int row, int col) {
	int sum = 0;
	if(row > 0) {
		sum += compatibilityMatrix[matrix[row][col]-1][matrix[row-1][col]-1];
	}
	if(row < rows - 1) {
		sum += compatibilityMatrix[matrix[row][col]-1][matrix[row+1][col]-1];
	}
	if(col > 0) {
		sum += compatibilityMatrix[matrix[row][col]-1][matrix[row][col-1]-1];
	}
	if(col < cols - 1) {
		sum += compatibilityMatrix[matrix[row][col]-1][matrix[row][col+1]-1];
	}
	return sum;
}

//berechnet die Wahrscheinlichkeit, mit der eine neue Sitzordnung akzeptiert wird
double acceptanceProbability(int currentScore, int newScore, double temperature) {
	if(newScore > currentScore) {
		return 1.0;
	}
	return exp((newScore - currentScore) / temperature);
}


//implementiert den Simulated Annealing Algorithmus

void algorithm(vector<vector<int>>& matrix, vector<vector<int>>& compatibilityMatrix) {
	double temperature = 1e8;
	double minTemperature = 1e-8;
	double coolingRate = 1e-6;

	int currentScore = totalCompatibility(matrix, compatibilityMatrix);
	srand(time(0));
	
	auto start = chrono::high_resolution_clock::now();
	
	while(temperature > minTemperature) {
		int randomRow1 = rand() % 10;
		int randomRow2 = rand() % 10;
		int randomCol1 = rand() % 10;
		int randomCol2 = rand() % 10;
		
		int deltaScore = 0;

		deltaScore -= localCompatibility(matrix, compatibilityMatrix, randomRow1, randomCol1);
		deltaScore -= localCompatibility(matrix, compatibilityMatrix, randomRow2, randomCol2);

		swap(matrix[randomRow1][randomCol1], matrix[randomRow2][randomCol2]);

		deltaScore += localCompatibility(matrix, compatibilityMatrix, randomRow1, randomCol1);
		deltaScore += localCompatibility(matrix, compatibilityMatrix, randomRow2, randomCol2);

		int newScore = currentScore + deltaScore;

		if(acceptanceProbability(currentScore, newScore, temperature) > (double)rand() / (RAND_MAX)) {
			currentScore = newScore;
		} else {
			swap(matrix[randomRow1][randomCol1], matrix[randomRow2][randomCol2]);
		}

		auto current = chrono::high_resolution_clock::now();
		if (current - start >= chrono::seconds(10)) {
			cout << "Time limit exceeded" << endl;
			break;
		}
		
		temperature *= 1 - coolingRate;
	}
	cout << "Elapsed time: " << chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - start).count() << "ms" << endl;
}

int main() {

	vector<vector<int>> matrix(rows, vector<int>(cols));

	readMatrix(cin, matrix);

	cout << "#Starting seating chart of size 10 by 10 with R = " << totalCompatibility(matrix, compatibilityMatrix) << endl;
	int sum = totalCompatibility(matrix, compatibilityMatrix); // Pass compatibilityMatrix as a reference
	algorithm(matrix, compatibilityMatrix);

	int optimizedSum = totalCompatibility(matrix, compatibilityMatrix);
	cout << "#Optimized eeating chart of size 10 by 10 with R = " << optimizedSum << endl;

	for (const auto& row : matrix) {
        for (int seat : row) {
            cout << seat << " ";
        }
        cout << endl;
    }
}
