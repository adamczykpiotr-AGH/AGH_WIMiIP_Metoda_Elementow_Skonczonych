#include "Jacobian.h"
#include "UniversalElement.h"
#include <iostream>

Jacobian::Jacobian(std::array<double, 4> x, std::array<double, 4> y, int point, UniversalElement element) {

	double dxDksi =
		  x[0] * element.getKsiValue(point, 0)
		+ x[1] * element.getKsiValue(point, 1)
		+ x[2] * element.getKsiValue(point, 2)
		+ x[3] * element.getKsiValue(point, 3);
	double dyDksi =
		  y[0] * element.getKsiValue(point, 0)
		+ y[1] * element.getKsiValue(point, 1)
		+ y[2] * element.getKsiValue(point, 2)
		+ y[3] * element.getKsiValue(point, 3);
	double dxDeta =
		  x[0] * element.getEtaValue(point, 0)
		+ x[1] * element.getEtaValue(point, 1)
		+ x[2] * element.getEtaValue(point, 2)
		+ x[3] * element.getEtaValue(point, 3);
	double dyDeta =
		  y[0] * element.getEtaValue(point, 0)
		+ y[1] * element.getEtaValue(point, 1)
		+ y[2] * element.getEtaValue(point, 2)
		+ y[3] * element.getEtaValue(point, 3);

	//Setting jacobian values
	m_data[0][0] = dxDksi;
	m_data[0][1] = dyDksi;
	m_data[1][0] = dxDeta;
	m_data[1][1] = dyDeta;

	//Calculating determinant
	m_determinant  = m_data[0][0] * m_data[1][1];
	m_determinant -= m_data[0][1] * m_data[1][0];

	//Calculating inverted jacobian values
	m_invertedData[0][0] =		m_data[1][1] / m_determinant;
	m_invertedData[0][1] = -1 * m_data[0][1] / m_determinant;
	m_invertedData[1][0] = -1 * m_data[1][0] / m_determinant;
	m_invertedData[1][1] =		m_data[0][0] / m_determinant;
}
