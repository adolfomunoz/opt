#pragma once

#include "concepts.h"

namespace opt {

template<typename XType, typename RNG, typename FMutation>
//requires MutationFunction<FMutation, XType, RNG>
auto mutation_repeat(const FMutation& f, int ntimes) {
	return [=] (const XType& x, RNG& rng) {
		XType y = x;
		for (int i=0;i<ntimes;++i) y = f(y,rng);
		return y;
	};
}

} // namespace opt
