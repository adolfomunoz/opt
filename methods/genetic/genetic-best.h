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
class GeneticBest
{
private:
	unsigned int   iters_;			// number of iterations
	unsigned int   best_for_mutation_;	// number of the best elements of the population to mutate
	unsigned int   nmutations_;		// number of mutations per iteration
	unsigned int   best_for_crossover_;	// number of the best elements of the population to crossover
	unsigned int   ncrossovers_;		// number of crossovers per iteration
	unsigned long  seed_;			// The seed for the random number generator (random by default)


public:
	GeneticBest(unsigned int iters 	        = 1000,
		unsigned int best_for_mutation  =   10,
		unsigned int nmutations         =   10,
		unsigned int best_for_crossover =   10,
		unsigned int ncrossovers        =   10,
		unsigned long seed = (std::random_device())()) :
		iters_(iters), 
		best_for_mutation_(best_for_mutation), nmutations_(nmutations),
		best_for_crossover_(best_for_crossover), ncrossovers_(ncrossovers),
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
	 *   	This is the function to minimize.
	 * - YType is comparable (has support for the binary < operator, returning bool) and can be checked for nans.
	 * - OS is an output stream (support for standard binary << and for binary << with XType)
	 **/
	template<typename XCollection, typename FTarget, typename FMutation, typename FCrossover, typename YType, typename OS, 
			typename XType = typename XCollection::value_type>
	requires Container<XCollection> &&
	         TargetFunction<FTarget, XType, YType> &&
	         MutationFunction<FMutation, XType, std::mt19937> &&
	         CrossoverFunction<FCrossover, XType, std::mt19937>
	XType minimize(const XCollection& ini, const FTarget& f, const FMutation& mutate, const FCrossover& cross, const YType& threshold, OS& os) const {
		std::mt19937 random(seed_);

		auto cmp = [&f] (const XType& x1, const XType& x2) { YType fx1 = f(x1); YType fx2 = f(x2); return std::isnan(fx1)?false:(std::isnan(fx2)?true:(f(x1) < f(x2))); };

		std::unordered_set<XType> population; 		
		for (const XType& x: ini) {
			population.insert(x);
		};

		std::vector<XType> vbest;    vbest.reserve(std::max(best_for_mutation_, best_for_crossover_)+1);
		XType best = *(population.begin());

		decltype(population.begin()) itpop;

		for (unsigned long iter = 0; (iter<iters_) && (f(best) > threshold);++iter) {
			//Mutation stage
			vbest.clear();
			for (itpop = population.begin(); (itpop != population.end()); ++itpop) 
			{
				vbest.push_back(*itpop);
				std::push_heap(vbest.begin(), vbest.end(), cmp);
				if (vbest.size() > best_for_mutation_) {
					std::pop_heap(vbest.begin(), vbest.end(), cmp);
					vbest.pop_back();
				}
			}
			
			os <<"["<<iter<<"] Best for mutation : ";
			for (const XType& b :  vbest) os << b << "   ";
			os<<std::endl;
			population.clear();
			
			for (unsigned int m = 0; m<nmutations_; ++m) {
				std::uniform_int_distribution<int> sample_mutation(0, vbest.size()-1);
				int chosen = sample_mutation(random);
				XType mutation = mutate(vbest[chosen], random);
//				os<<"Mutating: "<<vbest[chosen]<<" -> "<<mutation<<std::endl;
				population.insert(mutation);
			}

			for (const XType& x : vbest) population.insert(x);

			//Crossover stage
			vbest.clear();
			for (itpop = population.begin(); (itpop != population.end()); ++itpop) 
			{
				vbest.push_back(*itpop);
				std::push_heap(vbest.begin(), vbest.end(), cmp);
				if (vbest.size() > best_for_crossover_) {
					std::pop_heap(vbest.begin(), vbest.end(), cmp);
					vbest.pop_back();
				}
			}

			os <<"["<<iter<<"] Best for crossover: ";
			for (const XType& b :  vbest) os << b << "   ";
			os<<std::endl;
			population.clear();
			
			for (unsigned int c = 0; c<ncrossovers_; ++c) {
				std::uniform_int_distribution<int> sample_crossover(0, vbest.size()-1);
				int chosen1 = sample_crossover(random);
				int chosen2 = sample_crossover(random);
				if (chosen1 != chosen2) {
					population.insert(cross(vbest[chosen1],vbest[chosen2],random));
				};
			}

			for (const XType& x : vbest) population.insert(x);


			best = (*std::min_element(population.begin(), population.end(), cmp));
		}

		return best;
	}
};


} // namespace opt
