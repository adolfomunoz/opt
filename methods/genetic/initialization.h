#pragma once

#include "concepts.h"
#include "vector.h"

namespace opt {
namespace initialization {

template<typename F>
//We need some concept here
auto population(unsigned int size, const F& f) {
	return vector(size, f)();	
}


} // namespace initialization
} // namespace opt
