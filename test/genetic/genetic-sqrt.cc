#include "../../methods/genetic.h"
#include <iostream>
#include <strings.h>

int main(int argc, char** argv) {
	double sqrt_of = 4.0;
	
	for (int i = 0; i<argc-1; ++i) {
		if (strcmp("-sqrt-of", argv[i])==0) sqrt_of = atof(argv[++i]);
	}
	
	opt::Genetic method(1000, 1, 10, 20, 100);

	//We try to find the square root of a number based on its square.
	//The function to minimize is (a - x*x)^2)
	
	double sol = method.minimize(0.0,[=] (double x) { return (sqrt_of - x*x)*(sqrt_of - x*x); }, 1.e-10, std::cout); 

	//We start in 0
	std::cout<<sol<<std::endl;
}