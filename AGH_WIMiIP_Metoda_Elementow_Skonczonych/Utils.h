#pragma once
#include "GlobalData.h"
#include "Grid.h"
#include <vector>
#include <iostream>

static constexpr bool verboseSolver = true;

class Utils {
public:

	static void solver(GlobalData data, Grid grid);
	
	static std::vector<double> gaussianElimination(std::vector<std::vector<double>> hc, std::vector<double> p);
		
	template <typename T>
	static std::pair<T, T> findMinMax(std::vector<T> & vec);

};