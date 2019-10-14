#include <iostream>
 
#include "Grid.h"


int main() {

	GlobalData global(1.5, 0.5, 6., 4.);
	std::cout << global;

	Grid grid(global);
	std::cout << grid;

	Element el = grid.getElement(14);
	std::cout << el;
	Node el1_0 = el.getNode(0);
	std::cout << el1_0;
	Node el1_1 = el.getNode(1);
	std::cout << el1_1;
	Node el1_2 = el.getNode(2);
	std::cout << el1_2;
	Node el1_3 = el.getNode(3);
	std::cout << el1_3;
}

