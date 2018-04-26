#include "../../../opt.h"
#include <cmath>

int main(int argc, char** argv) {
	float minx, miny;
	//Ackley function (minima in 0,0)
	std::tie(minx,miny) = opt::minimize([=] (float x, float y) 
		{ return -20.0f*std::exp(-0.2f*std::sqrt(0.5f*(x*x + y*y))) - 
		          std::exp(0.5f*(std::cos(2*M_PI*x) + std::cos(2*M_PI*y))) +
			  std::exp(1.0f) + 20.0f; });
	std::cout<<"Minima are "<<minx<<" and "<<miny<<std::endl;
}
