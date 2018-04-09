#pragma once

#include <random>
#include <limits>
#include "concepts.h"
#include "../../utils/concepts.h"

namespace opt {

namespace mutation {

template <typename R>
requires RealNumber<R>
class real_uniform {
	R rmin, rmax;
public:
	real_uniform(R rmin, R rmax) :
		rmin(rmin), rmax(rmax) { }
		
	template<typename RNG>
	requires UniformRandomBitGenerator<RNG> 
	R operator()(const R& c, RNG& random) const {
		//The input c is just ignored
		std::uniform_real_distribution<R> sample(rmin, rmax);
	       	return sample(random);
	}
};

template <typename R>
requires RealNumber<R>
class real_normal {
	R stddev;
public:
	real_normal(R stddev) :
		stddev(stddev) { }
		
	template<typename RNG>
	requires UniformRandomBitGenerator<RNG> 
	R operator()(const R& c, RNG& random) const {
		//The input c is just ignored
		std::normal_distribution<R> sample(c, stddev);
	       	return sample(random);
	}
};

} //namespace mutation

namespace initialization {

template<typename R, typename RNG>
requires RealNumber<R> && UniformRandomBitGenerator<RNG>
auto real_uniform(RNG& random, R rmin = std::numeric_limits<R>::min(), R rmax = std::numeric_limits<R>::max()) {
	return [&random, rmin, rmax] () { std::uniform_real_distribution<R> sample(rmin, rmax); return sample(random); };
}

} //namespace initialization

} // namespace opt
