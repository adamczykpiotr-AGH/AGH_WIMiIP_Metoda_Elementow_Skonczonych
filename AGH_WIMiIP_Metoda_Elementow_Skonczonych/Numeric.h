#pragma once
#include <vector>
class Numeric {
public:
	static double gaussianQuadrature(const std::vector<double>& xi, const std::vector<double>& yi, const std::vector<double>& weight, const std::vector<double>& point);
	static std::vector<double> gaussianElimination(std::vector<std::vector<double>>* matrixPtr);
};

