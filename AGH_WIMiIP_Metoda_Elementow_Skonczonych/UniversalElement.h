#pragma once
#include <array>

struct IntegrationPoint {
	double ksi;
	double eta;
};

class UniversalElement {

	// 4x4 matrices - 4 nodes (shape functions) * 4 integration points
	std::array<std::array<double, 4>, 4> m_KsiMatrix;
	std::array<std::array<double, 4>, 4> m_EtaMatrix;

	std::array<IntegrationPoint, 4> m_integrationPoints;
	std::array<double, 4> m_integrationPointsWeights;

	std::array<std::array<double, 4>, 4> m_shapeFunctionValues;
	
public:
	UniversalElement();

	double getKsiValue(size_t i, size_t j) {
		return m_KsiMatrix.at(i).at(j);
	}

	double getEtaValue(size_t i, size_t j) {
		return m_EtaMatrix.at(i).at(j);
	}

	double getShapeFunctionValue(size_t i, size_t j) {
		return m_shapeFunctionValues.at(i).at(j);
	}
};