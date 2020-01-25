#include "Jacobian.h"
#include "UniversalElement.h"
#include <iostream>

Jacobian::Jacobian(std::array<double, 4> x, std::array<double, 4> y, size_t point, UniversalElement element) {

	double dX_dKsi =
		  x[0] * element.getKsiValue(point, 0)
		+ x[1] * element.getKsiValue(point, 1)
		+ x[2] * element.getKsiValue(point, 2)
		+ x[3] * element.getKsiValue(point, 3);
	double dY_dKsi =
		  y[0] * element.getKsiValue(point, 0)
		+ y[1] * element.getKsiValue(point, 1)
		+ y[2] * element.getKsiValue(point, 2)
		+ y[3] * element.getKsiValue(point, 3);
	double dX_dEta =
		  x[0] * element.getEtaValue(point, 0)
		+ x[1] * element.getEtaValue(point, 1)
		+ x[2] * element.getEtaValue(point, 2)
		+ x[3] * element.getEtaValue(point, 3);
	double dY_dEta =
		  y[0] * element.getEtaValue(point, 0)
		+ y[1] * element.getEtaValue(point, 1)
		+ y[2] * element.getEtaValue(point, 2)
		+ y[3] * element.getEtaValue(point, 3);

	//Setting jacobian values
	m_data[0][0] = dX_dKsi;
	m_data[0][1] = dY_dKsi;
	m_data[1][0] = dX_dEta;
	m_data[1][1] = dY_dEta;

	//Calculating determinant
	m_determinant  = dX_dKsi * dY_dEta;
	m_determinant -= dY_dKsi * dX_dEta;

	//Calculating inverted jacobian values
	m_invertedData[0][0] =		dY_dEta / m_determinant;
	m_invertedData[0][1] = -1 * dY_dKsi / m_determinant;
	m_invertedData[1][0] = -1 * dX_dEta / m_determinant;
	m_invertedData[1][1] =		dX_dKsi / m_determinant;
}
