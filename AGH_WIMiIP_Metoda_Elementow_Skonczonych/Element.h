#pragma once
#include <iostream>
#include <vector>

#include "Node.h"

class Element {
	std::vector<Node*> m_nodes;

	friend std::ostream& operator<< (std::ostream&, Element&);

public:
	Element(Node first, Node second, Node third, Node fourth) {
		m_nodes.reserve(4);
		m_nodes.push_back(&first);
		m_nodes.push_back(&second);
		m_nodes.push_back(&third);
		m_nodes.push_back(&fourth);
	}

	Element(Node * first, Node * second, Node * third, Node * fourth) {
		m_nodes.reserve(4);
		m_nodes.push_back(first);
		m_nodes.push_back(second);
		m_nodes.push_back(third);
		m_nodes.push_back(fourth);
	}
	

	Element() {
		m_nodes.reserve(4);
	}

	~Element() {
	}

	Node getNode(size_t index) {
		return (*m_nodes.at(index));
	}
};
