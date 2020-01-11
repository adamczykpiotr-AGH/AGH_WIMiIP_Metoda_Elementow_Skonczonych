#pragma once
#include <string>	//std::string
#include <ostream>	//std::ostream
#include <array>

class GlobalData {

	class Size {
	public:
		double m_height;
		double m_length;
	};

	class Grid {
	public:
		uint64_t m_mH;
		uint64_t m_mL;
		uint64_t m_mW;
		uint64_t m_mE;
	};

	class Temperature {
	public:
		double m_initial;
		double m_ambient;
	};

	class Time {
	public:
		double m_dration;
		double m_step;
	};

	class Material {
	public:
		double m_heatCapacity;
		double m_conductivityCoeff;
		double m_density;
		double m_heatExcangeCoeff;
	};

	Size m_size;
	Grid m_grid;
	Temperature m_temp;
	Time m_time;
	Material m_material;

public:

	GlobalData(Size size, Grid grid, Temperature temp, Time time, Material material) {
		m_size = size;
		m_grid = grid;
		m_temp = temp;
		m_time = time;
		m_material = material;

		m_grid.m_mW = m_grid.m_mL * m_grid.m_mH;
		m_grid.m_mE = (m_grid.m_mL - 1) * (m_grid.m_mH - 1);
	}

	GlobalData() {
		m_size = {0., 0.};
		m_grid = { 0, 0, 0, 0 };
		m_temp = { 0., 0. };
		m_time = { 0., 0. };
		m_material = { 0., 0., 0., 0. };
	}

	~GlobalData() {
	}

	double getHeight() {
		return m_size.m_height;
	}

	double getLength() {
		return m_size.m_length;
	}

	uint64_t getMH() {
		return m_grid.m_mH;
	}

	uint64_t getML() {
		return m_grid.m_mL;
	}

	uint64_t getMW() {
		return m_grid.m_mW;
	}

	uint64_t getME() {
		return m_grid.m_mE;
	} 

	double getInitialTemp() {
		return m_temp.m_initial;
	}

	double getAmbientTemp() {
		return m_temp.m_ambient;
	}

	double getTimeDuration() {
		return m_time.m_dration;
	}

	double getTimeStep() {
		return m_time.m_step;
	}

	double getMaterialHeatCapacity() {
		return m_material.m_heatCapacity;
	}

	double getMaterialConductivityCoefficient() {
		return m_material.m_conductivityCoeff;
	}	
	
	double getMaterialDensity() {
		return m_material.m_density;
	}

	double getMaterialHeatExchangeCoefficient() {
		return m_material.m_heatExcangeCoeff;
	}
};

