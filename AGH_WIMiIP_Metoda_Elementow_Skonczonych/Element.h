#pragma once
#include <iostream>
#include <vector>
#include <array>
#include <cassert>

#include "Node.h"

class Element {
	std::vector<Node*> m_nodes;
	uint16_t m_edgeBCCount;

	std::array<uint64_t, 4> m_nodeOrder;
	std::array<uint64_t, 2> m_edgeBoundaryConditions;

public:
	Element(Node first, Node second, Node third, Node fourth, std::array<uint64_t, 4> nodeOrder) {
		m_nodes.reserve(4);
		m_nodes.push_back(&first);
		m_nodes.push_back(&second);
		m_nodes.push_back(&third);
		m_nodes.push_back(&fourth);
		m_nodeOrder = nodeOrder;
		m_edgeBCCount = 0;
	}

	Element(Node * first, Node * second, Node * third, Node * fourth, std::array<uint64_t, 4> nodeOrder) {
		m_nodes.reserve(4);
		m_nodes.push_back(first);
		m_nodes.push_back(second);
		m_nodes.push_back(third);
		m_nodes.push_back(fourth);
		m_nodeOrder = nodeOrder;
		m_edgeBCCount = 0;
	}
	
	Element() {
		m_nodes.reserve(4);
		m_nodeOrder = { 0,0,0,0 };
		m_edgeBCCount = 0;
	}

	~Element() {
	}

	Node getNode(size_t index) {
		return (*m_nodes.at(index));
	}

	uint16_t getEdgeCount() {
		return m_edgeBCCount;
	}

	uint64_t getNodeIndex(size_t localIndex) {
		return m_nodeOrder.at(localIndex);
	}

	std::array<uint64_t, 2> getEdgeList() {
		return m_edgeBoundaryConditions;
	}
	
	void setEdgeListValue(uint64_t index, uint64_t value) {
		m_edgeBoundaryConditions.at(index) = value;
	}

	void incrementEdgeCount() {
		assert(m_edgeBCCount < 4);
		m_edgeBCCount++;
	}
};
