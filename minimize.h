#pragma once

#include "methods/genetic/genetic.h"
#include "utils/callable-tuple.h"

namespace opt {

template<typename F,
	typename XType = typename std::remove_cv_t<typename std::remove_reference_t<typename callable_traits<F>::template argument_type<0>>>,
	typename YType = decltype(std::declval<F>()(std::declval<XType>()))>
requires callable_traits<F>::argc == 1 
XType minimize(const F& f) {
	return minimize(f, genetic());
}

template<typename F, typename... Args> 
requires callable_traits<F>::argc > 1 
auto minimize(const F& f, Args && ... args) {
	using XType = typename callable_tuple<F>::type;
	return minimize([&f] (const XType& x) { return std::apply(f, x); }, std::forward<Args>(args)...);
}


}; // namespace opt

