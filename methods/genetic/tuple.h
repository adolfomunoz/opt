#pragma once

#include <random>
#include <tuple>
#include "concepts.h"
#include "../../utils/concepts.h"

namespace opt {
namespace mutation {

template<typename... FMutation>
class tuple_single {
	std::tuple<FMutation...> mutate_element;

	template<typename RNG, typename T, std::size_t ...I>
	T mutate_single(const T& t, RNG& random, int i, std::index_sequence<I...>) {
		return T((i==I)?(std::get<I>(mutate_element)(std::get<I>(t), random)):(std::get<I>(t))...);
	}
public:
	tuple_single(const FMutation&... mutate_element) :
		mutate_element(mutate_element) { }
		
	template<typename RNG, typename Args...> 
	requires UniformRandomBitGenerator<RNG> // Need to check: whether T is a tuple &&
		 // MutationFunction<FMutation, typename C::value_type, RNG>
	std::tuple<Args...> operator()(const std::tuple<Args...>& t, RNG& random) const {
		std::uniform_int_distribution<int> sample(0,sizeof...(Args));
		return mutate_single(t, sample(random), random, std::index_sequence_for<Args...>);
	}

};

template<typename... FMutation>
class tuple_all {
	std::tuple<FMutation...> mutate_element;

	template<typename RNG, typename T, std::size_t ...I>
	T mutate_all(const T& t, RNG& random, std::index_sequence<I...>) {
		return T(std::get<I>(mutate_element)(std::get<I>(t), random)...)
	}
public:
	tuple_all(const FMutation&... mutate_element) :
		mutate_element(mutate_element) { }
		
	template<typename RNG, typename Args...> 
	requires UniformRandomBitGenerator<RNG> // Need to check: whether T is a tuple &&
		 // MutationFunction<FMutation, typename C::value_type, RNG>
	std::tuple<Args...> operator()(const std::tuple<Args...>& t, RNG& random) const {
		return mutate_single(t, random, std::index_sequence_for<Args...>);
	}

};


} //namespace mutation

/**
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

class vector_uniform {
public:
	template<typename C, typename RNG>
	requires UniformRandomBitGenerator<RNG> && 
			 RandomAccessContainer<C>
	C operator()(const C& c1, const C& c2, RNG& random) const {
		C sol = c2;
		std::uniform_int_distribution<int> sample(0,1); //Equal probability for both parents
		for (int i=0 ; (i<std::min(c1.size(),c2.size()));++i) 
			if (!sample(random)) sol[i] = c1[i];
		return sol;
	}
};

} //namespace crossover

namespace initialization {

template<typename F>
//We need some concept here
auto vector(unsigned int size, const F& f) {
	return [size, &f] () { std::vector<decltype(f())> s(size); for (unsigned int i=0; i<size; ++i) s[i]= f(); return s; };
}

template<unsigned int size, typename F>
auto array(const F& f) {
	return [&f] () { std::array<decltype(f()),size> s; for (unsigned int i=0; i<size; ++i) s[i]= f(); return s; };
}

} //namespace initialization
**/

} // namespace opt
