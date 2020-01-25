#pragma once
#include <array>

#include "GlobalData.h"
#include "Node.h"
#include "Element.h"
#include "UniversalElement.h"
#include "Jacobian.h"
#include "Surface.h"


class Grid {
	std::vector<Element> m_elements;
	std::vector<Node> m_nodes;

	static std::vector<double> m_globalPVector;
	static std::vector<std::vector<double>> m_globalHMatrix;
	static std::vector<std::vector<double>> m_globalCMatrix;

	static GlobalData m_data;

	static void computeWorker(size_t start, size_t end, Grid* gridPtr, GlobalData* dataPtr);

public:

	Grid(GlobalData data);

	Node getNode(size_t index) {
		return m_nodes.at(index);
	}

	void setNodeTemp(size_t index, double temperature) {
		m_nodes.at(index).setTemp(temperature);
	}

	double getNodeTemp(size_t index) {
		return m_nodes.at(index).getTemp();
	}

	Element getElement(size_t index) {
		return m_elements.at(index);
	}

	Element* getElementPtr(size_t index) {
		return &m_elements.at(index);
	}

	std::vector<std::vector<double>> getGlobalHMatrix() {
		return m_globalHMatrix;
	}	

	std::vector<std::vector<double>> * getGlobalHMatrixPtr() {
		return &m_globalHMatrix;
	}
	
	void addToGlobalHMatrix(size_t i, size_t j, double value) {
		m_globalHMatrix.at(i).at(j) += value;
	}

	std::vector<std::vector<double>> getGlobalCMatrix() {
		return m_globalCMatrix;
	}

	void addToGlobalCMatrix(size_t i, size_t j, double value) {
		m_globalCMatrix.at(i).at(j) += value;
	}

	std::vector<double> getGlobalPVector() {
		return m_globalPVector;
	}

	std::vector<double> * getGlobalPVectorPtr() {
		return &m_globalPVector;
	}

	void addToGlobalPVector(size_t i, double value) {
		m_globalPVector.at(i) += value;
	}

	void compute(std::pair<bool, bool> settings);

};