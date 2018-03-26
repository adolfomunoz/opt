#pragma once

#include "concepts.h"
#include <iostream>
#include <iomanip>
#include <type_traits>
#include <vector>
#include <random>
#include <set>
#include <unordered_set>
#include <algorithm>
#include <array>

namespace opt {


/**
 * Genetic algorithm
 **/
class GeneticStochastic
{
private:
	unsigned int   iters_;			// number of iterations
	unsigned int   nmutations_;		// number of mutations per iteration
	unsigned int   best_for_crossover_;	// number of the best elements of the population to crossover
	unsigned long  seed_;			// The seed for the random number generator (random by default)


public:
	GeneticStochastic(unsigned int iters 	        = 1000,
		unsigned int nmutations         =   10,
		unsigned int ncrossovers        =   10,
		unsigned long seed = (std::random_device())()) :
		iters_(iters), 
		nmutations_(nmutations),
		crossovers_(ncrossovers),
		seed_(seed) 
	{}
			
	/**
	 * Minimizes the function f, giving an initial population ini, and logs the process in the output stream os. The
	 * threshold defines a value below which we consider the outcome good enough
	 *
	 * - XCollection is an iterable collection of XType, which is the initial population. It must have at least 
	 *     one element
	 * - XType has support for mutations and crossovers, by the functions FMutation and FCrossover 
	 * - RNG is a C++11-like random number engine
	 * - FTarget is a function that, given a XType, returns an YValue: YValue F(XType). 
	 *   	This is the function to minimize. The ULTIMATE TARGET should be 0
	 * - YType is comparable (has support for the binary < operator, returning bool) and can be checked for nans
	 * - YType can also be accumulated (added), used for random values and initialized from zero
	 * - OS is an output stream (support for standard binary << and for binary << with XType)
	 **/
	template<typename XCollection, typename FTarget, typename FMutation, typename FCrossover, typename OS, 
			typename XType = typename XCollection::value_type,
			typename YType = decltype(declval<FTarget>()(declval<XType>()))>
	requires Container<XCollection> &&
	         TargetFunction<FTarget, XType, YType> &&
	         MutationFunction<FMutation, XType, std::mt19937> &&
	         CrossoverFunction<FCrossover, XType, std::mt19937>
	XType minimize(const XCollection& ini, const FTarget& f, const FMutation& mutate, const FCrossover& cross, OS& os) const {
		std::mt19937 random(seed_);

		std::vector<XType> population; 		
		for (const XType& x: ini) {
			population.push_back(x);
		};
		std::vector<YType> energy(population.size());
		
		std::vector<XType> population_mutated(population.size()+nmutations_);
		std::vector<YType> energy_mutated(population_mutated.size());
		
		for (unsigned long iter = 0; iter<iters_;++iter) {
			energy.resize(population.size());
			std::transform(population.begin(),population.end(),energy.begin(),f) 
			std::discrete_distribution<int> index_mutation(energy.begin(), energy.end());

			population_mutated.resize(population.size()+nmutations_);
			std::copy(population.begin(), population.end(), population_mutated.begin());
			for (int i = 0; i<nmutations_; ++i) {
				population_mutated[population.size()+i] =
					mutate(population[index_mutation(random)], random);
			}
			
			energy_mutated.resize(population_mutated.size());
			std::transform(population_mutated.begin(),population_mutated.end(),
						energy_mutated.begin(),f) 
			std::discrete_distribution<int> index_crossover(energy_mutated.begin(), energy_mutated.end());
			
			
			//We ensure that the best one goes on (index 0)
			population.resize(1+ncrossovers_); population[0] = population_mutated[0];
			YType best = energy_mutated[0];
			for (size_t i = 1; i<population_mutated.size(); ++i) {
				if (energy_mutated[i] > best) {
					population[0] = population_mutated[i];
					best = energy_mutated[i];
				}
			}
			
			for (int i = 0; i<ncrossovers_; ++i) {
				population[1+i] =
					cross(population_mutated[index_crossover(random)], 
					      population_mutated[index_crossover(random)], 
					      random);
			}
		}

		return population[0]; //Not true, should search for it.
	}
};


} // namespace opt
