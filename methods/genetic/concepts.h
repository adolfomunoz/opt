#pragma once

#include "../../utils/concepts.h"

namespace opt {

template <typename FMutation, typename XType, typename RNG>
concept bool MutationFunction = 
   UniformRandomBitGenerator<RNG> &&
   requires(FMutation f, const XType& a, XType b, RNG& rng) {
	b = f(a, rng);
   };

template <typename FCrossover, typename XType, typename RNG>
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
} // namespace opt
