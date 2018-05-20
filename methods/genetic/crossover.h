#pragma once

#include "concepts.h"

namespace opt {
namespace crossover {


class object_method {
public:
	template<typename O, typename RNG>
	requires CrossableObject<O,RNG>
	O operator()(const O& x, const O& y, RNG& random) const {
		return x.cross(y, random);
	}	
};

} // namespace mutation
} // namespace opt
