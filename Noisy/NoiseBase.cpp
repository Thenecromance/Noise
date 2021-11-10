#include <cmath>

#include "NoiseBase.h"

float Noisy::BaseNoise::fade(float t)
{
	// f(t) = 6x^5-15x^4+10x^3
	return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
	//return 0.0f;
}

float Noisy::BaseNoise::fract(float x)
{
	x = abs(x);
	return x - floor(x);
}

float Noisy::BaseNoise::mix(float x, float y, float a)
{
	return a + x * (y - a);
}

float Noisy::BaseNoise::dot(float a_x, float a_y, float b_x, float b_y)
{
	return a_x * b_x + a_y * b_y;
}
