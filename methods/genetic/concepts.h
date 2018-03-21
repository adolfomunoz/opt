#pragma once

#include "../../utils/concepts.h"

namespace opt {

template <typename FMutation, typename XType, typename RNG>
concept bool MutationFunction = 
   UniformRandomBitGenerator<RNG>
   &&
   requires(FMutation f, const XType& a, XType b, RNG& rng) {
	b = f(a, rng);
   };

} // namespace opt
