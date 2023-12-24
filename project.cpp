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

//Kompatibilitätsmatrix aus der Aufgabenstellung
vector<vector<int>> compatibilityMatrix = {
	{10, 3, 0},
	{3, 7, 9},
	{0, 9, 2}};

//liest eine Matrix aus einem Inputstream ein und speichert sie in einen Vektor ab
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

//berechnet die Kompatibilität eines einzelnen Sitzes mit seinen Nachbarn
int localCompatibility(vector<vector<int>>& matrix, vector<vector<int>>& compatibilityMatrix, int& row, int& col) {
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
void algorithm(vector<vector<int>>& matrix, vector<vector<int>>& bestMatrix, int& bestScore, vector<vector<int>>& compatibilityMatrix) {
	srand(time(0));

	double temperature = 1e7;
	double minTemperature = 1e-8;
	double coolingRate = 1e-6; 

	int currentScore = totalCompatibility(matrix, compatibilityMatrix);

	while(temperature > minTemperature) {
		//zwei zufällige Sitzplätze auswählen
		int randomRow1 = rand() % 10;
		int randomRow2 = rand() % 10;
		int randomCol1 = rand() % 10;
		int randomCol2 = rand() % 10;
		
		int deltaScore = 0;
		//Kompatibilität der beiden Sitzplätze mit ihren Nachbarn berechnen
		deltaScore -= localCompatibility(matrix, compatibilityMatrix, randomRow1, randomCol1);
		deltaScore -= localCompatibility(matrix, compatibilityMatrix, randomRow2, randomCol2);

		swap(matrix[randomRow1][randomCol1], matrix[randomRow2][randomCol2]);

		//Kompatibilität der beiden Sitzplätze mit ihren Nachbarn berechnen, nachdem diese getauscht wurden
		deltaScore += localCompatibility(matrix, compatibilityMatrix, randomRow1, randomCol1);
		deltaScore += localCompatibility(matrix, compatibilityMatrix, randomRow2, randomCol2);

		int newScore = currentScore + deltaScore;
		//wenn die neue Sitzordnung eine neue beste Sitzordnung ist, diese speichern
		if (newScore > bestScore) {
			bestMatrix = matrix;
			bestScore = newScore;
		}
		//wenn die neue Sitzordnung akzeptiert wird, diese übernehmen
		if(acceptanceProbability(currentScore, newScore, temperature) > (double)rand() / (RAND_MAX)) {
			currentScore = newScore;
		} else {
			swap(matrix[randomRow1][randomCol1], matrix[randomRow2][randomCol2]);
		}		
		temperature *= 1 - coolingRate;
	}
}

//gibt eine Matrix auf der Konsole aus
void printMatrix(const vector<vector<int>>& matrix) {
	for(int i = 0; i < rows; ++i) {
		for(int j = 0; j < cols; ++j) {
			cout << matrix[i][j] << " ";
		}
		cout << endl;
	}
}

int main() {
	//Zeitmessung
	auto start = chrono::high_resolution_clock::now();

	vector<vector<int>> matrix(rows, vector<int>(cols));
	vector<vector<int>> bestMatrix(rows, vector<int>(cols));
	int bestScore = 0;

	//Matrix aus Standardinput und R berechnen
	readMatrix(cin, matrix);
	cout << "#Starting seating chart of size 10 by 10 with R = " << totalCompatibility(matrix, compatibilityMatrix) << endl;

	//Algorithmus 3 mal ausführen und die beste Sitzordnung speichern
	for (int counter = 0; counter < 3; counter++) {
		algorithm(matrix, bestMatrix, bestScore, compatibilityMatrix);
		if (bestScore > totalCompatibility(matrix, compatibilityMatrix)) {
			matrix = bestMatrix;
		}
	}

	//R der besten Sitzordnung ausgeben
	int optimizedSum = totalCompatibility(matrix, compatibilityMatrix);
	cout << "#Optimized seating chart of size 10 by 10 with R = " << optimizedSum << endl;

	//Matrix ausgeben
	printMatrix(matrix);

	auto end = chrono::high_resolution_clock::now();
	cout << "Total elapsed time: " << chrono::duration_cast<chrono::seconds>(end - start).count() << "s" << endl;
}
