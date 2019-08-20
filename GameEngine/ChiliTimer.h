#pragma once
#include <chrono>

class ChiliTimer
{
public:
	ChiliTimer()noexcept;
	//give time last time call mark
	float Mark() noexcept;
	//give  duration between now -> mark()
	float Peek() const noexcept;
private:
	std::chrono::steady_clock::time_point last;
};