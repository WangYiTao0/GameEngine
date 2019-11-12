#pragma once
#include<cmath>

constexpr float PI = 3.14159265f;
constexpr double PI_D = 3.1415926535897932;

template<typename T>
constexpr auto sq(const T& x)
{
	return x * x;
}

//we don't need large rad  
// limit rad in -1 ~ +1
template<typename T>
//set angle between -180 ande 180
T wrap_angle(T theta)
{
	const T modded = fmod(theta, (T)2.0 * (T)PI_D);
	return (modded > (T)PI_D) ?
		(modded - (T)2.0 * (T)PI_D) :
		modded;
}
//
template<typename T>
constexpr T interpolate(const T& src, const T& dst, float alpha)
{
	return src + (dst - src) * alpha;
}

//degree to rad
template<typename T>
constexpr T to_rad(T deg)
{
	return deg * PI / (T)180.0;
}
