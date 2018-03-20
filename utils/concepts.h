#pragma once

template <typename RNG>
concept bool UniformRandomBitGenerator = 
   requires { typename RNG::result_type; }  
   && 
   requires(RNG a) {
	a.max();
	a.min();
	a();
   };
