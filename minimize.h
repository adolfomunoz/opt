#pragma once

#include "methods/genetic/genetic.h"

namespace opt {

template<typename F, 
	typename XType = typename callable_traits<F>::template argument_type<0>, 
	typename YType = decltype(std::declval<F>()(std::declval<XType>()))>
requires TargetFunction<F,XType,YType> 
XType minimize(const F& f) {
	return minimize(f, genetic());
}


}; // namespace opt

