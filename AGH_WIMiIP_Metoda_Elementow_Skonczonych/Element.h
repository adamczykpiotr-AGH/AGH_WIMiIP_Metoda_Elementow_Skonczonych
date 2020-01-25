#pragma once
#include <iostream>
#include <vector>
#include <array>
#include <cassert>

#include "Node.h"

class Element {
	std::vector<Node*> m_nodes;
	uint16_t m_edgeBCCount;

	std::array<uint64_t, 4> m_nodeOrder = {};
	std::array<uint16_t, 2> m_edgeBoundaryConditions = {};

public:
	Element(Node first, Node second, Node third, Node fourth, std::array<uint64_t, 4> nodeOrder) {
		m_nodes = { &first, &second, &third, &fourth };
		m_nodeOrder = nodeOrder;
		m_edgeBCCount = 0;
	}

	Element(Node * first, Node * second, Node * third, Node * fourth, std::array<uint64_t, 4> nodeOrder) {
		m_nodes = { first, second, third, fourth };
		m_nodeOrder = nodeOrder;
		m_edgeBCCount = 0;
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

	std::array<uint16_t, 2> getEdgeList() {
		return m_edgeBoundaryConditions;
	}

	uint16_t getSurfaceId(uint16_t surface) {
		return m_edgeBoundaryConditions.at(surface);
	}
	
	void setEdgeListValue(uint64_t index, uint16_t value) {
		m_edgeBoundaryConditions.at(index) = value;
	}

	void incrementEdgeCount() {
		assert(m_edgeBCCount < 4);
		m_edgeBCCount++;
	}
};
