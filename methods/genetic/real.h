#pragma once

#include <random>
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

} // namespace opt
