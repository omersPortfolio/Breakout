#pragma once

#include <functional>

using InterpFunction = std::function<float(float)>;

// Some Easing Functions for Tweening

struct Linear
{
	static float Lerp(float value);
};


struct Elastic
{
	static float In(float value);
	static float Out(float value);
	static float InOut(float value);
};

struct Bounce
{
	static float In(float value);
	static float Out(float value);
	static float InOut(float value);
};

struct Back
{
	static float In(float value);
	static float Out(float value);
	static float InOut(float value);
};
