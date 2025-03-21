#include "Timer.h"


using namespace std::chrono;

Timer::Timer() {
	_last = steady_clock::now();
}

float Timer::mark() {
	const auto old = _last;
	_last = steady_clock::now();
	const duration<float> frameTime = _last - old;
	return frameTime.count();
}

float Timer::peek() const {
	return duration<float>(steady_clock::now() - _last).count();
}
