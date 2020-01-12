#pragma once
#include <iostream>
#include <vector>
#include <array>

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
	}

	Element(Node * first, Node * second, Node * third, Node * fourth, std::array<uint64_t, 4> nodeOrder) {
		m_nodes.reserve(4);
		m_nodes.push_back(first);
		m_nodes.push_back(second);
		m_nodes.push_back(third);
		m_nodes.push_back(fourth);
		m_nodeOrder = nodeOrder;
	}
	
	Element() {
		m_nodes.reserve(4);
		m_nodeOrder = { 0,0,0,0 };
	}

	~Element() {
	}

	Node getNode(size_t index) {
		return (*m_nodes.at(index));
	}

	uint16_t getEdgeCount() {
		return m_edgeBCCount;
	}

	std::array<uint64_t, 4> getNodeOrder() {
		return m_nodeOrder;
	}

	uint64_t getNodeIndex(size_t localIndex) {
		return m_nodeOrder.at(localIndex);
	}

	std::array<uint64_t, 2> getEdgeList() {
		return m_edgeBoundaryConditions;
	}
	
	void updateEdgeList(uint64_t index, uint64_t value) {
		m_edgeBoundaryConditions.at(index) = value;
	}

	void setEdgeCount(uint64_t count) {
		m_edgeBCCount = count;
	}

};
