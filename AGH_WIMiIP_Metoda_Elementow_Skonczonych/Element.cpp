#include "Element.h"

std::ostream& operator<<(std::ostream& input, Element element) {
	input << "Element:\n\n";

	input << " (" << element.getNode(3).getX() << ";" << element.getNode(3).getY() << ") ------ (" << element.getNode(2).getX() << ";" << element.getNode(2).getY() << ")\n\n";
	input << " |    element    | \n\n";
	input << " (" << element.getNode(0).getX() << ";" << element.getNode(0).getY() << ") ------ (" << element.getNode(1).getX() << ";" << element.getNode(1).getY() << ")\n\n";

	return input;
}