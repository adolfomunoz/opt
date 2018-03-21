#pragma once

#include "concepts.h"

namespace opt {

//We rely on constructor template deduction (C++17)
template<typename FMutation>
class mutation_repeat {
	FMutation f;
	int ntimes;
public:
	mutation_repeat(const FMutation& f, int ntimes) : f(f), ntimes(ntimes) { }

	template<typename XType, typename RNG>
	requires MutationFunction<FMutation, XType, RNG>
	XType operator()(const XType& x, RNG& rng) const {
		XType s = x;
		for (int i=0;i<ntimes;++i) s = f(s,rng);
		return s;
	}
};

} // namespace opt
