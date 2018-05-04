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

/*************************************
 * Default calls strategies *
 *************************************/
template<typename F, 
	typename XType = typename std::remove_cv_t<typename std::remove_reference_t<typename callable_traits<F>::template argument_type<0>>>, 
	typename YType = decltype(std::declval<F>()(std::declval<XType>()))>
requires GeneticMethod<Method> &&
         TargetFunction<F,XType,YType> 
XType minimize(const F& f, const HookeJeeves& method) {
	null_ostream os;
	std::mt19937 random;
	return method.minimize(initialization::population(100,init_default<XType>::strategy(random)), f, mutation_default<XType>::strategy, crossover_default<XType>::strategy, YType(1.e-6), os);
}


}; // namespace opt

