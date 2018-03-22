#pragma once

#include <random>
#include "concepts.h"
#include "../../utils/concepts.h"

namespace opt {
namespace mutation {
template<typename FMutation>
class vector_single {
	FMutation mutate_element;
public:
	vector_single(const FMutation& mutate_element) :
		mutate_element(mutate_element) { }
		
	template<typename C, typename RNG>
	requires UniformRandomBitGenerator<RNG> &&
			 RandomAccessContainer<C> &&
			 MutationFunction<FMutation, typename C::value_type, RNG>
	C operator()(const C& c, RNG& random) const {
		C sol = c;
		std::uniform_int_distribution<int> sample(0,c.size()-1);
		int chosen = sample(random);
		sol[chosen] = mutate_element(c[chosen], random);
		return sol;
	}

};

template<typename FMutation>
class vector_all {
	FMutation mutate_element;
public:
	vector_all(const FMutation& mutate_element) :
		mutate_element(mutate_element) { }
		
	template<typename C, typename RNG>
	requires UniformRandomBitGenerator<RNG> &&
			 RandomAccessContainer<C> &&
			 MutationFunction<FMutation, typename C::value_type, RNG>
	C operator()(const C& c, RNG& random) const {
		C sol = c;
		for (int i = 0; i < int(c.size()); ++i) sol[i] = mutate_element(c[i], random);
		return sol;
	}

};
} //namespace mutation

namespace crossover {
class vector_onepoint {
public:
	template<typename C, typename RNG>
	requires UniformRandomBitGenerator<RNG> && 
			 RandomAccessContainer<C>
	C operator()(const C& c1, const C& c2, RNG& random) const {
		C sol = c2;
		std::uniform_int_distribution<int> sample(1,c1.size()-1);
		int chosen = sample(random);
		for (int i=0 ; i<chosen;++i) sol[i] = c1[i];
		return sol;
	}
};
} //namespace crossover

} // namespace opt
