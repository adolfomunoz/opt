#pragma once

#include "concepts.h"
#include <iostream>
#include <type_traits>
#include <vector>
#include <random>
#include <algorithm>
#include <cmath>
#include <tuple>

namespace opt {


/**
 * Genetic algorithm
 **/
class GeneticStochastic
{
private:
	unsigned int   iters_;		// number of iterations
	unsigned int   npopulation_;     // population size (for selection)
	unsigned int   nmutations_;	// number of mutations per iteration
	unsigned int   ncrossovers_;    // number of crossovers per iteration
	unsigned long  seed_;		// The seed for the random number generator (random by default)

	template<typename XType, typename YType>
	requires std::is_floating_point_v<YType>
	void selection(const typename std::vector<std::tuple<XType, YType>>::const_iterator& source_begin, 
		       const typename std::vector<std::tuple<XType, YType>>::const_iterator& source_end, 
		       const typename std::vector<std::tuple<XType, YType>>::iterator& target_begin, std::mt19937& random) const
	{
		//Warning, the iterator "target_begin" should have "npopulation_" elements left.
		std::vector<YType> probability(source_end - source_begin);
		YType furthest = std::accumulate(source_begin, source_end, YType(0), 
			[] (YType a, const std::tuple<XType,YType>& b) { 
				return (std::isfinite(std::get<1>(b)) && std::get<1>(b)>a)?std::get<1>(b):a;});
		std::transform(source_begin, source_end, probability.begin(), 
			[furthest] (const std::tuple<XType,YType>& e) { 
				return std::isfinite(std::get<1>(e))?
					(((furthest+YType(1)-std::get<1>(e)))/(furthest + YType(1))):YType(0); });
/*		std::cerr<<"[prob] -> ";
		std::for_each(probability.begin(), probability.end(), [] (const YType& p) { std::cerr<<p<<" | "; });
		std::cerr<<std::endl;*/

		std::discrete_distribution<int> index(probability.begin(), probability.end());
		for (unsigned int i = 0; i < npopulation_; ++i) *(target_begin + i) = *(source_begin + index(random));
		//Some element may appear twice or even more...
	}

	template<typename XType, typename YType, typename FTarget, typename FMutation>
	requires std::is_floating_point_v<YType> &&
		 TargetFunction<FTarget, XType, YType> &&
	         MutationFunction<FMutation, XType, std::mt19937> 
	void mutation(const typename std::vector<std::tuple<XType, YType>>::const_iterator& source_begin, 
		      const typename std::vector<std::tuple<XType, YType>>::const_iterator& source_end, 
		      const typename std::vector<std::tuple<XType, YType>>::iterator& target_begin, 
		      const FTarget& f, const FMutation& mutate, std::mt19937& random) const 
	{
		std::uniform_int_distribution<int> index(0, source_end - source_begin - 1);	
		//Warning, the iterator "target_begin" should have "nmutations_" elements left.
		for (unsigned int i = 0; i < nmutations_; ++i) {
			XType mutated = mutate(std::get<0>(*(source_begin + index(random))), random);
			*(target_begin + i) = std::tuple(mutated, f(mutated));
		}
	}

	template<typename XType, typename YType, typename FTarget, typename FCrossover>
	requires std::is_floating_point_v<YType> &&
		 TargetFunction<FTarget, XType, YType> &&
	         CrossoverFunction<FCrossover, XType, std::mt19937> 
	void crossover(const typename std::vector<std::tuple<XType, YType>>::const_iterator& source_begin, 
		      const typename std::vector<std::tuple<XType, YType>>::const_iterator& source_end, 
		      const typename std::vector<std::tuple<XType, YType>>::iterator& target_begin, 
		      const FTarget& f, const FCrossover& cross, std::mt19937& random) const
	{
		std::uniform_int_distribution<int> index(0, source_end - source_begin - 1);	
		//Warning, the iterator "target_begin" should have "nmutations_" elements left.
		for (unsigned int i = 0; i < ncrossovers_; ++i) {
			XType crossed = cross(
				std::get<0>(*(source_begin + index(random))),
				std::get<0>(*(source_begin + index(random))),
					random);
			*(target_begin + i) = std::tuple(crossed, f(crossed));
		}
	}

	template<typename XType, typename YType, typename OS> 
	void stream(unsigned long iter, const typename std::vector<std::tuple<XType, YType>>::const_iterator& source_begin, 
		   const typename std::vector<std::tuple<XType, YType>>::const_iterator& source_end, OS& os) const {

		os<<"["<<std::setw(4)<<iter<<"] -> ";
		std::for_each(source_begin, source_end, [&os] (const std::tuple<XType, YType>& t) {
				os<<std::get<0>(t)<<" ("<<std::get<1>(t)<<") | "; });
		os<<std::endl;
	}

public:
	GeneticStochastic(unsigned int iters    = 1000,
		unsigned int npopulation        =   10,
		unsigned int nmutations         =   10,
		unsigned int ncrossovers        =   10,
		unsigned long seed = (std::random_device())()) :
		iters_(iters), 
		npopulation_(npopulation),
		nmutations_(nmutations),
		ncrossovers_(ncrossovers),
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
	template<typename XCollection, typename FTarget, typename FMutation, typename FCrossover, typename YType, typename OS, 
			typename XType = typename XCollection::value_type>
	requires std::is_floating_point_v<YType> &&
	         Container<XCollection> &&
	         TargetFunction<FTarget, XType, YType> &&
	         MutationFunction<FMutation, XType, std::mt19937> &&
	         CrossoverFunction<FCrossover, XType, std::mt19937>
	XType minimize(const XCollection& ini, const FTarget& f, const FMutation& mutate, const FCrossover& cross, const YType& threshold, OS& os) const {
		std::mt19937 random(seed_);
		
		std::vector<std::tuple<XType,YType>> initial_population; //tuple contains both and xvalue and its fitness
		for (const XType& x: ini) initial_population.push_back(std::tuple(x,f(x)));

		std::vector<std::tuple<XType,YType>> population(npopulation_ + nmutations_ + ncrossovers_);
		std::vector<std::tuple<XType,YType>> population_next(npopulation_ + nmutations_ + ncrossovers_);		

		selection<XType,YType>(initial_population.begin(), initial_population.end(), population.begin(), random);

		stream<XType,YType>(0, population.begin(), population.begin() + npopulation_, os);
		
		//The threshold is almost ignored but it is fast because we only find the best in the
		//end
		for (unsigned long iter = 1; (iter<=iters_) && (std::get<1>(population[0])>threshold);++iter) {
			crossover<XType,YType>(population.begin(), population.begin() + npopulation_, 
					population.begin() + npopulation_, f, cross, random);
			mutation<XType,YType>(population.begin(), population.begin() + npopulation_, 
					population.begin() + npopulation_ + ncrossovers_, f, mutate, random);
			selection<XType,YType>(population.begin(), population.end(), population_next.begin(), random); //Unneded in the last iteration
			std::swap(population, population_next);
			stream<XType,YType>(iter, population.begin(), population.begin() + npopulation_, os);
		}

		return std::get<0>(std::accumulate(population_next.begin() + 1, population_next.end(), population_next.front(), 
			[] (const std::tuple<XType, YType>& a, const std::tuple<XType, YType>& b) {
				return (std::get<1>(a) < std::get<1>(b))?a:b;
			})); //Obtain the minimum in the big population.
	}
};


} // namespace opt
