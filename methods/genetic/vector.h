#pragma once

#include <random>
#include "concepts.h"
#include "../../utils/concepts.h"

namespace opt {

template<typename FMutation>
class mutationvector_single {
	FMutation mutate_element;
public:
	mutationvector_single(const FMutation& mutate_element) :
		mutate_element(mutate_element) { }
		
	template<typename C, typename RNG>
	requires UniformRandomBitGenerator<RNG> &&
			 RandomAccessContainer<C> &&
			 MutationFunction<FMutation, typename C::value_type, RNG>
	C operator()(const C& c, RNG& random) const {
		C sol = c;
		std::uniform_int_distribution<int> sample(0,c.size()-1);
		typename C::size_type chosen = sample(random);
		sol[chosen] = mutate_element(c[chosen], random);
		return sol;
	}

};

class crossovervector_onepoint {
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

} // namespace opt
