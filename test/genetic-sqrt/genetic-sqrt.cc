#include "../../methods/genetic.h"
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
	
	float sol = method.minimize(0.0,[=] (float x) { return (sqrt_of - x*x)*(sqrt_of - x*x); }, 1.e-10f, std::cout); 

	//We start in 0
	std::cout<<sol<<std::endl;
}
