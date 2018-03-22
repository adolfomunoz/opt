#include "../../opt.h"
#include "../../utils/null-ostream.h"
#include <iostream>
#include <cstring>
#include <chrono>

int main(int argc, char** argv) {
	float sqrt_of = 4.0;
	//double is a much bigger exploration space so it is better to move to floats
	for (int i = 0; i<argc-1; ++i) {
		if (strcmp("-sqrt-of", argv[i])==0) sqrt_of = atof(argv[++i]);
	}
	
	opt::GeneticBest method(10000, 10, 100, 10, 100);

	null_ostream os;
	//We try to find the square root of a number based on its square.
	//The function to minimize is (a - x*x)^2

	auto start = std::chrono::system_clock::now();
	float sol = method.minimize(std::array<float,1>{{0.0f}},
			[=] (float x) { return (sqrt_of - x*x)*(sqrt_of - x*x); }, 
			opt::mutation::repeat(opt::mutation::bit32_swap(),5),
			opt::crossover::bit32_onepoint(),
			1.e-6f, os); 
	auto stop = std::chrono::system_clock::now();
	std::chrono::duration<float> duration = stop - start;

	//We start in 0
	std::cout<<"Time = "<<duration.count()<<" sec. | Result = "<<sol<<" | Squared result = "<<sol*sol<<" | Error = "<< fabs(std::sqrt(sqrt_of) - fabs(sol))<<std::endl;
}
