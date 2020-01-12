#pragma once
#include <iomanip>
#include <array>
#include <ppl.h>

#include "GlobalData.h"
#include "Node.h"
#include "Element.h"
#include "UniversalElement.h"
#include "Jacobian.h"

class Grid {
	std::vector<Element> m_elements;
	std::vector<Node> m_nodes;

	std::vector<double> m_globalPVector;
	std::vector<std::vector<double>> m_globalHMatrix;
	std::vector<std::vector<double>> m_globalCMatrix;

	static GlobalData m_data;

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
	
	std::vector<std::vector<double>> getGlobalCMatrix() {
		return m_globalCMatrix;
	}

	std::vector<double> getGlobalPVector() {
		return m_globalPVector;
	}

	void compute();

};