#pragma once
#include "../NoiseBase.h"
namespace Noisy {


	typedef std::vector<std::vector<float>> orth_coord;
	typedef std::vector<float> coord;
	class Simplex
		:public BaseNoise
	{
	public:
		Simplex() {}

		float noise(float x, float y) { return 0.0f; }
		float noise(std::vector<float> pos);
		float fbm(float x, float y);
	protected:
		/// <summary>
		/// Coordinate skewing
		/// </summary>
		/// <param name="origin">a vector must be a 1pos !!!!original 2D¡¢3D or higher dimensions pos</param>
		/// <returns>skewed pos size is the same as origin</returns>
		coord skew(std::vector<float>& origin);

		orth_coord simplicial_subdivision(std::vector<float>& skewed);

		float kernel_summation(std::vector<float> pos, coord skewedPos, orth_coord skewed);

		coord anti_skewed(std::vector<float>& p);

		std::vector<float> hash(std::vector<float> pos);

		float grad(coord cubepos, coord dotpos);

		/// <summary>
		/// high dimensions vector dot calculate
		/// </summary>
		/// <param name="pos_a"></param>
		/// <param name="pos_b"></param>
		/// <returns></returns>
		float dot(coord pos_a, coord pos_b);

	private:
		/// <summary>
		///  get internal coords sorted index (decreasing)
		/// </summary>
		/// <param name="p">internal coords</param>
		/// <returns></returns>
		std::vector<int> pos_index(std::vector<float>& p);

		std::vector<std::vector<float>> random_pos = { {127.1f, 311.7f},{269.5f, 183.3f} };
		float r2 = 0.6;
		//if r^2 is 0.5 is 70.0		2D
		//if r^2 is 0.6 is 24.56	2D

		float normalize = 70.f;
		coord skewed_lattice_pos;
	};
}