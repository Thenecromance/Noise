#include <cmath>
#include <vector>
#include "ValueNoise.h"


namespace Noisy {

	float Value::noise(float x, float y)
	{
		x = x / lattice_size * 1.f;
		y = y / lattice_size * 1.f;

		float px = floor(x);
		float py = floor(y);

		//get 4 random value in each lattice
		float vertex_value[4] = {
		  grad(px,py),  //p0
		  grad(px + 1,py),//p1
		  grad(px,py + 1),//p2
		  grad(px + 1,py + 1),//p3
		};
		float wx = fade(fract(x));
		float wy = fade(fract(y));
		return interpolation(wx, wy, vertex_value);
	}

	void Value::set_lattice_size(float size)
	{
		this->lattice_size = size;
	}

	float Value::interpolation(float x, float y, float vertex_random[4])
	{
		auto res = std::lerp(
			std::lerp(vertex_random[0], vertex_random[1], x),//p0 p1 interpolation
			std::lerp(vertex_random[2], vertex_random[3], x),//p2 p3 interpolation
			y);
		return res;
	}

	float Value::fbm(float x, float y)
	{
		#define MULT(var1,var2) var1*=2.0f;var2*=2.0f;
		float f = 0.0f;
		x *= 4.0f;
		y *= 4.0f;

		f += 1.000000 * noise(x, y);
		MULT(x, y);

		f += 0.500000 * noise(x, y);
		MULT(x, y);
		MULT(x, y);

		f += 0.250000 * noise(x, y);
		MULT(x, y);

		f += 0.125000 * noise(x, y);
		MULT(x, y);

		f += 0.062500 * noise(x, y);
		MULT(x, y);

		f += 0.312500 * noise(x, y);
		MULT(x, y);
		return f;
		#undef MULT
	}

	float Value::grad(float x, float y)
	{
		x = 50.0 * fract(x * 0.3183099 + 0.71);
		y = 50.0 * fract(y * 0.3183099 + 0.113);

		return -1.0 + 2.0 * fract(x * y * (x + y));
		//return 0.0f;
	}

}