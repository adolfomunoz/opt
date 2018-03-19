#include "../../opt.h"
#include <iostream>
#include <cstring>

int main(int argc, char** argv) {
	float sqrt_of = 4.0;
	//double is a much bigger exploration space so it is better to move to floats
	for (int i = 0; i<argc-1; ++i) {
		if (strcmp("-sqrt-of", argv[i])==0) sqrt_of = atof(argv[++i]);
	}
	
	opt::Genetic method(10000, 10, 1000, 10, 1000);

	//We try to find the square root of a number based on its square.
	//The function to minimize is (a - x*x)^2)
	
	float sol = method.minimize(std::array<float,1>{{0.0f}},
			[=] (float x) { return (sqrt_of - x*x)*(sqrt_of - x*x); }, 
			[] (const float& x, auto& rng) { return opt::mutation32bit<float, std::mt19937>(x,rng); },
			[] (const float& x1, const float& x2, auto& rng) { return opt::crossover32bit<float, std::mt19937>(x1,x2,rng); },
			1.e-10f, std::cout); 

	//We start in 0
	std::cout<<sol<<std::endl;
}
