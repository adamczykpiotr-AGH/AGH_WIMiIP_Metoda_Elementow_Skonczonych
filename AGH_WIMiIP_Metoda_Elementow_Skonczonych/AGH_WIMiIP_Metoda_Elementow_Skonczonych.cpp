#include "Grid.h"
#include "Utils.h"
#include "Timer.h"

constexpr auto siliconWaferTest = 1;
constexpr auto testCase1		= 2;
constexpr auto testCase2		= 3;

constexpr auto displayGrid		= false;
constexpr auto displayMatrices	= false;

int main() {
	std::pair<bool, bool> settings = { displayGrid, displayMatrices };

	const uint16_t mode = testCase1;
	GlobalData data;
	
	switch (mode) {
		case siliconWaferTest: {
			//https://www.americanelements.com/silicon-wafer-7440-21-3
			//https://converter.eu/thermal_conductivity/#1.49_Watt/Centimeter-%C2%B0C_in_Watt/Meter-K
			data = GlobalData({ 0.12, 0.12 }, { 20, 20 }, { 25., 480. }, { 120., 5. }, { 703.3824, 149., 2330., 300. });
			break;
		}

		case testCase1: {
			data = GlobalData({ 0.1, 0.1 }, { 4, 4 }, { 100., 1200. }, { 500., 50. }, { 700., 25., 7800., 300. });
			break;
		}	
		
		case testCase2: {
			data = GlobalData({ 0.1, 0.1 }, { 31, 31 }, { 100., 1200. }, { 100., 1. }, { 700., 25., 7800., 300. });
			break;
		}							
	}

	Grid grid(data);

	auto t = new Timer("MES Simulation");
	Utils::solver(data, grid, settings);	
	delete t;
}

