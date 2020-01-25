#include "Jacobian.h"
#include "UniversalElement.h"
#include <iostream>

Jacobian::Jacobian(std::array<double, 4> x, std::array<double, 4> y, size_t point, UniversalElement element) {

	double dX_dKsi =
		  x.at(0) * element.getKsiValue(point, 0)
		+ x.at(1) * element.getKsiValue(point, 1)
		+ x.at(2) * element.getKsiValue(point, 2)
		+ x.at(3) * element.getKsiValue(point, 3);
	double dY_dKsi =
		  y.at(0) * element.getKsiValue(point, 0)
		+ y.at(1) * element.getKsiValue(point, 1)
		+ y.at(2) * element.getKsiValue(point, 2)
		+ y.at(3) * element.getKsiValue(point, 3);
	double dX_dEta =
		  x.at(0) * element.getEtaValue(point, 0)
		+ x.at(1) * element.getEtaValue(point, 1)
		+ x.at(2) * element.getEtaValue(point, 2)
		+ x.at(3) * element.getEtaValue(point, 3);
	double dY_dEta =
		  y.at(0) * element.getEtaValue(point, 0)
		+ y.at(1) * element.getEtaValue(point, 1)
		+ y.at(2) * element.getEtaValue(point, 2)
		+ y.at(3) * element.getEtaValue(point, 3);

	//Setting jacobian values
	m_data.at(0).at(0) = dX_dKsi;
	m_data.at(0).at(1) = dY_dKsi;
	m_data.at(1).at(0) = dX_dEta;
	m_data.at(1).at(1) = dY_dEta;

	//Calculating determinant
	m_determinant  = dX_dKsi * dY_dEta;
	m_determinant -= dY_dKsi * dX_dEta;

	//Calculating inverted jacobian values
	m_invertedData.at(0).at(0) =	  dY_dEta / m_determinant;
	m_invertedData.at(0).at(1) = -1 * dY_dKsi / m_determinant;
	m_invertedData.at(1).at(0) = -1 * dX_dEta / m_determinant;
	m_invertedData.at(1).at(1) =	  dX_dKsi / m_determinant;
}
