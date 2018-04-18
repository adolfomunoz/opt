#include "../../../opt.h"
#include "../../../methods/genetic/tuple.h"
#include <iostream>
#include <cstring>
#include <chrono>

// helper function to print a tuple of any size
template<class Tuple, std::size_t N>
struct TuplePrinter {
    static void print(const Tuple& t) 
    {
        TuplePrinter<Tuple, N-1>::print(t);
        std::cout << ", " << std::get<N-1>(t);
    }
};

template<class Tuple>
struct TuplePrinter<Tuple, 1> {
    static void print(const Tuple& t) 
    {
        std::cout << std::get<0>(t);
    }
};

template<class... Args>
void print(const std::tuple<Args...>& t) 
{
    std::cout << "(";
    TuplePrinter<decltype(t), sizeof...(Args)>::print(t);
    std::cout << ")\n";
}
// end helper function

int main(int argc, char** argv) {
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
