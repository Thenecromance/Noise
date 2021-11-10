
#include <iostream>
#include "Img/lodepng.h"
#include "Noisy/Perlin/Perlin.h"
#include "Noisy/Simplex/Simplex.h"
#include "Noisy/Value/ValueNoise.h"
#include <chrono>
#include <vector>

//define each image size 
int width = 500;
int height = 500;
#define USE_FBM

std::string build_by_settings(std::string& base, int width, int height, int lattice_size = 1, float seed = 1.0f)
{

#ifdef USE_FBM 
	base += "_FBM";
#endif // USE_FBM
	base += "_" + std::to_string(width) + "x" + std::to_string(height);
	if (lattice_size != 1)
	{
		base += "_" + std::to_string(lattice_size);
	}
	return base + ".png";
}

//generate a 400*400 perlin noise gray map
void PerlinNoise_test()
{


	Noisy::Perlin p;
	float lattice_size = 25;
	float seed = 19;


	p.set_lattice_size(lattice_size);
	p.set_seed(seed);
	//noise map size 400*400

	std::vector<unsigned char> image;
	std::vector<std::vector<float>> img_vec(width, std::vector<float>(height));

	image.resize(width * height * 4);
	for (auto y = 0; y < height; y++)
		for (auto x = 0; x < width; x++) {
			float col =

#ifdef USE_FBM
				p.fbm(
					((x * 1.0f) / (width * 1.0f)) * 32.0f, 
					((y * 1.0f) / (height * 1.0f)) * 32.0f)
#else
				p.noise(x, y)
#endif // USE_FBM
				* 0.5 + 0.5 //noise build with 6 fbm
				//img_vec[x][y]
				;
			col *= 255;
			image[4 * width * y + 4 * x + 0] = col;	//r
			image[4 * width * y + 4 * x + 1] = col;	//g
			image[4 * width * y + 4 * x + 2] = col;	//b
			image[4 * width * y + 4 * x + 3] = 255;	//a
		}

	std::string file = ".\\Demo\\Perlin";
	file = build_by_settings(file, width, height, lattice_size, seed);
	unsigned error = lodepng::encode(file, image, width, height);
	system(file.c_str());
}


void Simplex_Noise()
{
	Noisy::Simplex s;
	std::vector<unsigned char> image;

	float col = 0.0f;
	image.resize(width * height * 4);
	for (auto y = 0; y < height; y++)
		for (auto x = 0; x < width; x++) {
			float stx = x / 400.0f;
			float sty = y / 400.0f;

			col = 0.5 *
#ifdef USE_FBM
				s.fbm(stx * 32.0f, sty * 32.0f)
#else			
				s.noise({ stx * 32.0f,sty * 32.0f })
#endif // USE_FBM
				;
			col *= 255;
			image[4 * width * y + 4 * x + 0] = col;
			image[4 * width * y + 4 * x + 1] = col;
			image[4 * width * y + 4 * x + 2] = col;
			image[4 * width * y + 4 * x + 3] = 255;
		}


	std::string file = ".\\Demo\\Simplex";

	file = build_by_settings(file, width, height);
	//<< "fileName:" << file << "\n";
	unsigned error = lodepng::encode(file, image, width, height);
	system(file.c_str());
}

//generate a 400*400 value noise gray map
void Value_Noise_test()
{
	Noisy::Value v;
	float lattice_size = 10;


	v.set_lattice_size(lattice_size);
	std::vector<unsigned char> image;
	image.resize(width * height * 4);

	/*(p.noise(x, y)*0.4)+
	(p.noise(x*2, y*2)*0.6)+
	(p.noise(x*4, y*4)*0.8)+
	+0.5*/
	for (auto y = 0; y < height; y++)
		for (auto x = 0; x < width; x++) {
			float col =
#ifdef USE_FBM
				v.fbm(x / 400.0f * 32.0f, y / 400.0f * 32.0f)
#else
				v.noise(x, y)
#endif // USE_FBM
				* 0.5 + 0.5
				//img_vec[x][y]
				;
			col *= 255;
			image[4 * width * y + 4 * x + 0] = col;
			image[4 * width * y + 4 * x + 1] = col;
			image[4 * width * y + 4 * x + 2] = col;
			image[4 * width * y + 4 * x + 3] = 255;
		}




	std::string file = ".\\Demo\\Value";

	file = build_by_settings(file, width, height, lattice_size);

	unsigned error = lodepng::encode(file, image, width, height);
	system(file.c_str());
}

int main()
{
	using namespace std::chrono;
	steady_clock::time_point now = steady_clock::now();
	steady_clock::time_point after;
	//Simplex_Noise();
	after = steady_clock::now();
	duration<double> time_span = duration_cast<duration<double>>(after - now);
	std::cout << "Simplex Noise Cast :" << time_span.count() << std::endl;
	now = steady_clock::now();
	PerlinNoise_test();
	after = steady_clock::now();
	time_span = duration_cast<duration<double>>(after - now);
	std::cout << "Perlin Noise Cast :" << time_span.count() << std::endl;

	//now = steady_clock::now();
	//Value_Noise_test();
	//after = steady_clock::now();
	//time_span = duration_cast<duration<double>>(after - now);
	//std::cout << "Value Noise Cast :" << time_span.count() << std::endl;

	return 0;
}
