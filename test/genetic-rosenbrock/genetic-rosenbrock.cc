#include "../../opt.h"
#include "../../utils/null-ostream.h"
#include "../../utils/test-functions.h"
#include <iostream>
#include <cstring>
#include <chrono>

std::ostream& operator<<(std::ostream& os, const std::array<float,2>& a) {
	os<<"( "<<a[0]<<" , "<<a[1]<<" )";
	return os;
}

namespace std {
    template <size_t N> struct hash<std::array<float,N>>
    {
        size_t operator()(const std::array<float,N>& a) const
        {
			return std::hash<float>()(a[0]+a[1]);
        }
    };
}

int main(int argc, char** argv) {
	float a = 1.0; float b = 100.0;
	unsigned int iters = 1000;
        unsigned long seed = (std::random_device())();	

	for (int i = 0; i<argc-1; ++i) {
		if (strcmp("-a", argv[i])==0)      a = atof(argv[++i]);
		else if (strcmp("-b", argv[i])==0) b = atof(argv[++i]);
		else if (strcmp("-iterations", argv[i])==0) iters = atoi(argv[++i]);
		else if (strcmp("-seed", argv[i])==0)       seed = atol(argv[++i]);
	}
	
	testfunction::rosenbrock f(a,b);
	
//	opt::GeneticBest method(iters, 30, 1000, 30, 100);
//	opt::GeneticStochastic method(iters, 10, 20, 20, seed);
	opt::GeneticStochasticBest method(iters, 5, 20, 20, seed);


	null_ostream os;

	std::array<float, 2> initial{{0.0f,0.0f}};

	auto start = std::chrono::system_clock::now();
	std::array<float, 2> sol = method.minimize(std::array<std::array<float,2>,1>{{initial}},
			f, 
			opt::mutation::vector_all(opt::mutation::real_normal(1.0f)),
			opt::crossover::vector_onepoint(),
			std::cout); 
	auto stop = std::chrono::system_clock::now();
	std::chrono::duration<float> duration = stop - start;

	//We start in 0
	std::cout<<"Time = "<<duration.count()<<" sec. | Result = "
						<<sol<<" | Value at result = "<<f(sol)<<" | Error = "<< f.error(sol)<<std::endl;
}
