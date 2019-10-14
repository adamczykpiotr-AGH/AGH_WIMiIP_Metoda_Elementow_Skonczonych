#include "Grid.h"

GlobalData Grid::m_data;

std::ostream& operator<<(std::ostream& input, Grid& grid) {
	input << "Grid:\n\n";

	uint64_t mE = grid.m_data.getME();
	uint64_t mL = grid.m_data.getML();
	uint64_t mH = grid.m_data.getMH();

	uint16_t boxSize = 1;
	while (mE > 10) {
		boxSize++;
		mE /= 10;
	}
	
	for (uint64_t i = mH - 1; i > 0; i--) {
		for (uint64_t j = 0; j < mL; j++) {
			input << ' ' << std::setfill('0') << std::setw(boxSize) << i + mH * j << "  ";

			if (j != mL - 1) {
				for (uint16_t k = 0; k < boxSize + 4; k++) {
					input << "-";
				}
			}

			input << " ";
		}
		input << "\t - Y: " << i << "\n\n";

		for (uint64_t j = 0; j < mL-1; j++) {
			if (j == 0) {
				for (uint16_t k = 0; k < boxSize; k++) {
					input << " ";
				}
			}
			input << "|    " << std::setfill('0') << std::setw(boxSize) << (i - 1) + (mH - 1) * j << "    |";
		}
		input << "\n\n";

	}

	for (uint64_t j = 0; j < mL; j++) {
		input << ' ' << std::setfill('0') << std::setw(boxSize) << (mH)*j  << "  ";

		if (j != mL - 1) {
			for (uint16_t k = 0; k < boxSize + 4; k++) {
				input << "-";
			}
		}

		input << " ";
	}
	input << "\t - Y: " << 0 << "\n\n\n\n";

	for (uint64_t j = 0; j < mL; j++) {
		input << ' ' << std::setfill('0') << std::setw(boxSize) << "|  ";
		if (j != mL - 1) {
			for (uint16_t k = 0; k < boxSize + 6; k++) {
				input << " ";
			}
		}
	}
	input << "\n";

	for (uint64_t j = 0; j < mL; j++) {
		input << ' ' << std::setfill('0') << std::setw(boxSize) <<  "X:" << j << "  ";
		if (j != mL - 1) {
			for (uint16_t k = 0; k < boxSize + 4; k++) {
				input << " ";
			}
		}
	}
	input << "\n\n";

	return input;
}
