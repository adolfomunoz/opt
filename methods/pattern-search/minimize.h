#pragma once

#include "../../utils/null-ostream.h"
#include "../../utils/concepts.h"
#include "../../utils/tuple-array.h"

namespace opt {

/*************************************
 * Default calls strategies *
 *************************************/

template<typename F, typename XType, typename YType = decltype(std::declval<F>()(std::declval<XType>()))>
requires TargetFunction<F,XType,YType> && Container<XType>
XType minimize(const F& f, const PatternSearch& method, const XType& ini) {
	null_ostream os;
	return method.minimize(ini, f, os);
}

template<typename F, 
	typename XType = typename std::remove_cv_t<typename std::remove_reference_t<typename callable_traits<F>::template argument_type<0>>>, 
	typename YType = decltype(std::declval<F>()(std::declval<XType>()))>
requires TargetFunction<F,XType,YType> && Container<XType>
XType minimize(const F& f, const PatternSearch& method) {
	return minimize(f, method, XType());
}

template<typename F, 
	typename XType = typename std::remove_cv_t<typename std::remove_reference_t<typename callable_traits<F>::template argument_type<0>>>, 
	typename YType = decltype(std::declval<F>()(std::declval<XType>())),
	typename ArrayType = decltype(tuple_to_array(std::declval<XType>()))>
requires TargetFunction<F,XType,YType> && Tuple<XType>
XType minimize(const F& f, const PatternSearch& method) {	
	return array_to_tuple(minimize([f] (const ArrayType& x) { return f(array_to_tuple(x)); }, method, tuple_to_array(XType())));
}

}; // namespace opt

