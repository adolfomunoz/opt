#pragma once

#include <iostream>
#include <iomanip>
#include <type_traits>
#include <vector>
#include "../utils/null-ostream.h"
#include <random>
#include <set>
#include <algorithm>
#include <array>

namespace opt {

/*
template<typename T>
concept bool typenameMutable = 
    requires(T t, T u, UniformRandomBitGenerator g) {
	mutation(t,g) -> T; 
	crossover(t,u,g) -> T;
    };
*/

template<typename bits32>
class BitwiseLiteral32 {
	uint32_t literal;
public:
	BitwiseLiteral32(const bits32& l = bits32()) : literal(*reinterpret_cast<const uint32_t*>(&l)) { }
	BitwiseLiteral32(const uint32_t& l) : literal(l) { }
	operator bits32() const { return *reinterpret_cast<const bits32*>(&literal); }
	BitwiseLiteral32& operator=(const bits32& l) { literal = *reinterpret_cast<const uint32_t*>(&l); return (*this); }
	BitwiseLiteral32& operator=(const uint32_t& l) { literal = l; return (*this); }


	template<typename RNG>
	BitwiseLiteral32<bits32> mutation(RNG& random) {
		std::uniform_int_distribution<int> sample(0,31);
		return BitwiseLiteral32(literal ^ (1 << sample(random)));
	}

	//A bit of a hack thanks to the cast to bits32
	template<typename RNG>
	BitwiseLiteral32<bits32> crossover(const bits32& that, RNG& random) {
		uint32_t literal2(that);
		std::uniform_int_distribution<int> sample(1,30);
		int splitat = sample(random);

		return BitwiseLiteral32(
			(literal & ((~0) << splitat)) &
			(literal2 & ((~0) >> (32 - splitat)))
		);
	}	
};

template<typename bits64>
class BitwiseLiteral64 {
	uint64_t literal;
public:
	BitwiseLiteral64(const bits64& l = bits64()) : literal(*reinterpret_cast<const uint64_t*>(&l)) { }
	BitwiseLiteral64(const uint64_t& l) : literal(l) { }
	operator bits64() const { return *reinterpret_cast<const bits64*>(&literal); }
	BitwiseLiteral64& operator=(const bits64& l) { literal = *reinterpret_cast<const uint64_t*>(&l); return (*this); }
	BitwiseLiteral64& operator=(const uint64_t& l) { literal = l; return (*this); }

	template<typename RNG>
	BitwiseLiteral64<bits64> mutation(RNG& random) const {
		std::uniform_int_distribution<int> sample(0,63);
		int bit = sample(random);
//		std::cerr<<std::dec<<bit<<std::endl<<
//			"    "<<std::hex<<std::setw(16)<<literal<<std::endl<<
//			"    "<<std::hex<<std::setw(16)<<(uint64_t(1)<<bit)<<std::endl<<
//			"    "<<std::hex<<std::setw(16)<<(literal ^ (uint64_t(1)<<bit))<<std::endl;
		return BitwiseLiteral64<bits64>(literal ^ (uint64_t(1)<<bit));
	}

	//A bit of a hack thanks to the cast to bits32
	template<typename RNG>
	BitwiseLiteral64<bits64> crossover(const bits64& that, RNG& random) const {
		uint64_t literal2(that);
		std::uniform_int_distribution<int> sample(1,62);
		int splitat = sample(random);

		return BitwiseLiteral64<bits64>(
			(literal & (uint64_t(~0) << splitat)) &
			(literal2 & (uint64_t(~0) >> (64 - splitat)))
		);
	}	
};



/**
 * Genetic algorithm
 **/
class Genetic
{
private:
	unsigned int   iters_;			// number of iterations
	unsigned int   best_for_mutation_;	// number of the best elements of the population to mutate
	unsigned int   nmutations_;		// number of mutations per iteration
	unsigned int   best_for_crossover_;	// number of the best elements of the population to crossover
	unsigned int   ncrossovers_;		// number of crossovers per iteration
	unsigned long  seed_;			// The seed for the random number generator (random by default)


public:
	Genetic(unsigned int iters 	        = 1000,
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
	 * - XType has support for mutations and crossovers, by the following methods:
	 *   	XType mutation(RNG& random)
	 *   	XType crossover(const XType& that, RNG& random)
	 * - RNG is a C++11-like random number engine
	 * - F is a function that, given a XType, returns an YValue: YValue F(XType). 
	 *   	This is the function to minimize.
	 * - YValue is comparable (has support for the binary < operator, returning bool).
	 * - OS is an output stream (support for standard binary << and for binary << with XType)
	 **/
	template<typename XCollection, typename F, typename YValue, typename OS, 
			typename XType = typename std::remove_const<typename std::remove_reference<decltype(*(std::begin(std::declval<XCollection>())))>::type>::type>
	XType minimize(const XCollection& ini, const F& f, const YValue& threshold, OS& os) const {
		std::mt19937 random(seed_);
		std::uniform_int_distribution<int> sample_mutation(0, best_for_mutation_ - 1);
		std::uniform_int_distribution<int> sample_crossover(0, best_for_crossover_ - 1);

		auto cmp = [&f] (const XType& x1, const XType& x2) { return f(x1) < f(x2); };

		std::set<XType> population;		
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
				int chosen = sample_mutation(random);
				XType mutation = vbest[chosen].mutation(random);
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
				int chosen1 = sample_crossover(random);
				int chosen2 = sample_crossover(random);
				if (chosen1 != chosen2) {
					population.insert(vbest[chosen1].crossover(vbest[chosen2],random));
				};
			}

			for (const XType& x : vbest) population.insert(x);


			best = (*std::min_element(population.begin(), population.end(), cmp));
		}

