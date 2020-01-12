#include "Grid.h"

GlobalData Grid::m_data;

Grid::Grid(GlobalData data) {
	m_elements.reserve(static_cast<size_t>(data.getME()));
	m_nodes.reserve(static_cast<size_t>(data.getMW()));

	uint64_t mH = data.getMH();
	uint64_t mL = data.getML();

	double height = data.getHeight();
	double length = data.getLength();

	double initialTemp = data.getInitialTemp();

	//Fill Nodes and set BC
	for (uint64_t i = 0; i < data.getMW(); i++) {

		uint64_t x = static_cast<uint64_t>(floor(i / mH));
		uint64_t y = i % mH;
		uint16_t boundaryCondition = 0;

		//most lower / most upper / most left / most right edge
		if (x == 0 || x == mH - 1 || y == 0 || y == mL - 1) {
			boundaryCondition = 1;
		}

		double xReal = x;
		xReal /= (mL - 1);
		xReal *= height;

		double yReal = y;
		yReal /= (mH - 1);
		yReal *= length;

		m_nodes.push_back(Node(xReal, yReal, initialTemp, boundaryCondition));
	}

	//Construct Elements
	uint64_t maxIndex = data.getME();
	for (uint64_t i = 0; i < maxIndex; i++) {

		//skip highest row
		if (i % mH == mH - 1) {
			maxIndex++;
			continue;
		}

		//anti clockwise direction
		std::array<uint64_t, 4> nodeOrder = { i, i + mH, i + mH + 1, i + 1 };

		Node* first, * second, * third, * fourth;
		first  = &m_nodes[nodeOrder[0]];
		second = &m_nodes[nodeOrder[1]];
		third  = &m_nodes[nodeOrder[2]];
		fourth = &m_nodes[nodeOrder[3]];

		std::array<uint16_t, 4> nodeBCs = { first->getBoundaryCondition(), second->getBoundaryCondition(), third->getBoundaryCondition(), fourth->getBoundaryCondition() };
			   
		Element el = Element(first, second, third, fourth, nodeOrder);

		//Compute edge list:

		//Bottom
		if (nodeBCs[0] == 1 && nodeBCs[1] == 1) {
			if (el.getEdgeCount() == 0) {
				el.setEdgeListValue(0, 1);
				el.incrementEdgeCount();
			} else {
				el.setEdgeListValue(1, 1);
				el.incrementEdgeCount();
			}
		}
		
		//Right
		if (nodeBCs[1] == 1 && nodeBCs[2] == 1) {
			if (el.getEdgeCount() == 0) {
				el.setEdgeListValue(0, 2);
				el.incrementEdgeCount();
			} else {
				el.setEdgeListValue(1, 2);
				el.incrementEdgeCount();
			}
		}

		//Top
		if (nodeBCs[2] == 1 && nodeBCs[3] == 1) {
			if (el.getEdgeCount() == 0) {
				el.setEdgeListValue(0, 3);
				el.incrementEdgeCount();
			} else {
				el.setEdgeListValue(1, 3);
				el.incrementEdgeCount();
			}
		}

		//Left
		if (nodeBCs[3] == 1 && nodeBCs[0] == 1) {
			if (el.getEdgeCount() == 0) {
				el.setEdgeListValue(0, 4);
				el.incrementEdgeCount();
			} else {
				el.setEdgeListValue(1, 4);
				el.incrementEdgeCount();
			}
		}

		m_elements.push_back(el);
	}

	m_data = data; 
}


//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!



