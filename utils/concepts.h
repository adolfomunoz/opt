#pragma once

template <typename RNG>
concept bool UniformRandomBitGenerator = 
//   requires { typename RNG::value_type; }  
//   && 
   requires(RNG a) {
	a.max();
	a.min();
	a();
   };
