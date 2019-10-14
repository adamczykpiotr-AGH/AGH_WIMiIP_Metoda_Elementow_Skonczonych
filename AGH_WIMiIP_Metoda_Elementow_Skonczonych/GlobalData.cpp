#include "GlobalData.h"

std::ostream& operator<<(std::ostream& input, GlobalData& data) {
	input << "GlobalData:\n\n";
	input << "Height:\t" << data.m_height << "\n";
	input << "Length:\t" << data.m_length << "\n";
	input << "mH:\t" << data.m_mH << "\n";
	input << "mL:\t" << data.m_mL << "\n";
	input << "mW:\t" << data.m_mW << "\n";
	input << "mE:\t" << data.m_mE << "\n";
	input << "\n";

	return input;
}