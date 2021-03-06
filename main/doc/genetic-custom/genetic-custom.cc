#include "../../../opt.h"
#include <cmath>
#include <random>
#include <iostream>

int main(int argc, char** argv) { 
	//Calculates the cubic root of 27 (should be 3) with double precision and specific strategies
	std::cout<< opt::minimize(
		//Function to minimize
		[] (double r) { return std::abs(27.0 - r*r*r); }, 
		// Genetic method with 100 iterations, 10 population, 10 mutations and 5 crossovers per iteration
		opt::genetic(1000,10,10,5),
		//Starting population: uniformly distributed values from -100 to 100
		std::vector<double>{-100.0,-50.0, 0.0, 50.0, 100.0}, 
		// Mutation strategy: linear probability from distance 0 to 10
		[] (double r, auto& random) -> double {	
			std::array<double,3> values{r-10.0, r, r+10.0};
			std::array<double,3> probs{0.0, 1.0, 0.0};
			std::piecewise_linear_distribution<double> d(values.begin(), values.end(), probs.begin());
			return d(random);		
		},
		// Crossover strategy: random linear interpolation
		[] (double r1, double r2, auto& random) -> double {
			std::uniform_real_distribution<double> d(r1,r2);
			return d(random);
		}
	)<<std::endl;
}
