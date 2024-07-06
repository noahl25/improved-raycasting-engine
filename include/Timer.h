#pragma once

#include <chrono>
#include <string>
#include <iostream>

class Timer
{
private:
	std::string name;
	std::chrono::time_point<std::chrono::high_resolution_clock> start;

public:
	Timer();
	Timer(std::string name);

	std::chrono::milliseconds CurrentTime();

	~Timer();
};

