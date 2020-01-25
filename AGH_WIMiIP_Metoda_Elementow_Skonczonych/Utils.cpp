#include "Utils.h"

void Utils::solver(GlobalData data, Grid & grid, std::pair<bool, bool> settings){

	unsigned int steps = static_cast<unsigned int>(data.getTimeDuration() / data.getTimeStep());
	double timePassed = 0.;

	for (unsigned int i = 0; i < steps; i++) {
		auto t = new Timer("Loop");
		timePassed += data.getTimeStep();
		printf("Iteration .at(%u) after %0.1lf s.\n", i, timePassed);

		//Calculate global matrices H, C and P vector
		grid.compute(settings);

		//Solve equasion
		std::vector<double> temps(grid.getGlobalHMatrixPtr()->size(), 0);
		Utils::gaussianElimination(grid.getGlobalHMatrixPtr(), grid.getGlobalPVectorPtr(), &temps);

		//Set calculated temperature for each node
		for (uint64_t j = 0; j < data.getMW(); j++) {
			grid.setNodeTemp(j, temps.at(j));
		}

		if (std::get<0>(settings)) {
			for (uint64_t k = 0; k < data.getMH(); k++) {
				for (uint64_t l = 0; l < data.getML(); l++) {
					printf("%0.3lf\t\t", temps.at(k * data.getML() + l));
				}
				printf("\n");
			}
		}

		std::tuple<double, double> minMax = Utils::findMinMax(temps);
		delete t;
		printf("Min: %0.3lf\t\t", std::get<0>(minMax));
		printf("Max: %0.3lf\n\n", std::get<1>(minMax));
	}
}

void Utils::gaussianElimination(std::vector<std::vector<double>>* hc, std::vector<double>* p, std::vector<double>* result) {

	//.at(HC) += {P}
	size_t matrixSize = hc->size();
	for (int i = 0; i < matrixSize; i++) {
		(*hc).at(i).push_back((*p).at(i));
	}

	//Eliminatinon
	auto t1 = new Timer("Gaussian elimination");
	gaussianElimimationWorker(0, matrixSize, hc, matrixSize);
	delete t1;

	//Generating result vector
	for (int i = matrixSize - 1; i >= 0; i--) {
		(*result).at(i) = (*hc).at(i).at(matrixSize);

		for (int j = i + 1; j < matrixSize; j++) {
			if (j != i) {
				(*result).at(i) = (*result).at(i) - (*hc).at(i).at(j) * (*result).at(j);
			}
		}

		(*result).at(i) = (*result).at(i) / (*hc).at(i).at(i);
	}	
}

void Utils::gaussianElimimationWorker(size_t start, size_t end, std::vector<std::vector<double>>* matrixPtr, size_t matrixSize) {

	for (size_t i = start; i < end; i++) {

		for (size_t k = i + 1; k < matrixSize; k++) {

			if ((*matrixPtr).at(k).at(i) == 0) continue;

			double coefficient = (*matrixPtr).at(k).at(i) / (*matrixPtr).at(i).at(i);
			
			for (size_t j = 0; j <= matrixSize; j++) {
				(*matrixPtr).at(k).at(j) -= coefficient * (*matrixPtr).at(i).at(j);
			}
		}
	}
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