#include "../../../opt.h"
#include <iostream>
#include <cstring>

std::ostream& operator<<(std::ostream& os, const std::tuple<float, int>& t)
{
    os << "("<<std::get<0>(t)<<","<<std::get<1>(t)<<")";
	return os;
}


int main(int argc, char** argv) {
	unsigned long seed = (std::random_device())();
	unsigned int mutations = 10;
	unsigned int crossovers = 10;
	unsigned int population = 10;
	std::tuple<float, int> t1(0.0f, 0), t2(1.0f,1);
	
	for (int i = 0; i<argc-1; ++i) {
		if (strcmp("-seed", argv[i])==0)       seed = atol(argv[++i]);
		else if (strcmp("-population", argv[i])==0) population = atoi(argv[++i]);
		else if (strcmp("-mutations", argv[i])==0)  mutations = atoi(argv[++i]);
		else if (strcmp("-crossovers", argv[i])==0) crossovers = atoi(argv[++i]);
	}
	
	std::mt19937 random(seed);
	
    std::cout<<"Mutating "<<t1<<" with tuple single"<<std::endl;  
	auto mutate_tuple_single = opt::mutation::tuple_single(
			opt::mutation::bit32_swap(), opt::mutation::bit32_swap());
    for (unsigned int i=0;i<mutations;++i) 
		std::cout<<mutate_tuple_single(t1, random)<<" "; 
    std::cout<<std::endl;	
	
    std::cout<<"Mutating "<<t1<<" with tuple all"<<std::endl;  
	auto mutate_tuple_all = opt::mutation::tuple_all(
			opt::mutation::bit32_swap(), opt::mutation::bit32_swap());
    for (unsigned int i=0;i<mutations;++i) 
		std::cout<<mutate_tuple_all(t1, random)<<" "; 
	std::cout<<std::endl;
	
    std::cout<<"Mutating "<<t1<<" with default"<<std::endl;  
	auto mutate_default = opt::mutation_default<decltype(t1)>::strategy;
    for (unsigned int i=0;i<mutations;++i) 
		std::cout<<mutate_default(t1, random)<<" "; 
	std::cout<<std::endl;	
	
	std::cout<<"Crossover onepoint between  "<<t1<<" and "<<t2<<std::endl;  
	auto crossover_onepoint = opt::crossover::tuple_onepoint();
    for (unsigned int i=0;i<crossovers;++i) 
		std::cout<<crossover_onepoint(t1, t2, random)<<" "; 
	std::cout<<std::endl;	

	std::cout<<"Crossover uniform between  "<<t1<<" and "<<t2<<std::endl;  
	auto crossover_uniform = opt::crossover::tuple_uniform();
    for (unsigned int i=0;i<crossovers;++i) 
		std::cout<<crossover_uniform(t1, t2, random)<<" "; 
	std::cout<<std::endl;	

	std::cout<<"Crossover default between  "<<t1<<" and "<<t2<<std::endl;  
	auto crossover_default = opt::crossover_default<decltype(t1)>::strategy;
    for (unsigned int i=0;i<crossovers;++i) 
		std::cout<<crossover_default(t1, t2, random)<<" "; 
	std::cout<<std::endl;	

	std::cout<<"Init tuples randomly"<<std::endl;
	auto init = opt::initialization::tuple(
			opt::initialization::real_uniform<float>(random),
			opt::initialization::int_uniform<int>(random));
	for (unsigned int i=0;i<population;++i)
		std::cout<<init()<<" "; 
	std::cout<<std::endl;	
	
	std::cout<<"Init tuples default"<<std::endl;
	auto initd = opt::init_default<decltype(t1)>::strategy(random);
	for (unsigned int i=0;i<population;++i)
		std::cout<<initd()<<" "; 
	std::cout<<std::endl;	
}
