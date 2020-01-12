#include <iostream>
#include <iterator>

#include "Grid.h"
#include "Numeric.h"
#include "Utils.h"
#include "Timer.h"

int main() {
	
	GlobalData global({ 0.1, 0.1 }, { 45, 45 }, { 100., 1200.}, { 500., 50 }, { 700., 25., 7800., 300. });
	Grid grid(global);

	auto t = new Timer("Solver");
	Utils::solver(global, grid);
	delete t;
}