void Grid::compute() {

	double heatCapacity = m_data.getMaterialHeatCapacity();
	double density = m_data.getMaterialDensity();

	double conductivityCoefficient = m_data.getMaterialConductivityCoefficient();

	double timeStep = m_data.getTimeStep();

	double heatExchangeCoefficient = m_data.getMaterialHeatExchangeCoefficient();
	double ambientTemperature = m_data.getAmbientTemp();

	UniversalElement* uElem = new UniversalElement();

	UniversalElement universal;

	//Setting up global matrices and vector
	size_t newSize = m_data.getMH();
	newSize *= m_data.getML();

	//Global P vector
	m_globalPVector.resize(newSize);
	for (size_t i = 0; i < newSize; i++) {
		m_globalPVector[i] = 0.;
	}

	//Global H matrix
	m_globalHMatrix.resize(newSize);
	for (size_t i = 0; i < newSize; i++) {
		m_globalHMatrix[i].resize(newSize);
		for (size_t j = 0; j < newSize; j++) {
			m_globalHMatrix[i][j] = 0.;
		}
	}

	// Global C matrix
	m_globalCMatrix.resize(newSize);
	for (size_t i = 0; i < newSize; i++) {
		m_globalCMatrix[i].resize(newSize);
		for (size_t j = 0; j < newSize; j++) {
			m_globalCMatrix[i][j] = 0.;
		}
	}

	//XDDDDDDDDDDDDDD
#ifdef para
	for (Element& localElement : m_elements) {
#endif // para
#ifndef para
		concurrency::parallel_for_each(m_elements.begin(), m_elements.end(), [heatCapacity, density, conductivityCoefficient, timeStep, heatExchangeCoefficient, ambientTemperature, this, uElem](Element& localElement) {
#endif // !para




			std::array<double, 4> coordX;
			std::array<double, 4> coordY;

			double dNdx[4];
			double dNdy[4];

			double initialTemp[4];
			double tempInt = 0.;
			double det = 0.;

			//Local H matrix, P vector and C matrix
			std::array<std::array<double, 4>, 4> hLocal = { 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0. };
			std::array<std::array<double, 4>, 4> hsLocal = { 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0. };
			std::array<std::array<double, 4>, 4> cLocal = { 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0. };
			std::array<double, 4> pLocal = { 0. ,0., 0., 0. };


			// Gets all nodes parameters from element
			for (int j = 0; j < 4; j++) {
				uint64_t nodeIndex = localElement.getNodeIndex(j);
				Node localNode = getNode(nodeIndex);
				coordX[j] = localNode.getX();
				coordY[j] = localNode.getY();
				initialTemp[j] = localNode.getTemp();
			}

			// Tworzy macierze jakobiego i wyznacza pochodne funkcji ksztaltu po x/y + interpolacja temp
			// J - punkt calkowania
			for (int j = 0; j < 4; j++) {
				Jacobian jacobian = Jacobian(coordX, coordY, j, *uElem);
				tempInt = 0.;
				for (int k = 0; k < 4; k++) {

					dNdx[k] = jacobian.getInvertedJacobianValue(0, 0) * uElem->getKsiValue(j, k)
						+ jacobian.getInvertedJacobianValue(0, 1) * uElem->getEtaValue(j, k);

					dNdy[k] = jacobian.getInvertedJacobianValue(1, 0) * uElem->getKsiValue(j, k)
						+ jacobian.getInvertedJacobianValue(1, 1) * uElem->getEtaValue(j, k);

					tempInt += initialTemp[k] * uElem->getShapeFunctionValue(k, j);
				}

				det = jacobian.getDeterminant();

				// N x N^T
				// Calka objetosciowa do H i C
				for (int k = 0; k < 4; k++) {
					for (int l = 0; l < 4; l++) {

						// C lokalne
						cLocal[k][l] += heatCapacity * density * uElem->getShapeFunctionValue(j, k) * uElem->getShapeFunctionValue(j, l) * det;
						// H lokalne
						hLocal[k][l] += conductivityCoefficient * (dNdx[k] * dNdx[l] + dNdy[k] * dNdy[l]) * det;
						// Pc lokalnie
						pLocal[k] += (heatCapacity * density * uElem->getShapeFunctionValue(j, k) * uElem->getShapeFunctionValue(j, l) * det) / timeStep * tempInt;
					}

				}
			}

			//Calka powierzchniowa do H i wektor P
			for (int n_surf = 0; n_surf < localElement.getEdgeCount(); n_surf++) {

				//FIXME 
				uint64_t surface_id = localElement.getEdgeList()[n_surf];
				double edge_length, surf_det, x_cords[4], y_cords[4], shape_func[4], ksi, eta;
				// Wspolrzedne elementu skonczonego
				for (int cords = 0; cords < 4; cords++) {
					x_cords[cords] = localElement.getNode(cords).getX();
					y_cords[cords] = localElement.getNode(cords).getY();
				}

				// Obliczanie dlugosci boku
				if (surface_id == 1) {
					edge_length = sqrt(pow((x_cords[1] - x_cords[0]), 2) + pow((y_cords[1] - y_cords[0]), 2));
				}
				else if (surface_id == 2) {
					edge_length = sqrt(pow((x_cords[1] - x_cords[2]), 2) + pow((y_cords[1] - y_cords[2]), 2));
				}
				else if (surface_id == 3) {
					edge_length = sqrt(pow((x_cords[2] - x_cords[3]), 2) + pow((y_cords[2] - y_cords[3]), 2));
				}
				else if (surface_id == 4) {
					edge_length = sqrt(pow((x_cords[3] - x_cords[0]), 2) + pow((y_cords[3] - y_cords[0]), 2));
				}

				// lokalny wyznacznik macierzy jakobiego
				surf_det = edge_length / 2.;

				shape_func[0] = 0.;
				shape_func[1] = 0.;
				shape_func[2] = 0.;
				shape_func[3] = 0.;

				// Dla 2 pkt calkowania
				for (int i_point = 0; i_point < 2; i_point++) {
					for (int j = 0; j < 4; j++) {
						for (int k = 0; k < 4; k++) {


							// mala zlozonosc cyklomatyczna
							if (surface_id == 1) {
								if (i_point == 0) {
									ksi = -1. / sqrt(3);
									eta = -1;
								}
								else {
									ksi = 1. / sqrt(3);
									eta = -1;
								}
								// N1 + N2
								shape_func[0] = 0.25 * (1 - ksi) * (1 - eta);
								shape_func[1] = 0.25 * (1 + ksi) * (1 - eta);
								shape_func[2] = 0.;
								shape_func[3] = 0.;
							}
							else if (surface_id == 2) {
								if (i_point == 0) {
									ksi = 1;
									eta = -1. / sqrt(3);
								}
								else {
									ksi = 1;
									eta = 1. / sqrt(3);
								}
								// N2 + //N3
								shape_func[0] = 0.;
								shape_func[1] = 0.25 * (1 + ksi) * (1 - eta);
								shape_func[2] = 0.25 * (1 + ksi) * (1 + eta);
								shape_func[3] = 0.;

							}
							else if (surface_id == 3) {
								if (i_point == 0) {
									ksi = 1. / sqrt(3);
									eta = 1;
								}
								else {
									ksi = -1. / sqrt(3);
									eta = 1;
								}
								// N3 + //N4
								shape_func[0] = 0.;
								shape_func[1] = 0.;
								shape_func[2] = 0.25 * (1 + ksi) * (1 + eta);
								shape_func[3] = 0.25 * (1 - ksi) * (1 + eta);
							}
							else {
								if (i_point == 0) {
									ksi = -1;
									eta = 1. / sqrt(3);
								}
								else {
									ksi = -1;
									eta = -1. / sqrt(3);
								}
								// N4 + N1
								shape_func[0] = 0.25 * (1 - ksi) * (1 - eta);
								shape_func[1] = 0.;
								shape_func[2] = 0.;
								shape_func[3] = 0.25 * (1 - ksi) * (1 + eta);
							}
							/*


							switch (surface_id) {
								case 1: {
									ksi = 1. / sqrt(3);
									eta = -1;
									if (i_point == 0) ksi *= -1;

									// N1 + N2
									shape_func[0] = 0.25 * (1 - ksi) * (1 - eta);
									shape_func[1] = 0.25 * (1 + ksi) * (1 - eta);
									shape_func[2] = 0.;
									shape_func[3] = 0.;
									break;
								}
								case 2: {
									ksi = 1;
									eta = 1. / sqrt(3);
									if (i_point == 0) eta *= -1;

									// N2 + N3
									shape_func[0] = 0.;
									shape_func[1] = 0.25 * (1 + ksi) * (1 - eta);
									shape_func[2] = 0.25 * (1 + ksi) * (1 + eta);
									shape_func[3] = 0.;
									break;

								}
								case 3: {
									ksi = -1. / sqrt(3);
									eta = 1;
									if (i_point == 0) ksi *= -1;

									// N3 + N4
									shape_func[0] = 0.;
									shape_func[1] = 0.;
									shape_func[2] = 0.25 * (1 + ksi) * (1 + eta);
									shape_func[3] = 0.25 * (1 - ksi) * (1 + eta);
									break;
								}
								default: {
									ksi = -1;
									eta = -1. / sqrt(3);
									if (i_point == 0) eta *= -1;
									ksi = 1;

									// N4 + N1
									shape_func[0] = 0.25 * (1 - ksi) * (1 - eta);
									shape_func[1] = 0.;
									shape_func[2] = 0.;
									shape_func[3] = 0.25 * (1 - ksi) * (1 + eta);
								}

							}*/

							// Macierz H po powierzchni
							hLocal[j][k] += heatExchangeCoefficient * shape_func[j] * shape_func[k] * surf_det;
						}
						pLocal[j] += heatExchangeCoefficient * shape_func[j] * surf_det * ambientTemperature;
					}
				}
			}

			// Agregacja
			for (int a = 0; a < 4; a++) {
				for (int b = 0; b < 4; b++) {
					// Indeks i
					uint64_t i_index = localElement.getNodeIndex(a);
					// Indeks j
					uint64_t j_index = localElement.getNodeIndex(b);
					// Dodanie do tablic globalnych
					// Dodawanie do H globalnej
					m_globalHMatrix[i_index][j_index] += hLocal[a][b];
					// Dodawanie do C globalnej
					m_globalCMatrix[i_index][j_index] += cLocal[a][b];
				}
				// Dodawanie do P globalnego
				m_globalPVector[localElement.getNodeIndex(a)] += pLocal[a];
			}

#ifdef para
		}
#endif // para
#ifndef para
		}); 
#endif // !para

	
	// H + C/dT

	for (int a = 0; a < m_globalHMatrix.size(); a++) {
		for (int b = 0; b < m_globalHMatrix.size(); b++) {
			m_globalHMatrix[a][b] += m_globalCMatrix[a][b] / timeStep;
		}
	}

	return;
	// Wypisywanie tablic
	bool display_global_matrixes = true;
	if (display_global_matrixes == true) {
		std::cout << std::setprecision(2);
		std::cout << "MACIERZ H+C" << std::endl;
		for (int a = 0; a < 16; a++) {
			for (int b = 0; b < 16; b++) {
				printf("%.3f ",m_globalHMatrix[a][b]);
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
		std::cout << "MACIERZ C" << std::endl;
		for (int a = 0; a < 16; a++) {
			for (int b = 0; b < 16; b++) {
				printf("%.3f ", m_globalCMatrix[a][b]);
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
		std::cout << "WEKTOR P" << std::endl;
		for (int a = 0; a < 16; a++) {
			printf("%.3f ", m_globalPVector[a]);
		}
		std::cout << std::endl;
	}

	delete uElem;


}


