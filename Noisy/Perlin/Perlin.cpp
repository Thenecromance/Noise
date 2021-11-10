#include <vector>
#include <map>
#include <string>
#include <cmath>
#include "Perlin.h"
#define FRACT(var )abs(var) -abs( floor(var))
namespace Noisy {

	float Perlin::noise(float x, float y)
	{

		x = x / lattice_size * 1.0f;	 //transform pixel pos
		y = y / lattice_size * 1.0f;	 //transform pixel pos

		auto px = floor(x);
		auto py = floor(y);
		//first build a Lattice struct for calculating 
		vec2 vertex[4] = {
			{px,py},  //p0
			{px + 1,py},//p1
			{px,py + 1},//p2
			{px + 1,py + 1},//p3
		};
		vec2 vertex_dis[4];
		//if lattice size is 1(min pixels), so vertex distance will be 0,0 to each dot  
		if (lattice_size == 1)
		{
			vertex_dis[0] = { x,y };
			vertex_dis[1] = { x,y };
			vertex_dis[2] = { x,y };
			vertex_dis[3] = { x,y };
		}
		else
		{
			vertex_dis[0] = { x - vertex[0].x,y - vertex[0].y };
			vertex_dis[1] = { x - vertex[1].x,y - vertex[1].y };
			vertex_dis[2] = { x - vertex[2].x,y - vertex[2].y };
			vertex_dis[3] = { x - vertex[3].x,y - vertex[3].y };
		}

		//got lattice's 4 random gradians
		float vertex_random[4] = {
			grad(vertex[0].x,vertex[0].y,vertex_dis[0].x,vertex_dis[0].y),
			grad(vertex[1].x,vertex[1].y,vertex_dis[1].x,vertex_dis[1].y),
			grad(vertex[2].x,vertex[2].y,vertex_dis[2].x,vertex_dis[2].y),
			grad(vertex[3].x,vertex[3].y,vertex_dis[3].x,vertex_dis[3].y),
		};
		float wx = (x - px) / 1.0f;
		float wy = (y - py) / 1.0f;
		//using interpolation functions to make each lattice become smoooooooooooth
		return
			interpolation(wx, wy, vertex_random)
			;
	}

	float Perlin::fbm(float x, float y)
	{
		float f = 0.0f;
		x *= 4.0f;
		y *= 4.0f;

		f += 1.000000 * noise(x, y);
		x *= 2.0f;
		y *= 2.0f;

		f += 0.500000 * noise(x, y);
		x *= 2.0f;
		y *= 2.0f;

		f += 0.250000 * noise(x, y);
		x *= 2.0f;
		y *= 2.0f;

		f += 0.125000 * noise(x, y);
		x *= 2.0f;
		y *= 2.0f;

		f += 0.062500 * noise(x, y);
		x *= 2.0f;
		y *= 2.0f;

		f += 0.312500 * noise(x, y);
		return f;
	}

	std::vector<float> Perlin::hash(std::vector<float> pos)
	{
		std::vector<float> p = { dot(pos[0], pos[1], 127.1f, 311.7f),dot(pos[0], pos[1], 269.5f, 183.3f) };
		//make hash become to [0,1)
		if (build_with_seed)
		{
			p[0] = fract(sin(p[0] + seed) * 43758.5453123) * 2.0f - 1.0f;
			p[1] = fract(sin(p[1] + seed) * 43758.5453123) * 2.0f - 1.0f;
		}
		else {
			p[0] = fract(sin(p[0] + 20.0f) * 43758.5453123) * 2.0f - 1.0f;
			p[1] = fract(sin(p[1] + 20.0f) * 43758.5453123) * 2.0f - 1.0f;
		}
		return p;
	}

	float Perlin::grad(float vx, float vy, float px, float py)
	{
		std::vector<float> v = { vx,vy };
		//each dot gradiant 
		v = hash(v);
		return dot(v[0], v[1], px, py);
	}

	float Perlin::interpolation(float x, float y, float vertex_random[4])
	{
		//p0 p1 interpolation
		auto p01 = std::lerp(vertex_random[0], vertex_random[1], fade(x));
		//p2 p3 interpolation 
		auto p23 = std::lerp(vertex_random[2], vertex_random[3], fade(x));

		auto res = std::lerp(
			p01,
			p23,
			fade(y));
		return res;
	}
}

