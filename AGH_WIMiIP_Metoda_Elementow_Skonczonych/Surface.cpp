#include "Surface.h"

Surface::Surface(Element element) {

	for (size_t i = 0; i < 4; i++) {
		m_xCoordinates[i] = element.getNode(i).getX();
		m_yCoordinates[i] = element.getNode(i).getY();
	}
}

double Surface::getEdgeLength(uint16_t surfaceId) {

	size_t firstNodeIndex, secondNodeIndex;

	switch (surfaceId) {
		case surfaceBottom: {
			firstNodeIndex = 1;
			secondNodeIndex = 0;
			break;
		}

		case surfaceRight: {
			firstNodeIndex = 1;
			secondNodeIndex = 2;
			break;
		}

		case surfaceTop: {
			firstNodeIndex = 2;
			secondNodeIndex = 3;
			break;
		}

		case surfaceLeft: {
			firstNodeIndex = 3;
			secondNodeIndex = 0;
			break;
		}
	
		default: {
			firstNodeIndex = 0;
			secondNodeIndex = 0;
		}	
	}

	double xDiff = m_xCoordinates.at(firstNodeIndex) - m_xCoordinates.at(secondNodeIndex);
	double yDiff = m_yCoordinates.at(firstNodeIndex) - m_yCoordinates.at(secondNodeIndex);

	return sqrt(xDiff * xDiff + yDiff * yDiff);
}

std::array<double, 4> Surface::getShapeFunction(uint16_t surfaceId, uint16_t integrationPoint) {

	double ksi, eta;
	std::array<double, 4> shapeFunction;

	switch (surfaceId) {
		case surfaceBottom : {

			if (integrationPoint == 0) {
				ksi = -1. / sqrt(3);
				eta = -1;
			} else {
				ksi = 1. / sqrt(3);
				eta = -1;
			}

			//N = N1 + N2
			shapeFunction[0] = 0.25 * (1 - ksi) * (1 - eta);
			shapeFunction[1] = 0.25 * (1 + ksi) * (1 - eta);
			shapeFunction[2] = 0.;
			shapeFunction[3] = 0.;
	
			break;
		}
	
		case surfaceRight: {

			if (integrationPoint == 0) {
				ksi = 1;
				eta = -1. / sqrt(3);
			} else {
				ksi = 1;
				eta = 1. / sqrt(3);
			}

			//N = N2 + N3
			shapeFunction[0] = 0.;
			shapeFunction[1] = 0.25 * (1 + ksi) * (1 - eta);
			shapeFunction[2] = 0.25 * (1 + ksi) * (1 + eta);
			shapeFunction[3] = 0.;
			
			break;
		}

		case surfaceTop: {

			if (integrationPoint == 0) {
				ksi = 1. / sqrt(3);
				eta = 1;
			} else {
				ksi = -1. / sqrt(3);
				eta = 1;
			}

			//N = N3 + N4
			shapeFunction[0] = 0.;
			shapeFunction[1] = 0.;
			shapeFunction[2] = 0.25 * (1 + ksi) * (1 + eta);
			shapeFunction[3] = 0.25 * (1 - ksi) * (1 + eta);

			break;
		}

		case surfaceLeft: {

			if (integrationPoint == 0) {
				ksi = -1;
				eta = 1. / sqrt(3);
			}
			else {
				ksi = -1;
				eta = -1. / sqrt(3);
			}

			//N = N4 + N1
			shapeFunction[0] = 0.25 * (1 - ksi) * (1 - eta);
			shapeFunction[1] = 0.;
			shapeFunction[2] = 0.;
			shapeFunction[3] = 0.25 * (1 - ksi) * (1 + eta);

			break;
		}

		default: {
			shapeFunction = {0}; 
		}
	}

	return shapeFunction;
}

double Surface::getSurfaceDeterminant(uint16_t surfaceId) {

	//dX / dKsi = (x2 - x1) / (1 - (-1))
	return 0.5 * getEdgeLength(surfaceId);
}
