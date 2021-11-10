#pragma once
#include "../NoiseBase.h"

namespace Noisy {

	//2D lattice based ValueNoise
	class Value :public BaseNoise {
	public:
		VFUNC(float, noise(float x, float y));
		VFUNC(void, set_lattice_size(float size));

		virtual float grad(float x, float y);
		virtual float interpolation(float x, float y, float vertex_random[4]);
		//Fractal Brown Motion
		virtual float fbm(float x, float y);
	private:
		float lattice_size = 1.0f;
	};


}