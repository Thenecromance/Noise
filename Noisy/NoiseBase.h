#pragma once
#ifdef NOVFUNC
#define VFUNC(type,func)type func
#else
#define VFUNC(type,func)virtual type func
#endif // NOVFUNC



namespace Noisy {

	struct vec2 {
		float x;
		float y;
	};
	//some basic Noise function is in here
	//just like fract,dot, mix etc function which is not included in standard C++ libs 
	class BaseNoise {
	public:
		VFUNC(float, fade(float t));
		VFUNC(float, fract(float x));
		VFUNC(float, mix(float x, float y, float a));
		VFUNC(float, dot(float a_x, float a_y, float b_x, float b_y));

		//each noise build type is different, so just make it as pure virtual functions 
		VFUNC(float, noise(float x, float y) = 0);

		VFUNC(float, fbm(float x, float y) = 0);
	};

}