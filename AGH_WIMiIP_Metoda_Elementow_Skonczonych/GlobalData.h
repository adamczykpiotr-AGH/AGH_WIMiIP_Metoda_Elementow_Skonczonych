#pragma once
#include <string>	//std::string
#include <ostream>	//std::ostream

class GlobalData {
	double m_height;
	double m_length;
	uint64_t m_mH;
	uint64_t m_mL;
	uint64_t m_mW;
	uint64_t m_mE;
		
	friend std::ostream& operator<< (std::ostream&, GlobalData&);

public:
	GlobalData(double height, double length, uint64_t mH, uint64_t mL) {
		m_height = height;
		m_length = length;
		m_mH = mH;
		m_mL = mL;
		m_mW = mL * m_mH;
		m_mE = (mL - 1) * (m_mH - 1);
	}

	GlobalData() {
		m_height = m_length = 0.;
		m_mH = m_mL = m_mW = m_mE = 0;
	}

	~GlobalData() {
	}

	double getHeight() {
		return m_height;
	}

	double getLength() {
		return m_length;
	}

	uint64_t getMH() {
		return m_mH;
	}

	uint64_t getML() {
		return m_mL;
	}

	uint64_t getMW() {
		return m_mW;
	}
	uint64_t getME() {
		return m_mE;
	}
};

