#pragma once
#include <cstdint>	//uint64_t define
#include <ostream>	//std::ostream
#include <iomanip>
#include <iostream>

class Node {
	double m_x;
	double m_y;
	double m_temp;
	uint16_t m_boundaryCondition;

	friend std::ostream& operator<< (std::ostream&, Node);

public:
	Node(double x, double y, double temp, uint16_t boundaryCondition = 0) {
		m_x = x;
		m_y = y;
		m_temp = temp;
		m_boundaryCondition = boundaryCondition;
	}

	Node() {
		m_x = m_y = 0;
		m_temp = .0;
		m_boundaryCondition = 0;
	}
	
	~Node() {
	}

	double getX() {
		return m_x;
	}

	double getY() {
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

