#pragma once
#include <chrono>

class ChiliTimer
{
public:
	ChiliTimer();
	//give time last time call mark
	float Mark();
	//give  duration between now -> mark()
	float Peek() const;
private:
	std::chrono::steady_clock::time_point last;
};