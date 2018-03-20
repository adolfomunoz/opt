#pragma once

#include "../../utils/concepts.h"

namespace opt {

template <typename FMutation, typename XType, typename RNG>
concept bool MutationFunction = 
   UniformRandomBitGenerator<RNG>
   &&
   requires(FMutation f, const XType& a,  RNG& rng) {
	f(a, rng) -> XType;
   };

} // namespace opt
