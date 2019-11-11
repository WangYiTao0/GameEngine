#include "GameTimer.h"

using namespace std::chrono;

GameTimer::GameTimer() noexcept
{
	last = steady_clock::now();
}

float GameTimer::Mark()noexcept
{
	const auto old = last;
	last = steady_clock::now();
	const duration<float> frameTime = last - old;
	return frameTime.count();
}

float GameTimer::Peek() const noexcept
{
	return duration<float>(steady_clock::now() - last).count();
}
