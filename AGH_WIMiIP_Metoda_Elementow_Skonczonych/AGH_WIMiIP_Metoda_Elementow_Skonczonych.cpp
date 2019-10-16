#include <iostream>
 
#include "Grid.h"


int main() {

	GlobalData global(1.5, 0.5, 6, 4);
	std::cout << global;

	Grid grid(global);
	std::cout << grid;

	std::vector<Element> elements{ grid.getElement(0), grid.getElement(4), grid.getElement(10), grid.getElement(14) };

	for (const auto& el : elements) {
		std::cout << el << "\n";
	}

	std::cout << elements.at(0).getNode(0);
	std::cout << elements.at(0).getNode(1);
	std::cout << elements.at(0).getNode(2);
	std::cout << elements.at(0).getNode(3);
}

