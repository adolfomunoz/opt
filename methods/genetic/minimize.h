#pragma once

#include <array>
#include "concepts.h"
#include "bitwise.h"
#include "vector.h"
#include "../../utils/traits.h"
#include "../../utils/null-ostream.h"

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
requires (sizeof(X) == 4)
struct crossover_default<X> {
	static constexpr auto strategy = crossover::bit32_onepoint();
};

template<typename X>
requires (sizeof(X) == 8)
struct crossover_default<X> {
	static constexpr auto strategy = crossover::bit64_onepoint();
};

template<typename C>
requires RandomAccessContainer<C>
struct mutation_default<C> {
	static constexpr auto strategy = mutation::vector_single(mutation_default<typename C::value_type>::strategy);
};

template<typename C>
requires RandomAccessContainer<C>
struct crossover_default<C> {
	static constexpr auto strategy = crossover::vector_onepoint();
};

template<typename Method, typename F, typename XType = first_argument_t<F>, typename YType = decltype(std::declval<F>()(std::declval<XType>()))>
requires GeneticMethod<Method> &&
         TargetFunction<F,XType,YType>
XType minimize(const F& f, const Method& method) {
	null_ostream os;
	return method.minimize(std::array<XType,1>{{XType(0)}}, f, mutation_default<XType>::strategy, crossover_default<XType>::strategy, YType(0), os);
}


}; // namespace opt

