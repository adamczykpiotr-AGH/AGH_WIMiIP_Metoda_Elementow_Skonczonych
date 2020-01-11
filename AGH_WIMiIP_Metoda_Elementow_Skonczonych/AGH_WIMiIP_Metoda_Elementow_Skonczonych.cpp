#include <iostream>
#include <iterator>

#include "Grid.h"
#include "Numeric.h"


int main() {

	GlobalData global({ 1.5, 0.5 }, { 6, 4 }, { 0., 0. }, { 14., 0.1 }, {0.1, 0.2, 0.3, 0.4});

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

	/*

	2x + 5y + 3z = 5
	4x + 2y + 5z = 4
	3x + 8y + 4z = 9
	
	*/

	std::vector<std::vector<double>> matrix = { {2,5,3,	5}, {4,2,5,	4}, {3,8,4,	9} };
	std::vector<double> vec = Numeric::gaussianElimination(&matrix);
	for (int i = 0; i < vec.size(); i++) {
		std::cout << "x" << i+1 << " = " << vec[i] << "\n";
	}
	std::cout << "\n";

	std::vector<double> xi = { 0.,5.,5.,0. };
	std::vector<double> yi = { 0.,0.,5.,5. };
	std::vector<double> weight = { 1.,1. };
	std::vector<double> point = { -0.5773502692, 0.5773502692 };

	double area = Numeric::gaussianQuadrature(xi, yi, weight, point);
	std::cout << "Area: " << area << "\n";
}

