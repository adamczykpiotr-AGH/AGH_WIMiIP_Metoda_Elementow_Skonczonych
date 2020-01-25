#pragma once
#include <array>

#include "Element.h"

constexpr auto surfaceBottom = 1;
constexpr auto surfaceRight  = 2;
constexpr auto surfaceTop	 = 3;
constexpr auto surfaceLeft	 = 4;

class Surface {

	std::array<double, 4> m_xCoordinates;
	std::array<double, 4> m_yCoordinates;

public:

	Surface(Element element);

	double getEdgeLength(uint16_t surfaceId);
	double getSurfaceDeterminant(uint16_t surfaceId);

	std::array<double, 4> getShapeFunction(uint16_t edge, uint16_t integrationPoint);
};

