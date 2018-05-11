#pragma once

#include <array>
#include <iostream>
#include <functional>
#include "../../utils/concepts.h"
#include "bitwise.h"

namespace opt {

template <typename FMutation, typename XType, typename RNG = std::mt19937>
concept bool MutationFunction = 
   UniformRandomBitGenerator<RNG> &&
   requires(FMutation f, const XType& a, XType b, RNG& rng) {
	b = f(a, rng);
   };

template <typename FCrossover, typename XType, typename RNG = std::mt19937>
concept bool CrossoverFunction = 
   UniformRandomBitGenerator<RNG> &&
   requires(FCrossover f, const XType& a1, const XType& a2, XType b, RNG& rng) {
	b = f(a1, a2, rng);
   };

template<typename FTarget, typename XType, typename YType>
concept bool TargetFunction =
   requires(FTarget f, const XType& x, YType y) {
	y = f(x);
   };

template<typename O, typename RNG = std::mt19937>
concept bool MutableObject =
   requires(O a, O b, RNG& random) { 
       b = a.mutate(random);
   };

template<typename Method>
concept bool GeneticMethod =
    requires(const Method& m, std::function<float(float)> target, const std::array<float,1>& init, float sol) {
	sol = m.minimize(init, target, mutation::bit32_swap(), crossover::bit32_onepoint(), 0.0f, std::cout); 
    };
} // namespace opt
