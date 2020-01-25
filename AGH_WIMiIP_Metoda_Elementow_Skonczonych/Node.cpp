#include "Node.h"

std::ostream& operator<<(std::ostream& input, Node node) {
	input << "Node:\n\n";
	input << "X:\t"		<< node.m_x << "\n";
	input << "Y:\t"		<< node.m_y << "\n";
	input << "Temp:\t"	<< node.m_temp << "\n";
	input << "BC:\t"	<< node.m_boundaryCondition << "\n";
	input << "\n";

	return input;
}