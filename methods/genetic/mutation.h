#pragma once

#include "concepts.h"

namespace opt {
namespace mutation {

//We rely on constructor template deduction (C++17)
template<typename FMutation>
class repeat {
	FMutation f;
	int ntimes;
public:
	repeat(const FMutation& f, int ntimes) : f(f), ntimes(ntimes) { }

	template<typename XType, typename RNG>
	requires MutationFunction<FMutation, XType, RNG>
	XType operator()(const XType& x, RNG& rng) const {
		XType s = x;
		for (int i=0;i<ntimes;++i) s = f(s,rng);
		return s;
	}
};


class object_method {
public:
	template<typename O, typename RNG>
	requires MutableObject<O,RNG>
	O operator()(const O& x, RNG& random) const {
		return x.mutate(random);
	}	
};

} // namespace mutation
} // namespace opt
