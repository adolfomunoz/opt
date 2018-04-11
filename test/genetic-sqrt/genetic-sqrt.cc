#include "../../opt.h"
#include "../../utils/null-ostream.h"
#include <iostream>
#include <cstring>
#include <chrono>

template<typename Method>
void test_method(const char* name, float sqrt_of, const Method& method) {
	null_ostream os;
	auto start = std::chrono::system_clock::now();
	std::mt19937 random;
	float sol = method.minimize(opt::initialization::population(100, opt::initialization::real_uniform<float>(random)),
			[=] (float x) { return (sqrt_of - x*x)*(sqrt_of - x*x); }, 
//			opt::mutation::repeat(opt::mutation::bit32_swap(),5),
			opt::mutation::bit32_swap(),
			opt::crossover::bit32_onepoint(),
			1.e-10f,
			os); 
	auto stop = std::chrono::system_clock::now();
	std::chrono::duration<float> duration = stop - start;
	std::cout<<std::setw(20)<<name<<"\t| Time = "<<std::setw(10)<<std::setprecision(3)<<duration.count()<<" sec.\t| Error = "<<std::setw(7)<<100.0f*(fabs(std::sqrt(sqrt_of) - fabs(sol))/std::sqrt(sqrt_of))<<" \%"<<std::endl;
};	

int main(int argc, char** argv) {
	float sqrt_of = 4.0;
	unsigned int iters = 1000;
    unsigned long seed = (std::random_device())();	
	unsigned int population = 50;
	unsigned int mutations  = 50;
	unsigned int crossovers = 50;
	
	for (int i = 0; i<argc-1; ++i) {
		if (strcmp("-sqrt-of", argv[i])==0)         sqrt_of = atof(argv[++i]);
		else if (strcmp("-iterations", argv[i])==0) iters = atoi(argv[++i]);
		else if (strcmp("-seed", argv[i])==0)       seed = atol(argv[++i]);
		else if (strcmp("-population", argv[i])==0) population = atoi(argv[++i]);
		else if (strcmp("-mutations", argv[i])==0)  mutations = atoi(argv[++i]);
		else if (strcmp("-crossovers", argv[i])==0) crossovers = atoi(argv[++i]);
	}

	test_method("Best", sqrt_of, opt::GeneticBest(iters, population, mutations, population, crossovers, seed));
	test_method("Stochastic", sqrt_of, opt::GeneticStochastic(iters, population, mutations, crossovers, seed));
	test_method("StochasticBest", sqrt_of, opt::GeneticStochasticBest(iters, population, mutations, crossovers, seed));
}
