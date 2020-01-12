#include "Utils.h"

void Utils::solver(GlobalData data, Grid grid){

	unsigned int steps = static_cast<unsigned int>(data.getTimeDuration() / data.getTimeStep());
	double timePassed = 0.;

	for (unsigned int i = 0; i < steps; i++) {
		timePassed += data.getTimeStep();
		if(verboseSolver) printf("Iteration [%u] after %0.1lf s.\n", i, timePassed);

		//Calculate global matrices H, C and P vector
		grid.compute();

		//Solve equasion
		std::vector<double> temps = Utils::gaussianElimination(grid.getGlobalHMatrix(), grid.getGlobalPVector());
		
		//Set calculated temperature for each node
		for (uint64_t j = 0; j < 16; j++) {
			grid.setNodeTemp(j, temps[j]);
		}

		for (int k = 0; k < data.getMH(); k++) {
			for (int l = 0; l < data.getML(); l++) {
				printf("%0.3lf\t\t", temps[k * data.getML() + l]);
			}
			printf("\n");
		}

		std::tuple<double, double> minMax = Utils::findMinMax(temps);
		printf("Min: %0.3lf\t\t", std::get<0>(minMax));
		printf("Max: %0.3lf\n\n", std::get<1>(minMax));


	}
}

//taken from https://github.com/adamczykpiotr/AGH_WIMiIP_Metody_Numeryczne
std::vector<double> Utils::gaussianElimination(std::vector<std::vector<double>> hc, std::vector<double> p) {

	std::vector<std::vector<double>> matrix = hc;

	//Add p vector as a last column in hc matrix
	for (int i = 0; i < hc.size(); i++) {
		matrix[i].push_back(p[i]);
	}

	unsigned int matrixSize = matrix.size();
	unsigned int matrixCols = matrix[0].size();
	unsigned int startPos = 0;
	std::vector<double> coefficients;
	coefficients.reserve(matrixSize);

	//Triangulate matrix
	while (coefficients.size() < 1) {

		for (unsigned int i = startPos + 1; i < matrixSize; i++) {
			double coeff = matrix[i][startPos] / matrix[startPos][startPos];
			for (unsigned int j = startPos; j < matrix[i].size(); j++) {
				matrix[i][j] -= matrix[startPos][j] * coeff;
			}
		}

		//If last row has only 1 coefficient, push value to vector thus end loop
		if (matrix[matrixSize - 1][matrixCols - 3] == 0) {
			double xn = matrix[matrixSize - 1][matrixCols - 1] / matrix[matrixSize - 1][matrixCols - 2];
			coefficients.push_back(xn);
		}
		startPos++;
	}

	//Start searching for other coefficients
	unsigned int coefficientsSize = coefficients.size();
	while (coefficientsSize < matrixSize) {
		double xn = matrix[matrixSize - coefficientsSize - 1][matrixCols - 1];
		for (unsigned int i = 0; i < coefficientsSize; i++) {
			xn -= matrix[matrixSize - coefficientsSize - 1][matrixCols - 2 - i] * coefficients[i];
		}
		xn /= matrix[matrixSize - coefficientsSize - 1][matrixCols - coefficientsSize - 2];
		coefficients.push_back(xn);
		coefficientsSize = coefficients.size();
	}

	//Reverse vector
	std::vector<double> ret;
	for (unsigned int i = 0; i < matrixSize; i++) {
		ret.push_back(coefficients[matrixSize - 1 - i]);
	}

	return ret;
}

template<typename T>
inline std::pair<T, T> Utils::findMinMax(std::vector<T>& vec) {

	if (vec.size() == 0) {
		return std::make_pair<T, T>(0, 0);
	}

	T min = vec.at(0);
	T max = vec.at(0);

	for (const auto& el : vec) {
		if (el > max) max = el;
		if (el < min) min = el;
	}

	return std::make_pair<T, T>(static_cast<T>(min), static_cast<T>(max));
}