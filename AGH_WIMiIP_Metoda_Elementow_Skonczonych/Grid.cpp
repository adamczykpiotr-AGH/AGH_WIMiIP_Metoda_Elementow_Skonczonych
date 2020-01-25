#include "Grid.h"

GlobalData Grid::m_data;

std::vector<double> Grid::m_globalPVector;
std::vector<std::vector<double>> Grid::m_globalHMatrix;
std::vector<std::vector<double>> Grid::m_globalCMatrix;

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
		first  = &m_nodes.at(nodeOrder.at(0));
		second = &m_nodes.at(nodeOrder.at(1));
		third  = &m_nodes.at(nodeOrder.at(2));
		fourth = &m_nodes.at(nodeOrder.at(3));

		std::array<uint16_t, 4> nodeBCs = { first->getBoundaryCondition(), second->getBoundaryCondition(), third->getBoundaryCondition(), fourth->getBoundaryCondition() };
			   
		Element el = Element(first, second, third, fourth, nodeOrder);

		//Compute edge list:

		//Bottom
		if (nodeBCs.at(0) == 1 && nodeBCs.at(1) == 1) {
			if (el.getEdgeCount() == 0) {
				el.setEdgeListValue(0, surfaceBottom);
				el.incrementEdgeCount();
			} else {
				el.setEdgeListValue(1, surfaceBottom);
				el.incrementEdgeCount();
			}
		}
		
		//Right
		if (nodeBCs.at(1) == 1 && nodeBCs.at(2) == 1) {
			if (el.getEdgeCount() == 0) {
				el.setEdgeListValue(0, surfaceRight);
				el.incrementEdgeCount();
			} else {
				el.setEdgeListValue(1, surfaceRight);
				el.incrementEdgeCount();
			}
		}

		//Top
		if (nodeBCs.at(2) == 1 && nodeBCs.at(3) == 1) {
			if (el.getEdgeCount() == 0) {
				el.setEdgeListValue(0, surfaceTop);
				el.incrementEdgeCount();
			} else {
				el.setEdgeListValue(1, surfaceTop);
				el.incrementEdgeCount();
			}
		}

		//Left
		if (nodeBCs.at(3) == 1 && nodeBCs.at(0) == 1) {
			if (el.getEdgeCount() == 0) {
				el.setEdgeListValue(0, surfaceLeft);
				el.incrementEdgeCount();
			} else {
				el.setEdgeListValue(1, surfaceLeft);
				el.incrementEdgeCount();
			}
		}

		m_elements.push_back(el);
	}

	m_data = data; 
}

void Grid::compute(std::pair<bool, bool> settings) {

	size_t newSize = m_data.getMH() * m_data.getML();

	//Set all {P} values to 0.
	m_globalPVector.resize(newSize);
	for (auto& value : m_globalPVector) {
		value = 0.;
	}

	//Set all [C] values to 0.
	m_globalCMatrix.resize(newSize);
	for (auto& vector : m_globalCMatrix) {
		vector.resize(newSize);
		for (auto& value : vector) {
			value = 0.;
		}
	}

	//Set all [H] values to 0.
	m_globalHMatrix.resize(newSize);
	for (auto& vector : m_globalHMatrix) {
		vector.resize(newSize);
		for (auto& value : vector) {
			value = 0.;
		}
	}
	
	//Compute
	computeWorker(0, m_elements.size(), this, &m_data);

	//[H] = [H] + [C]/dT
	double timeStep = m_data.getTimeStep();

	for (size_t i = 0; i < m_globalHMatrix.size(); i++) {
		for (size_t j = 0; j < m_globalHMatrix.size(); j++) {
			m_globalHMatrix.at(i).at(j) += m_globalCMatrix.at(i).at(j) / timeStep;
		}
	}

	//Printing matrices
	if (std::get<1>(settings)) {

		printf("\nC Matrix (.at(C))\n");
		for (int a = 0; a < 16; a++) {
			for (int b = 0; b < 16; b++) {
				printf("%.3f\t", m_globalCMatrix.at(a).at(b));
			}
			printf("\n");
		}

		printf("H Matrix (.at(H)+.at(C)/dT)\n");
		for (int a = 0; a < 16; a++) {
			for (int b = 0; b < 16; b++) {
				printf("%.3f\t", m_globalHMatrix.at(a).at(b));
			}
			printf("\n");
		}

		printf("\nVector ({P})\n");
		for (int a = 0; a < 16; a++) {
			printf("%.3f\t", m_globalPVector.at(a));
		}
		printf("\n");
	}
}

