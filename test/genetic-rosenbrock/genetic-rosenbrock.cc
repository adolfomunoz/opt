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

	for (int i = 0; i<argc-1; ++i) {
		if (strcmp("-a", argv[i])==0)      a = atof(argv[++i]);
		else if (strcmp("-b", argv[i])==0) b = atof(argv[++i]);
	}
	
	testfunction::rosenbrock f(a,b);
	
	opt::GeneticBest method(10000, 30, 1000, 30, 100);
	null_ostream os;

	std::array<float, 2> initial{{0.0f,0.0f}};

	auto start = std::chrono::system_clock::now();
	std::array<float, 2> sol = method.minimize(std::array<std::array<float,2>,1>{{initial}},
			f, 
			opt::mutation::vector_all(opt::mutation::real_normal(1.0f)),
			opt::crossover::vector_onepoint(),
			0.0f, os); 
	auto stop = std::chrono::system_clock::now();
	std::chrono::duration<float> duration = stop - start;

	//We start in 0
	std::cout<<"Time = "<<duration.count()<<" sec. | Result = "
						<<sol<<" | Value at result = "<<f(sol)<<" | Error = "<< f.error(sol)<<std::endl;
}
