#include "GameTimer.h"

using namespace std::chrono;

GameTimer::GameTimer() noexcept
{
	//startÅ@timer
	last = steady_clock::now();
}

//give time last time call mark
float GameTimer::Mark()noexcept
{
	const auto old = last;
	last = steady_clock::now();
	const duration<float> frameTime = last - old;
	return frameTime.count();
}

//give  duration between now -> mark()
float GameTimer::Peek() const noexcept
{
	return duration<float>(steady_clock::now() - last).count();
}
