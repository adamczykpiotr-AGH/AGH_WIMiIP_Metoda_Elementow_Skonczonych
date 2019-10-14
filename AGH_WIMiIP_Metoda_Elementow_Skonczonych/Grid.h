#pragma once
#include <ostream>	//std::ostream
#include <iomanip>	//std::setw, std::setfill

#include "GlobalData.h"
#include "Node.h"
#include "Element.h"

class Grid {
	std::vector<Element> m_elements;
	std::vector<Node> m_nodes;

	static GlobalData m_data;

	friend std::ostream& operator<< (std::ostream&, Grid&);

public:

	Grid(GlobalData data) {
		m_elements.reserve(static_cast<size_t>(data.getME()));
		m_nodes.reserve(static_cast<size_t>(data.getMW()));

		uint64_t mH = data.getMH();
		uint64_t mL = data.getML();

		//Fill Nodes and set BC
		for (uint64_t i = 0; i < data.getMW(); i++) {

			uint64_t x = floor(i / mH);
			uint64_t y = i % mH;
			uint16_t boundaryCondition = 0;

			if (x == 0 || x == mH - 1 || y == 0 || y == mL - 1) {
				boundaryCondition = 1;
			}

			m_nodes.push_back(Node(x, y, boundaryCondition));
		}
		
		//Construct Elements
		uint64_t maxIndex = data.getME();
		for (uint64_t i = 0; i < maxIndex; i++) {
			
			//skip highest row
			if (i % mH == mH - 1) {
				maxIndex++;
				continue;
			}
	
			Node * first, * second, * third, * fourth;
			first = &m_nodes[i];
			second = &m_nodes[i + mH];
			third = &m_nodes[i + mH + 1]; 
			fourth = &m_nodes[i + 1];

			//std::cout << first;

			Element el = Element(first, second, third, fourth);

			m_elements.push_back(el);
		}

		m_data = data;
	}
	
	Node getNode(size_t index) {
		return m_nodes.at(index);
	}

	Element getElement(size_t index) {
		return m_elements.at(index);
	}

};