#pragma once
#include <float.h>
#include <algorithm>

#define IS_FLOAT_EQUAL(a, b) (a>=b-FLT_EPSILON&&a<=b+FLT_EPSILON)
#define IS_DOUBLE_EQUAL(a, b) (a>=b-DBL_EPSILON&&a<=b+DBL_EPSILON)

namespace Math
{ 
	static double PI = 3.1415926535897;
	static double InvPI = 0.3183098861838;
	static double RadToDeg = 57.29577951308;
	static double DegToRad = 0.01745329252;

	template<class T>
	inline T Clamp(T value, T min, T max)
	{
		if (value < min)
			value = min;
		if (value > max)
			value = max;
		return value;
	}

	template<class T>
	inline T Clamp01(T value)
	{
		return Clamp(value, 0, 1);
	}

	template<class T>
	inline T Lerp(T a, T b, T t)
	{
		return a + (b - a) * t;
	}

	template<class T>
	inline T Max(T a, T b)
	{
		return a > b ? a : b;
	}

	template<class T>
	inline T Min(T a, T b)
	{
		return a < b ? a : b;
	}
}