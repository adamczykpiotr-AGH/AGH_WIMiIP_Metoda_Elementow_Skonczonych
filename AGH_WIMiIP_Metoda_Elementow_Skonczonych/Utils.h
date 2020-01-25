#pragma once
#include <vector>
#include <iostream>

#include "GlobalData.h"
#include "Grid.h"
#include "Timer.h"

class Utils {
public:

	static void solver(GlobalData data, Grid & grid, std::pair<bool, bool> settings);

	static void gaussianElimination(std::vector<std::vector<double>> * hc, std::vector<double> *p, std::vector<double> * output);
		
	static void gaussianElimimationWorker(size_t start, size_t end, std::vector<std::vector<double>>* matrixPtr, size_t matrixSize);

	template <typename T>
	static std::pair<T, T> findMinMax(const std::vector<T> & vec);

};