void Grid::computeWorker(size_t start, size_t end, Grid* gridPtr, GlobalData* dataPtr) {

	Grid grid = *(gridPtr);
	GlobalData m_data = *(dataPtr);

	double timeStep = m_data.getTimeStep();

	double ambientTemperature = m_data.getAmbientTemp();

	double conductivityCoefficient = m_data.getMaterialConductivityCoefficient();
	double heatExchangeCoefficient = m_data.getMaterialHeatExchangeCoefficient();

	double Q = m_data.getMaterialHeatCapacity() * m_data.getMaterialDensity();
	UniversalElement universalElement;

	for (size_t i = start; i < end; i++) {

		Element localElement = grid.getElement(i);

		std::array<double, 4> dN_dX;
		std::array<double, 4> dN_dY;

		std::array<double, 4> coordX;
		std::array<double, 4> coordY;

		std::array<double, 4> initialTemps;
		
		//Local [H], [C] and {P}
		std::array<std::array<double, 4>, 4> localHMatrix = { 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0. };
		std::array<std::array<double, 4>, 4> localCMatrix = { 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0. };
		std::array<double, 4>				 localPVector = { 0. ,0., 0., 0. };

		//Get all nodes parameters from element
		for (size_t i = 0; i < 4; i++) {
			uint64_t nodeIndex = localElement.getNodeIndex(i);
			Node localNode = grid.getNode(nodeIndex);
			coordX.at(i) = localNode.getX();
			coordY.at(i) = localNode.getY();
			initialTemps.at(i) = localNode.getTemp();
		}

		for (size_t i = 0; i < 4; i++) {
			Jacobian jacobian = Jacobian(coordX, coordY, i, universalElement);
			double det = jacobian.getDeterminant();
		
			double temperature = 0.;
			for (size_t j = 0; j < 4; j++) {

				//Convert to local coordinate system
				dN_dX.at(j) = jacobian.getInvertedJacobianValue(0, 0) * universalElement.getKsiValue(i, j)
							+ jacobian.getInvertedJacobianValue(0, 1) * universalElement.getEtaValue(i, j);

				dN_dY.at(j) = jacobian.getInvertedJacobianValue(1, 0) * universalElement.getKsiValue(i, j)
							+ jacobian.getInvertedJacobianValue(1, 1) * universalElement.getEtaValue(i, j);

				//Interpolate temperature using functional
				temperature += initialTemps.at(j) * universalElement.getShapeFunctionValue(j, i);
			}

			double temperatureOverTimeStep = temperature / timeStep;

			//Volume integration
			for (size_t j = 0; j < 4; j++) {
				for (size_t k = 0; k < 4; k++) {
					localCMatrix.at(j).at(k) += Q * universalElement.getShapeFunctionValue(i, j) * universalElement.getShapeFunctionValue(i, k) * det;
					localHMatrix.at(j).at(k) += conductivityCoefficient * (dN_dX.at(j) * dN_dX.at(k) + dN_dY.at(j) * dN_dY.at(k)) * det;
					localPVector.at(j)		 += Q * universalElement.getShapeFunctionValue(i, j) * universalElement.getShapeFunctionValue(i, k) * det * temperatureOverTimeStep;
				}
			}
		}

		Surface surface(localElement);

		//Surface integration
		for (uint16_t elementSurface = 0; elementSurface < localElement.getEdgeCount(); elementSurface++) {

			uint16_t surfaceId = localElement.getSurfaceId(elementSurface);
			double surfaceDet = surface.getSurfaceDeterminant(surfaceId);

			std::array<double, 4> shapeFunction;

			//2 point integration
			for (int integrationPoint = 0; integrationPoint < 2; integrationPoint++) {
				shapeFunction = surface.getShapeFunction(surfaceId, integrationPoint);

				for (int j = 0; j < 4; j++) {

					for (int k = 0; k < 4; k++) {
						localHMatrix.at(j).at(k) += heatExchangeCoefficient * shapeFunction.at(j) * shapeFunction.at(k) * surfaceDet;
					}

					localPVector.at(j) += heatExchangeCoefficient * shapeFunction.at(j) * surfaceDet * ambientTemperature;
				}
			}
		}

		//Aggregation
		for (size_t i = 0; i < 4; i++) {

			uint64_t globalIndexI = localElement.getNodeIndex(i);

			for (size_t j = 0; j < 4; j++) {
				uint64_t globalIndexj = localElement.getNodeIndex(j);

				grid.addToGlobalHMatrix(globalIndexI, globalIndexj, localHMatrix.at(i).at(j));
				grid.addToGlobalCMatrix(globalIndexI, globalIndexj, localCMatrix.at(i).at(j));
			}

			grid.addToGlobalPVector(globalIndexI, localPVector.at(i));
		}
	}
}