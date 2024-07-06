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
	std::cout << "Timer" << (name == "" ? " : " : " " + name + ": ") << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start) << "\n";
}



