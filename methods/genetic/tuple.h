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
	T mutate_single(const T& t, RNG& random, int i, std::index_sequence<I...>) const {
		return T((i==I)?(std::get<I>(mutate_element)(std::get<I>(t), random)):(std::get<I>(t))...);
	}
public:
	constexpr tuple_single(const FMutation&... mutate_element) :
		mutate_element(mutate_element...) { }
		
	template<typename RNG, typename... Args> 
	requires UniformRandomBitGenerator<RNG> &&
	         (sizeof...(Args) == sizeof...(FMutation)) // Need to check type compatibility on mutations 
		 // MutationFunction<FMutation, typename C::value_type, RNG>	
	std::tuple<Args...> operator()(const std::tuple<Args...>& t, RNG& random) const {
		std::uniform_int_distribution<int> sample(0,sizeof...(Args)-1);
		return mutate_single(t, random, sample(random), std::index_sequence_for<Args...>{});
	}

};

template<typename... FMutation>
class tuple_all {
	std::tuple<FMutation...> mutate_element;

	template<typename RNG, typename T, std::size_t ...I>
	T mutate_all(const T& t, RNG& random, std::index_sequence<I...>) const {
		return T(std::get<I>(mutate_element)(std::get<I>(t), random)...);
	}
public:
	constexpr tuple_all(const FMutation&... mutate_element) :
		mutate_element(mutate_element...) { }
		
	template<typename RNG, typename... Args> 
	requires UniformRandomBitGenerator<RNG> &&
	         (sizeof...(Args) == sizeof...(FMutation)) // Need to check type compatibility on mutations 
		 // MutationFunction<FMutation, typename C::value_type, RNG>
	std::tuple<Args...> operator()(const std::tuple<Args...>& t, RNG& random) const {
		return mutate_all(t, random, std::index_sequence_for<Args...>{});
	}
};


} //namespace mutation


namespace crossover {
class tuple_onepoint {
	template<typename T, std::size_t ...I>
	T crossover_at(const T& t1, const T& t2, unsigned int i, std::index_sequence<I...>) const {
		return T(((I<i)?std::get<I>(t1):std::get<I>(t2))...);
	}
public:
	template<typename RNG, typename... Args> 
	requires UniformRandomBitGenerator<RNG> 	
	std::tuple<Args...> operator()(const std::tuple<Args...>& t1, const std::tuple<Args...>& t2, RNG& random) const {
		std::uniform_int_distribution<int> sample(1,sizeof...(Args)-1);
		return crossover_at(t1, t2, sample(random), std::index_sequence_for<Args...>{});
	}

};

class tuple_uniform {
	template<typename RNG, typename T, std::size_t ...I>
	T crossover(const T& t1, const T& t2, RNG& random, std::index_sequence<I...>) const {
		std::uniform_int_distribution<int> sample(0,1);
		return T((sample(random)?std::get<I>(t1):std::get<I>(t2))...);
	}
public:
	template<typename RNG, typename... Args> 
	requires UniformRandomBitGenerator<RNG> 	
	std::tuple<Args...> operator()(const std::tuple<Args...>& t1, const std::tuple<Args...>& t2, RNG& random) const {
		std::uniform_int_distribution<int> sample(1,sizeof...(Args)-1);
		return crossover(t1, t2, random, std::index_sequence_for<Args...>{});
	}

};

} //namespace crossover


namespace initialization {

template<typename... FIni>
class tuple {
	std::tuple<FIni...> init;

	template<std::size_t ...I>
	auto init_all(std::index_sequence<I...>) const {
		return std::tuple(std::get<I>(init)()...);
	}
public:
	tuple(const FIni&... init) :
		init(init...) { }
		
	auto operator()() const {
		return init_all(std::index_sequence_for<FIni...>{});
	}
};

} //namespace initialization
} // namespace opt
