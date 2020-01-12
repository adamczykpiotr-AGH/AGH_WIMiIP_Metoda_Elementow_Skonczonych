#pragma once
#include <array>
#include "UniversalElement.h"

class Jacobian {

	double m_determinant;
	std::array<std::array<double, 2>, 2> m_data;
	std::array<std::array<double, 2>, 2> m_invertedData;

public:

	Jacobian(std::array<double, 4> x, std::array<double, 4> y, int pointId, UniversalElement element);

	double getJacobianValue(size_t i, size_t j) {
		return m_data.at(i).at(j);
	}
	
	double getInvertedJacobianValue(size_t i, size_t j) {
		return m_invertedData.at(i).at(j);
	}
	
	double getDeterminant() {
		return m_determinant;
	}
};
