#pragma once

#include "concepts.h"
#include "int.h"
#include "real.h"

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

template<typename N>
requires std::is_integral_v<N>
int_uniform<N> numeric_uniform(const N& nmin, const N& nmax) { return int_uniform<N>(nmin, nmax); }

template<typename N>
requires std::is_floating_point_v<N>
real_uniform<N> numeric_uniform(const N& nmin, const N& nmax) { return real_uniform<N>(nmin, nmax); }


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
