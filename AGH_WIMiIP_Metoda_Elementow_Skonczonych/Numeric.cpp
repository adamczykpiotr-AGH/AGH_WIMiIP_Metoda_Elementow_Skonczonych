#include "Numeric.h"

double Numeric::gaussianQuadrature(const std::vector<double>& xi, const std::vector<double>& yi, const std::vector<double>& weight, const std::vector<double>& point) {

	//2 points only solution

	double XiDerr[2][4];
	double NiDerr[2][4];

	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {

			XiDerr[j][0] = -0.25 * (1.0 - point[j]);
			XiDerr[j][1] = 0.25 * (1.0 - point[j]);
			XiDerr[j][2] = 0.25 * (1.0 + point[j]);
			XiDerr[j][3] = -0.25 * (1.0 + point[j]);

			NiDerr[i][0] = -0.25 * (1.0 - point[i]);
			NiDerr[i][1] = -0.25 * (1.0 + point[i]);
			NiDerr[i][2] = 0.25 * (1.0 + point[i]);
			NiDerr[i][3] = 0.25 * (1.0 - point[i]);
		}
	}

	double dxdXi, dydXi, dxdNi, dydNi, area = 0.;

	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			dxdXi = XiDerr[j][0] * xi[0] + XiDerr[j][1] * xi[1] +
				XiDerr[j][2] * xi[2] + XiDerr[j][3] * xi[3];
			dydXi = XiDerr[j][0] * yi[0] + XiDerr[j][1] * yi[1] +
				XiDerr[j][2] * yi[2] + XiDerr[j][3] * yi[3];
			dxdNi = NiDerr[i][0] * xi[0] + NiDerr[i][1] * xi[1] +
				NiDerr[i][2] * xi[2] + NiDerr[i][3] * xi[3];
			dydNi = NiDerr[i][0] * yi[0] + NiDerr[i][1] * yi[1] +
				NiDerr[i][2] * yi[2] + NiDerr[i][3] * yi[3];

			area += (dxdXi * dydNi - dxdNi * dydXi) * weight[i] * weight[j];
		}
	}

	return area;
}

std::vector<double> Numeric::gaussianElimination(std::vector<std::vector<double>>* matrixPtr) {

	std::vector<std::vector<double>> matrix = *matrixPtr;

	unsigned int matrixSize = matrix.size();
	unsigned int matrixCols = matrix[0].size();
	unsigned int startPos = 0;
	std::vector<double> coefficients;
	coefficients.reserve(matrixSize);

	//Triangulate matrix
	while (coefficients.size() < 1) {

		for (unsigned int i = startPos + 1; i < matrixSize; i++) {
			double coeff = matrix[i][startPos] / matrix[startPos][startPos];
			for (unsigned int j = startPos; j < matrix[i].size(); j++) {
				matrix[i][j] -= matrix[startPos][j] * coeff;
			}
		}

		//If last row has only 1 coefficient, push value to vector thus end loop
		if (matrix[matrixSize - 1][matrixCols - 3] == 0) {
			double xn = matrix[matrixSize - 1][matrixCols - 1] / matrix[matrixSize - 1][matrixCols - 2];
			coefficients.push_back(xn);
		}
		startPos++;
	}

	//Start searching for other coefficients
	unsigned int coefficientsSize = coefficients.size();
	while (coefficientsSize < matrixSize) {
		double xn = matrix[matrixSize - coefficientsSize - 1][matrixCols - 1];
		for (unsigned int i = 0; i < coefficientsSize; i++) {
			xn -= matrix[matrixSize - coefficientsSize - 1][matrixCols - 2 - i] * coefficients[i];
		}
		xn /= matrix[matrixSize - coefficientsSize - 1][matrixCols - coefficientsSize - 2];
		coefficients.push_back(xn);
		coefficientsSize = coefficients.size();
	}

	//Reverse vector
	std::vector<double> ret;
	for (unsigned int i = 0; i < matrixSize; i++) {
		ret.push_back(coefficients[matrixSize - 1 - i]);
	}

	return ret;
}