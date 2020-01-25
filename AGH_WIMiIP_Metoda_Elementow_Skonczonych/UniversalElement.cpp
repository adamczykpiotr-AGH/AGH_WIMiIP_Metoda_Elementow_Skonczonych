#include "UniversalElement.h"

UniversalElement::UniversalElement() {

	const double val = 1. / sqrt(3);
	m_integrationPoints.at(0).ksi = -val;
	m_integrationPoints.at(0).eta = -val;

	m_integrationPoints.at(1).ksi = -val;
	m_integrationPoints.at(1).eta =  val;

	m_integrationPoints.at(2).ksi =  val;
	m_integrationPoints.at(2).eta =  val;

	m_integrationPoints.at(3).ksi =  val;
	m_integrationPoints.at(3).eta = -val;

	double ksi, eta;

	//Calculating derrivatives of shape functions against ksi/eta for each integration point
	for (size_t i = 0; i < 4; i++) {
		eta = m_integrationPoints.at(i).eta;
		ksi = m_integrationPoints.at(i).ksi;

		m_shapeFunctionValues.at(i).at(0)	=  0.25 * (1 - eta) * (1 - ksi);
		m_EtaMatrix.at(i).at(0)				= -0.25 * (1 - ksi);
		m_KsiMatrix.at(i).at(0)				= -0.25 * (1 - eta);

		m_shapeFunctionValues.at(i).at(3)	=  0.25 * (1 + eta) * (1 - ksi);
		m_EtaMatrix.at(i).at(3)				=  0.25 * (1 - ksi);
		m_KsiMatrix.at(i).at(3)				= -0.25 * (1 + eta);

		m_shapeFunctionValues.at(i).at(2)	=  0.25 * (1 + eta) * (1 + ksi);
		m_EtaMatrix.at(i).at(2)				=  0.25 * (1 + ksi);
		m_KsiMatrix.at(i).at(2)				=  0.25 * (1 + eta);

		m_shapeFunctionValues.at(i).at(1)	 =  0.25 * (1 - eta) * (1 + ksi);
		m_EtaMatrix.at(i).at(1)				= -0.25 * (1 + ksi);
		m_KsiMatrix.at(i).at(1)				=  0.25 * (1 - eta);
	}
}
