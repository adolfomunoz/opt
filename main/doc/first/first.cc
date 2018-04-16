#include "../../../opt.h"
#include <cstring>

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

	std::cout<<opt::minimize([=] (float x) { return (sqrt_of - x*x)*(sqrt_of - x*x); }, opt::genetic(iters, population, mutations, crossovers, seed))<<std::endl;
}
