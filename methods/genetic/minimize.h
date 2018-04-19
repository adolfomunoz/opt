#pragma once

#include <array>
#include "concepts.h"
#include "bitwise.h"
#include "vector.h"
#include "initialization.h"
#include <callable/callable.hpp>
#include "../../utils/null-ostream.h"
#include "../../utils/concepts.h"

namespace opt {

template<typename X>
struct mutation_default { };

template<typename X>
struct crossover_default { };

template<typename X>
struct init_default { };

template<typename X>
requires (sizeof(X) == 4)
struct mutation_default<X> {
	static constexpr auto strategy = mutation::bit32_swap();
};

template<typename X>
requires (sizeof(X) == 8)
struct mutation_default<X> {
	static constexpr auto strategy = mutation::bit64_swap();
};

template<typename X>
requires std::is_integral_v<X> 
struct init_default<X> {
	template<typename RNG>
	requires UniformRandomBitGenerator<RNG>
	static auto strategy(RNG& random) { return initialization::int_uniform<X>(random); }
};

template<typename X>
requires std::is_floating_point_v<X> 
struct init_default<X> {
	template<typename RNG>
	requires UniformRandomBitGenerator<RNG>
	static auto strategy(RNG& random) { return initialization::real_uniform<X>(random); }
};

template<typename X>
requires (sizeof(X) == 4)
struct crossover_default<X> {
	static constexpr auto strategy = crossover::bit32_onepoint();
};

template<typename X>
requires (sizeof(X) == 8)
struct crossover_default<X> {
	static constexpr auto strategy = crossover::bit64_onepoint();
};

template<typename X>
struct init_default<std::vector<X>> {
	template<typename RNG>
	requires UniformRandomBitGenerator<RNG>
	static auto strategy(RNG& random) { return initialization::vector(1,init_default<X>::strategy(random)); }
};

template<typename X, unsigned int N>
struct init_default<std::array<X,N>> {
	template<typename RNG>
	requires UniformRandomBitGenerator<RNG>
	static auto strategy(RNG& random) { return initialization::vector(init_default<X>::strategy(random)); }
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

template<typename... Args>
struct init_default<std::tuple<Args...>> {
	template<typename RNG>
	requires UniformRandomBitGenerator<RNG>
	static auto strategy(RNG& random) { return initialization::tuple(init_default<Args>::strategy(random)...); }
};

template<typename Method, typename F, 
	typename XType = typename std::remove_cv_t<typename std::remove_reference_t<typename callable_traits<F>::template argument_type<0>>>, 
	typename YType = decltype(std::declval<F>()(std::declval<XType>()))>
requires GeneticMethod<Method> &&
         TargetFunction<F,XType,YType> 
XType minimize(const F& f, const Method& method) {
	null_ostream os;
	std::mt19937 random;
	return method.minimize(initialization::population(100,init_default<XType>::strategy(random)), f, mutation_default<XType>::strategy, crossover_default<XType>::strategy, YType(1.e-6), os);
}


}; // namespace opt

