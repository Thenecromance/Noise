#pragma once

#include "../NoiseBase.h"
namespace Noisy {
	//Lattice based Noisy generate temp finished
	//https://en.wikipedia.org/wiki/Perlin_noise
	/// <summary>
	/// this part of Perlin noise is basically for 2D 
	/// </summary>
	class Perlin :
		public BaseNoise 
	{
	public:
		float noise(float x, float y);

	
		//decide which lattice size 
		void set_lattice_size(float size) {
			lattice_size = size;
		}

		void set_seed(float seed)
		{
			build_with_seed = true;
			this->seed = seed;
		}

		virtual float fbm(float x, float y);
	protected:
		//this hash is random generated!! need to change 
		std::vector<float> hash(std::vector<float> pos);
		/*float hash(float x);*/
		virtual float grad(float vx, float vy, float px, float py);

		virtual float interpolation(float x, float y, float vertex_random[4]);

	private:
		float lattice_size = 1.0f;
		float seed = 1.0f;
		bool build_with_seed = false;
	};

}



