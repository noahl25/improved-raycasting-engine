#include "Timer.h"

Timer::Timer()
	: name(""), start(std::chrono::high_resolution_clock::now())
{
}

Timer::Timer(std::string name)
	: name(std::move(name)), start(std::chrono::high_resolution_clock::now())
{
}

std::chrono::milliseconds Timer::CurrentTime()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start);
}

Timer::~Timer()
{
	printf("Timer: %f\n", (float)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count());
}



