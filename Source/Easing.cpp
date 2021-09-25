#include "GamePCH.h"

#include "Easing.h"

float Linear::Lerp(float value)
{
	return value;
}

float Elastic::In(float value)
{
	return sinf(13.0f * (float)(M_PI / 2) * value) * powf(2, 10 * (value - 1.0f));
}

float Elastic::Out(float value)
{
	return sinf(-13.0f * (float)(M_PI / 2) * (value + 1.0f)) * powf(2, -10 * value) + 1.0f;
}


float Elastic::InOut(float value)
{
	if (value < 0.5f)
	{
		return 0.5f * sinf(13.0f * (float)(M_PI / 2) * (2.0f * value)) * powf(2, 10 * ((2.0f * value) - 1.0f));
	}
	else
	{
		return 0.5f * (sinf(-13.0f * (float)(M_PI / 2) * ((2.0f * value - 1.0f) + 1.0f)) * powf(2, -10 * (2.0f * value - 1.0f)) + 2.0f);
	}
}

float Bounce::In(float value)
{
	return 1.0f - Bounce::Out(1.0f - value);
}

float Bounce::Out(float value)
{
	const float n1 = 7.5625f;
	const float d1 = 2.75f;

	if (value < 1.0f / d1)
	{
		return n1 * value * value;
	}
	else if (value < 2.0f / d1)
	{
		return n1 * (value -= 1.5f / d1) * value + 0.75f;
	}
	else if (value < 2.5f / d1) 
	{
		return n1 * (value -= 2.25f / d1) * value + 0.9375f;
	}
	else 
	{
		return n1 * (value -= 2.625f / d1) * value + 0.984375f;
	}
}

float Bounce::InOut(float value)
{
	if (value < 0.5f)
	{
		return 0.5f * Bounce::In(value * 2.0f);
	}
	else
	{
		return 0.5f * Bounce::Out(value * 2.0f - 1.0f) + 0.5f;
	}
}

float Back::In(float value)
{
	return powf(3, value) - value * sinf(value * (float)M_PI);
}

float Back::Out(float value)
{
	float f = (1.0f - value);
	return 1.0f - (f * f * f - f * sinf(f * (float)M_PI));
}

float Back::InOut(float value)
{
	if (value < 0.5f)
	{
		float f = 2.0f * value;
		return 0.5f * (f * f * f - f * sinf(f * (float)M_PI));
	}
	else
	{
		float f = (1.0f - (2.0f * value - 1.0f));
		return 0.5f * (1.0f - (f * f * f - f * sinf(f * (float)M_PI))) + 0.5f;
	}
}