		return best;
		
	}

	/**
	 * Minimizes the function f, giving an initial value ini, and logs the process in the output stream os. The
	 * threshold defines a value below which we consider the outcome good enough
	 *
	 * - F is a function that, given a XType, returns an YValue: YValue F(XType). 
	 *   	This is the function to minimize.
	 * - YValue is comparable (has support for the binary < operator, returning bool).
	 * - OS is an output stream (support for standard binary << and for binary << with XType)
	 **/
	template<typename F, typename YValue, typename OS>
	float minimize(const float& ini, const F& f, const YValue& threshold, OS& os) const {
		std::array<BitwiseLiteral32<float>, 1> ini_population{{BitwiseLiteral32<float>(ini)}};	
		return minimize(ini_population, f, threshold, os);
	}

	/**
	 * Minimizes the function f, giving an initial value ini, and logs the process in the output stream os. The
	 * threshold defines a value below which we consider the outcome good enough
	 *
	 * - F is a function that, given a XType, returns an YValue: YValue F(XType). 
	 *   	This is the function to minimize.
	 * - YValue is comparable (has support for the binary < operator, returning bool).
	 * - OS is an output stream (support for standard binary << and for binary << with XType)
	 **/
	template<typename F, typename YValue, typename OS>
	double minimize(const double& ini, const F& f, const YValue& threshold, OS& os) const {
		std::array<BitwiseLiteral64<double>, 1> ini_population{{BitwiseLiteral64<double>(ini)}};	
		return minimize(ini_population, f, threshold, os);
	}

	/**
	 * Minimizes the function f, giving an initial value ini, and logs the process in the output stream os. The
	 * threshold defines a value below which we consider the outcome good enough
	 *
	 * - F is a function that, given a XType, returns an YValue: YValue F(XType). 
	 *   	This is the function to minimize.
	 * - YValue is comparable (has support for the binary < operator, returning bool).
	 * - OS is an output stream (support for standard binary << and for binary << with XType)
	 **/
	template<typename F, typename YValue, typename OS>
	int minimize(const int& ini, const F& f, const YValue& threshold, OS& os) const {
		std::array<BitwiseLiteral32<int>, 1> ini_population{{BitwiseLiteral32<int>(ini)}};	
		return minimize(ini_population, f, threshold, os);
	}

	/**
	 * Minimizes the function f, giving an initial value ini, and logs the process in the output stream os. The
	 * threshold defines a value below which we consider the outcome good enough
	 *
	 * - F is a function that, given a XType, returns an YValue: YValue F(XType). 
	 *   	This is the function to minimize.
	 * - YValue is comparable (has support for the binary < operator, returning bool).
	 * - OS is an output stream (support for standard binary << and for binary << with XType)
	 **/
	template<typename F, typename YValue, typename OS>
	long minimize(const long& ini, const F& f, const YValue& threshold, OS& os) const {
		std::array<BitwiseLiteral64<long>, 1> ini_population{{BitwiseLiteral64<long>(ini)}};	
		return minimize(ini_population, f, threshold, os);
	}

	/**
	 * Minimizes the function f, giving an initial population ini, and logs the process in the output stream os. The
	 * threshold defines a value below which we consider the outcome good enough

	 *
	 * - XCollection is either an iterable collection of XType, which is the initial population, or a signle XType
	 *   for basic types (float, double, int, long). It must have at least 
	 *     one element
	 * - XType has support for mutations and crossovers, by the following methods:
	 *   	XType mutation(RNG& random)
	 *   	XType crossover(const XType& that, RNG& random)
	 * - RNG is a C++11-like random number engine
	 * - F is a function that, given a XType, returns an YValue: YValue F(XType). 
	 *   	This is the function to minimize.
	 * - YValue is comparable (has support for the binary < operator, returning bool).
	 **/
	template<typename XCollection, typename F, typename YValue>
	auto minimize(const XCollection& ini, const F& f, const YValue& threshold) const {
		null_ostream os;
		return minimize(ini, f, threshold, os);
	}





};


} // namespace opt
