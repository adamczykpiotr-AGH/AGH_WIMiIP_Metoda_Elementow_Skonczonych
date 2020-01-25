#pragma once
#include <chrono>
#include <iostream>

class Timer {

public:
	Timer(const char * title) {
		m_startPoint = std::chrono::high_resolution_clock::now();
		m_title = title;
	}

	~Timer() {
		std::chrono::time_point<std::chrono::high_resolution_clock> endPoint = std::chrono::high_resolution_clock::now();
		auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_startPoint).time_since_epoch().count();
		auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endPoint).time_since_epoch().count();

		auto us = end - start;
		auto ms = us * 0.001;
		auto s = ms * 0.001;
		auto m = s / 60;

		std::cout << m_title << " took ";
		
		if (m > 1) {
			std::cout << m << " minutes\n";
			return;
		}
	
		if (s > 1) {
			std::cout << s << " seconds\n";
			return;
		}	
		
		if (ms > 1) {
			std::cout << ms << " milliseconds\n";
			return;
		}

		if (us > 1) {
			std::cout << us << " microseconds\n";
			return;
		}
	}

private:
	const char * m_title;
	std::chrono::time_point<std::chrono::high_resolution_clock> m_startPoint;
};

