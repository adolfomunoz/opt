#pragma once

#include <random>
#include <limits>
#include "concepts.h"
#include "../../utils/concepts.h"

namespace opt {

namespace initialization {

template<typename I, typename RNG>
requires std::is_integral_v<I> && UniformRandomBitGenerator<RNG>
auto int_uniform(RNG& random, I imin = std::numeric_limits<I>::min(), I imax = std::numeric_limits<I>::max()) {
	return [&random, imin, imax] () { std::uniform_int_distribution<I> sample(imin, imax); return sample(random); };
}

} //namespace initialization

} // namespace opt
