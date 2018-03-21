#pragma once

#include "concepts.h"

namespace testfunction {
/**
 * Simple Rosenbrock function implementation
 *
 * Reference: https://en.wikipedia.org/wiki/Rosenbrock_function
 **/
class rosenbrock {
	float a;
	float b;
public:
	rosenbrock(float a = 1, float b = 100) : a(a), b(b) { }

	template<typename V>
	requires RandomAccessContainer<V>
	float operator()(const V& v) const
	{	auto x = v[0];	auto y = v[1];
		return (a - x)*(a - x) + b*(y - x*x)*(y - x*x); 
	}

	template<typename V>
	requires RandomAccessContainer<V>
	V gradient(const V& v) const
	{	auto x = v[0];	auto y = v[1];
		return V{{-2.0f*(a - x) - 2.0f*b*x*(y - x*x), 2.0f*b*(y - x*x)}};	
	}

	template<typename V>
	requires RandomAccessContainer<V>
	float error(const V& v) const {
		return std::sqrt((v[0] - a)*(v[0] - a) + (v[1] - a*a)*(v[1] - a*a));
	}
};

} //namespace testfunction