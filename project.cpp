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

//berechnet die Kompatibilität der Sitzordnung
int totalCompatibility(vector<vector<int>>& seating, vector<vector<int>>& compatibilityMatrix) {
	int sum = 0;
	for(int i = 0; i < 9; ++i) {
		for(int j = 0; j < 9; ++j) {
			sum += compatibilityMatrix[seating[i][j]-1][seating[i][j+1]-1];
			sum += compatibilityMatrix[seating[i][j]-1][seating[i+1][j]-1];
		}
	}
	//letzte Zeile und letzte Spalte
	for (int  i = 0; i < 9; i++) {
		sum += compatibilityMatrix[seating[9][i+1]-1][seating[9][i]-1];
		sum += compatibilityMatrix[seating[i][9]-1][seating[i+1][9]-1];
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

void algorithm(vector<vector<int>>& seating, vector<vector<int>>& compatibilityMatrix) {
	double temperature = 1e4;
	double minTemperature = 1e-5;
	double coolingRate = 1e-5;

	int currentScore = totalCompatibility(seating, compatibilityMatrix);

	// Set random seed based on current time
	//srand(time(0));

	while(temperature > minTemperature) {
		vector<vector<int>> newSeating = seating;
		// Zufällige Zahlen generieren

		int randomRow1 = rand() % 10;
		int randomRow2 = rand() % 10;
		int randomCol1 = rand() % 10;
		int randomCol2 = rand() % 10;
		
		int temp = newSeating[randomRow1][randomCol1]; //temporärer Speicher
		newSeating[randomRow1][randomCol1] = newSeating[randomRow2][randomCol2]; //Tausch der Plätze von 1 durch zahl von 2
		newSeating[randomRow2][randomCol2] = temp; //Tausch der Plätze von 2 durch zahl von 1
		
		
		int newScore = totalCompatibility(newSeating, compatibilityMatrix);
		
		if(acceptanceProbability(currentScore, newScore, temperature) > (double)rand() / (RAND_MAX)) {
			seating = newSeating;
			currentScore = newScore;
		}
		
		temperature *= 1 - coolingRate;
	if (currentScore >= 1700) {
		break;
	}
	}
}

int main() {

	//Kompatibilitätsmatrix
	vector<vector<int>> compatibilityMatrix = {
		{10, 3, 0},
		{3, 7, 9},
		{0, 9, 2}};

	//cin
	vector<vector<int>> seating(10, vector<int>(10));
	readSeating(cin, seating);
	cout << "#Seating chart of size 10 by 10 with R = " << totalCompatibility(seating, compatibilityMatrix) << endl;
	int sum = totalCompatibility(seating, compatibilityMatrix); // Pass compatibilityMatrix as a reference
	algorithm(seating, compatibilityMatrix);

	int optimizedSum = totalCompatibility(seating, compatibilityMatrix);
	cout << "#Seating chart of size 10 by 10 with R = " << optimizedSum << endl;

	for (const auto& row : seating) {
        for (int seat : row) {
            cout << seat << " ";
        }
        cout << endl;
    }
}

