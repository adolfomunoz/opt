#pragma once

#include <array>
#include "concepts.h"
#include "crossover.h"
#include "bitwise.h"
#include "vector.h"
#include "initialization.h"
#include <callable/callable.hpp>
#include "../../utils/null-ostream.h"
#include "../../utils/concepts.h"

namespace opt {

/*************************************
 * Default mutation strategies       *
 *************************************/

template<typename X>
struct mutation_default { };

template<typename X>
requires MutableObject<X>
struct mutation_default<X> {
	static constexpr auto strategy = mutation::object_method();
};

template<typename X>
requires (sizeof(X) == 4) && (!RandomAccessContainer<X>) && (!MutableObject<X>)
struct mutation_default<X> {
	static constexpr auto strategy = mutation::bit32_swap();
};

template<typename X>
requires (sizeof(X) == 8) && (!RandomAccessContainer<X>) && (!MutableObject<X>)
struct mutation_default<X> {
	static constexpr auto strategy = mutation::bit64_swap();
};

template<typename C>
requires RandomAccessContainer<C>
struct mutation_default<C> {
	static constexpr auto strategy = mutation::vector_single(mutation_default<typename C::value_type>::strategy);
};

template<typename... Args>
struct mutation_default<std::tuple<Args...>> {
	static constexpr auto strategy = mutation::tuple_single(mutation_default<Args>::strategy...);
};

/*************************************
 * Default crossover strategies      *
 *************************************/

template<typename X>
struct crossover_default { };

template<typename X>
requires CrossableObject<X>
struct crossover_default<X> {
	static constexpr auto strategy = crossover::object_method();
};

template<typename X>
requires (sizeof(X) == 4)  && (!RandomAccessContainer<X>) && (!CrossableObject<X>)
struct crossover_default<X> {
	static constexpr auto strategy = crossover::bit32_onepoint();
};

template<typename X>
requires (sizeof(X) == 8)  && (!RandomAccessContainer<X>) && (!CrossableObject<X>)
struct crossover_default<X> {
	static constexpr auto strategy = crossover::bit64_onepoint();
};

template<typename C>
requires RandomAccessContainer<C>
struct crossover_default<C> {
	static constexpr auto strategy = crossover::vector_onepoint();
};

template<typename... Args>
requires sizeof...(Args) < 6
struct crossover_default<std::tuple<Args...>> {
	static constexpr auto strategy = crossover::tuple_onepoint();
};

template<typename... Args>
requires sizeof...(Args) >= 6
struct crossover_default<std::tuple<Args...>> {
	static constexpr auto strategy = crossover::tuple_uniform();
};

/*************************************
 * Default initialization strategies *
 *************************************/

template<typename X>
struct init_default {
	template<typename RNG>
	requires UniformRandomBitGenerator<RNG>
	static auto strategy(RNG& random) { return [] () { return X(); }; }
};

template<typename X>
requires std::is_integral_v<X> 
struct init_default<X> {
	template<typename RNG>
	requires UniformRandomBitGenerator<RNG>
	static auto strategy(RNG& random) { return initialization::int_uniform<X>(random,X(-1000),X(1000)); }
};

template<typename X>
requires std::is_floating_point_v<X> 
struct init_default<X> {
	template<typename RNG>
	requires UniformRandomBitGenerator<RNG>
	static auto strategy(RNG& random) { return initialization::real_uniform<X>(random,X(-1000),X(1000)); }
};

template<typename X>
struct init_default<std::vector<X>> {
	template<typename RNG>
	requires UniformRandomBitGenerator<RNG>
	static auto strategy(RNG& random) { return initialization::vector(1,init_default<X>::strategy(random)); }
};

template<typename X, std::size_t N>
struct init_default<std::array<X,N>> {
	template<typename RNG>
	requires UniformRandomBitGenerator<RNG>
	static auto strategy(RNG& random) { return initialization::array<N>(init_default<X>::strategy(random)); }
};


template<typename... Args>
struct init_default<std::tuple<Args...>> {
	template<typename RNG>
	requires UniformRandomBitGenerator<RNG>
	static auto strategy(RNG& random) { return initialization::tuple(init_default<Args>::strategy(random)...); }
};




/*************************************
 * Default calls strategies *
 *************************************/
template<typename Method, typename F, 
	typename XType = typename std::remove_cv_t<typename std::remove_reference_t<typename callable_traits<F>::template argument_type<0>>>, 
	typename YType = decltype(std::declval<F>()(std::declval<XType>())),
	typename InitialPopulation, typename FMutation, typename FCrossover>
requires GeneticMethod<Method> &&
         TargetFunction<F,XType,YType> &&
	 Container<InitialPopulation> &&
//	 std::is_same_v<InitialPopulation::value_type,XType> &&
	 MutationFunction<FMutation, XType> &&
	 CrossoverFunction<FCrossover, XType>
XType minimize(const F& f, const Method& method, const InitialPopulation& initial_population, const FMutation& mutation_strategy, const FCrossover& crossover_strategy) {
	null_ostream os;
	return method.minimize(initial_population, f, mutation_strategy, crossover_strategy, YType(1.e-10), os);
}

template<typename Method, typename F, 
	typename XType = typename std::remove_cv_t<typename std::remove_reference_t<typename callable_traits<F>::template argument_type<0>>>, 
	typename YType = decltype(std::declval<F>()(std::declval<XType>())),
	typename InitialPopulation>
requires GeneticMethod<Method> &&
         TargetFunction<F,XType,YType> &&
	 Container<InitialPopulation> 
//	 std::is_same_v<InitialPopulation::value_type,XType> 
XType minimize(const F& f, const Method& method, const InitialPopulation& initial_population) {
	return minimize(f,method, initial_population, mutation_default<XType>::strategy, crossover_default<XType>::strategy);
}


template<typename Method, typename F, 
	typename XType = typename std::remove_cv_t<typename std::remove_reference_t<typename callable_traits<F>::template argument_type<0>>>, 
	typename YType = decltype(std::declval<F>()(std::declval<XType>())),
	typename FMutation, typename FCrossover>
requires GeneticMethod<Method> &&
         TargetFunction<F,XType,YType> &&
	 MutationFunction<FMutation, XType> &&
	 CrossoverFunction<FCrossover, XType>
XType minimize(const F& f, const Method& method, const FMutation& mutation_strategy, const FCrossover& crossover_strategy) {
	std::mt19937 random;
	return minimize(f, method, initialization::population(100,init_default<XType>::strategy(random)), mutation_strategy, crossover_strategy);
}


template<typename Method, typename F, 
	typename XType = typename std::remove_cv_t<typename std::remove_reference_t<typename callable_traits<F>::template argument_type<0>>>, 
	typename YType = decltype(std::declval<F>()(std::declval<XType>())),
	typename FCrossover>
requires GeneticMethod<Method> &&
         TargetFunction<F,XType,YType> &&
	 CrossoverFunction<FCrossover, XType>
XType minimize(const F& f, const Method& method, const FCrossover& crossover_strategy) {
	return minimize(f,method, mutation_default<XType>::strategy, crossover_strategy);
}


template<typename Method, typename F, 
	typename XType = typename std::remove_cv_t<typename std::remove_reference_t<typename callable_traits<F>::template argument_type<0>>>, 
	typename YType = decltype(std::declval<F>()(std::declval<XType>())),
	typename FMutation>
requires GeneticMethod<Method> &&
         TargetFunction<F,XType,YType> &&
	 MutationFunction<FMutation, XType>
XType minimize(const F& f, const Method& method, const FMutation& mutation_strategy) {
	return minimize(f,method, mutation_strategy, crossover_default<XType>::strategy);
}


template<typename Method, typename F, 
	typename XType = typename std::remove_cv_t<typename std::remove_reference_t<typename callable_traits<F>::template argument_type<0>>>, 
	typename YType = decltype(std::declval<F>()(std::declval<XType>()))>
requires GeneticMethod<Method> &&
         TargetFunction<F,XType,YType> 
XType minimize(const F& f, const Method& method) {
	return minimize(f,method, mutation_default<XType>::strategy);
}


}; // namespace opt

