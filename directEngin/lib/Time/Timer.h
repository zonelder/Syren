#pragma once
#include <chrono>
class Timer
{
public:
	Timer();
	float peek() const;
	float mark();
private:
	std::chrono::steady_clock::time_point _last;
};

