#pragma once

#include <type_traits>
#include <random>
#include "../../utils/concepts.h"

namespace opt {

template<typename Bit32, typename RNG>
requires UniformRandomBitGenerator<RNG> && (sizeof(Bit32) == 4)
Bit32 mutation32bit(const Bit32& b, RNG& random) {
	uint32_t literal(b);
	std::uniform_int_distribution<int> sample(0,31);
	return Bit32(literal ^ (uint32_t(1) << sample(random)));
}

template<typename Bit32, typename RNG>
requires UniformRandomBitGenerator<RNG> && (sizeof(Bit32) == 4)
Bit32 crossover32bit(const Bit32& b1, const Bit32& b2, RNG& random) {
	uint32_t literal(b1);
	uint32_t literal2(b2);
	std::uniform_int_distribution<int> sample(1,30);
	int splitat = sample(random);
	return Bit32((literal & ((~0) << splitat)) & (literal2 & ((~0) >> (32 - splitat))));
}

template<typename Bit64, typename RNG>
requires UniformRandomBitGenerator<RNG> && (sizeof(Bit64) == 8)
Bit64 mutation64bit(const Bit64& b, RNG& random) {
	uint64_t literal(b);
	std::uniform_int_distribution<int> sample(0,63);
	return Bit32(literal ^ (uint32_t(1) << sample(random)));
}

template<typename Bit64, typename RNG>
requires UniformRandomBitGenerator<RNG> && (sizeof(Bit64) == 8)
Bit64 crossover64bit(const Bit64& b1, const Bit64& b2, RNG& random) {
	uint64_t literal(b1);
	uint64_t literal2(b2);
	std::uniform_int_distribution<int> sample(1,62);
	int splitat = sample(random);
	return Bit64((literal & ((~0) << splitat)) & (literal2 & ((~0) >> (64 - splitat))));
}



} // namespace opt
