#pragma once
#include <cstdint>	//uint64_t define
#include <ostream>	//std::ostream

class Node {
	uint64_t m_x;
	uint64_t m_y;
	double m_temp;
	uint16_t m_boundaryCondition;

	friend std::ostream& operator<< (std::ostream&, Node);

public:
	Node(uint64_t x, uint64_t y, uint16_t boundaryCondition = 0) {
		m_x = x;
		m_y = y;
		m_temp = .0;
		m_boundaryCondition = boundaryCondition;
	}

	Node() {
		m_x = m_y = 0;
		m_temp = .0;
		m_boundaryCondition = 0;
	}
	
	~Node() {
	}

	uint64_t getX() {
		return m_x;
	}

	uint64_t getY() {
		return m_y;
	}

	double getTemp() {
		return m_temp;
	}

	void setTemp(double temp) {
		m_temp = temp;
	}

	uint16_t getBoundaryCondition() {
		return m_boundaryCondition;
	}

	void setBoundaryCondition(uint16_t boundaryCondition) {
		m_boundaryCondition = boundaryCondition;
	}

};

