#include "Timer.h"
using namespace std::chrono;

Timer::Timer()
{
	last = steady_clock::now();
}

float Timer::Mark()
{
	const auto old = last;
	last = steady_clock::now();
	const duration<float> frametime = last - old;
	return frametime.count();
}

float Timer::Peek() const
{

	return duration<float>(steady_clock::now()-last).count();
}
