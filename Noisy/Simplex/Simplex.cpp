#include <vector>
#include <algorithm>
#include <numeric>
#include "Simplex.h"

namespace Noisy {

	float Simplex::noise(std::vector<float> pos)
	{
		auto skewedPos = skew(pos);
		auto orthpos = simplicial_subdivision(skewedPos);

		return kernel_summation(pos, skewedPos, orthpos);

	}

	coord Simplex::skew(std::vector<float>& origin)
	{
		skewed_lattice_pos.clear();
		skewed_lattice_pos.resize(origin.size());
		const float size = origin.size() * 1.0f;
		const float F = (sqrt(size + 1.0f) - 1.0f) / size; // F = √(n+1)-1/n;
		//Coordinate skewing
		float total = 0.f;
		for (auto f : origin)total += f;
		total *= F;//(x+y+...)*F;
		std::vector<float> p1(origin.size(), 0);
		for (int i = 0; i < origin.size(); i++)
		{
			p1[i] = origin[i] + total;// x'=x+(x+y+...)*F;
			skewed_lattice_pos[i] = floor(p1[i]);
		}
		return p1;
	}

	orth_coord Simplex::simplicial_subdivision(std::vector<float>& skewed)
	{

		// about why do just like this, just see:https://en.wikipedia.org/wiki/Simplex_noise #Simplicial subdivision section
		//no matter in which orthoscheme, it must has dimension+1
		int dimension = skewed.size();
		orth_coord orthoscheme(dimension + 1, std::vector<float>(dimension, 0));
		//start(0,0,...,0)
		//orthoscheme[0] = std::vector<float>(dimension, 0);
		auto idx = pos_index(skewed);
		orthoscheme[0] = skewed_lattice_pos;
		for (int i = 1; i < dimension + 1; i++)
		{
			orthoscheme[i] = skewed_lattice_pos;
			for (int j = 0; j < i; j++)
			{
				orthoscheme[i][idx[j]] += 1.0f;
			}
		}
		return orthoscheme;
	}

	float Simplex::kernel_summation(std::vector<float> pos, coord skewedPos, orth_coord skewed)
	{
		int d = pos.size();
		//get gradian pos
		orth_coord grad_vertex;
		for (int i = 0; i < skewed.size(); i++)
		{
			grad_vertex.push_back(hash(skewed[i]));
		}

		//using skewed coord to get original coord
		orth_coord originPos(d + 1, coord(d, 0));
		for (int i = 0; i < d + 1; i++)
		{
			originPos[i]=anti_skewed(skewed[i]);
		}
		orth_coord dist(d + 1, coord(d, 0));
		std::vector<float> t(d + 1, 0.0);
		std::vector<float> n(d + 1, 0.0);

		//(max(0,r^2-d^2)^4 x [(△x,△y,△z....)·(gradx,grady,gradz)]
		for (int i = 0; i < d + 1; i++)
		{
			t[i] = r2;
			for (int j = 0; j < d; j++)
			{
				dist[i][j] = pos[j] - originPos[i][j];
				t[i] -= dist[i][j] * dist[i][j];
			}
			if (t[i] < 0.0)n[i] = 0.0f;//max(0,r^2-d^2)^4
			else
			{
				t[i] *= t[i];
				n[i] = t[i]* t[i]* dot(grad_vertex[i], dist[i]);
			}
		}
		float res = 0.0f;
		for (auto f : n)res += f;
		return 24.51f * res;
	}

	coord Simplex::anti_skewed(std::vector<float>& p)
	{
		float total = 0.0f;
		int size = p.size();
		const float G = (1 - 1 / (sqrt(size + 1.0f))) / size;//G =(1-1/√(n+1))/n
		//Kernel summation
		total = 0.0f;
		for (auto f : p)total += f;
		total *= G;//(x+y+...)*F;
		std::vector<float> p1(p.size(), 0);
		for (int i = 0; i < p.size(); i++)	p1[i] = p[i] - total;// x'=x+(x+y+...)*F;
		return p1;
	}

	std::vector<float> Simplex::hash(std::vector<float> p)
	{
		coord random = std::vector<float>(p.size(), 0);
		for (int i = 0; i < p.size(); i++)random[i] = dot(p, random_pos[i]);

		for (int i = 0; i < random.size(); i++)
		{
			random[i]= fract(sin(random[i] + 20.f) * 43758.5453123) * 2.0f - 1.0f;
		}

		return random;
	}

	float Simplex::grad(coord cubepos, coord dotpos)
	{
		return dot(cubepos, dotpos);
		//return 0.0f;
	}

	float Simplex::dot(coord pos_a, coord pos_b)
	{
		float res = 0.0f;
		for (int i = 0; i < pos_a.size(); i++)
		{
			res += pos_a[i] * pos_b[i];
		}
		return res;
	}

	float Simplex::fbm(float x, float y)
	{
		float f = 0.0f;
		x *= 4.0f;
		y *= 4.0f;

		f += 1.000000 * noise({ x, y });
		x *= 2.0f;
		y *= 2.0f;

		f += 0.500000 * noise({ x, y });
		x *= 2.0f;
		y *= 2.0f;

		f += 0.250000 * noise({ x, y });
		x *= 2.0f;
		y *= 2.0f;

		f += 0.125000 * noise({ x, y });
		x *= 2.0f;
		y *= 2.0f;

		f += 0.062500 * noise({ x, y });
		x *= 2.0f;
		y *= 2.0f;

		f += 0.312500 * noise({ x, y });
		x *= 2.0f;
		y *= 2.0f;
		return f;
	}

	std::vector<int> Simplex::pos_index(std::vector<float>& p)
	{
		std::vector<int> idx(p.size());
		iota(idx.begin(), idx.end(), 0);
		sort(idx.begin(), idx.end(), [&p](const float& a, const float& b) {return p[a] > p[b]; });
		return idx;
	}

}

