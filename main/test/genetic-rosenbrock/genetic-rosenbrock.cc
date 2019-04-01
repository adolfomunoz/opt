#include "../../../opt.h"
#include "../../../utils/null-ostream.h"
#include "../../../utils/test-functions.h"
#include <iostream>
#include <cstring>
#include <chrono>

std::ostream& operator<<(std::ostream& os, const std::array<float,2>& a) {
	os<<" <"<<a[0]<<" , "<<a[1]<<"> ";
	return os;
}

template<typename Method>
requires opt::GeneticMethod<Method>
void test_method(const char* name, const testfunction::rosenbrock& f, const Method& method) {
	auto logger = opt::genetic_logger::stream(std::cout);
	auto start = std::chrono::system_clock::now();
	std::mt19937 random;
	std::array<float,2> sol = method.minimize(
			opt::initialization::population(100, 
					opt::initialization::array<2>(opt::initialization::real_uniform(random, -100.0f, 100.0f))),
			f, 
			opt::mutation::vector_single(opt::mutation::real_normal(1.0f)),
			opt::crossover::vector_onepoint(),
			1.e-4f,
			logger); 
	auto stop = std::chrono::system_clock::now();
	std::chrono::duration<float> duration = stop - start;
	std::cout<<std::setw(20)<<name<<"\t| Time = "<<std::setw(10)<<std::setprecision(3)<<duration.count()<<" sec.\t| Result = "
						<<sol<<"\t| Value at result = "<<f(sol)<<"\t| Error = "<< f.error(sol)<<std::endl;
}

int main(int argc, char** argv) {
	float a = 1.0; 
	float b = 100.0;
	unsigned int iters = 1000;
    	unsigned long seed = (std::random_device())();
	unsigned int population = 50;
	unsigned int mutations  = 50;
	unsigned int crossovers = 50;

	for (int i = 0; i<argc-1; ++i) {
		if (strcmp("-a", argv[i])==0)      a = atof(argv[++i]);
		else if (strcmp("-b", argv[i])==0) b = atof(argv[++i]);
		else if (strcmp("-iterations", argv[i])==0) iters = atoi(argv[++i]);
		else if (strcmp("-seed", argv[i])==0)       seed = atol(argv[++i]);
		else if (strcmp("-population", argv[i])==0) population = atoi(argv[++i]);
		else if (strcmp("-mutations", argv[i])==0)  mutations = atoi(argv[++i]);
		else if (strcmp("-crossovers", argv[i])==0) crossovers = atoi(argv[++i]);
		
	}
	
	testfunction::rosenbrock f(a,b);
	
	test_method("StochasticBest", f, opt::GeneticStochasticBest(iters, population, mutations, crossovers, seed));
}
