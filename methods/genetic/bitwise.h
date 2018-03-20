#pragma once

#include <type_traits>
#include <random>
#include "../../utils/concepts.h"

namespace opt {

template<typename Bit32, typename RNG>
requires UniformRandomBitGenerator<RNG> && (sizeof(Bit32) == 4)
Bit32 mutation32bit_swap(const Bit32& b, RNG& random) {
	uint32_t literal(*reinterpret_cast<const uint32_t*>(&b));
	std::uniform_int_distribution<int> sample(0,31);
	uint32_t solution = literal ^ (uint32_t(1) << sample(random));
	Bit32* typed_solution = reinterpret_cast<Bit32*>(&solution);
	return *typed_solution;
}

template<typename Bit32, typename RNG>
requires UniformRandomBitGenerator<RNG> && (sizeof(Bit32) == 4)
Bit32 crossover32bit_onepoint(const Bit32& b1, const Bit32& b2, RNG& random) {
	uint32_t literal(*reinterpret_cast<const uint32_t*>(&b1));
	uint32_t literal2(*reinterpret_cast<const uint32_t*>(&b2));
	std::uniform_int_distribution<int> sample(1,30);
	int splitat = sample(random);
	uint32_t solution = (literal & ((~0) << splitat)) & (literal2 & ((~0) >> (32 - splitat)));
	Bit32* typed_solution = reinterpret_cast<Bit32*>(&solution);
	return *typed_solution;
}

template<typename Bit64, typename RNG>
requires UniformRandomBitGenerator<RNG> && (sizeof(Bit64) == 8)
Bit64 mutation64bit_swap(const Bit64& b, RNG& random) {
	uint64_t literal(*reinterpret_cast<const uint64_t*>(&b));
	std::uniform_int_distribution<int> sample(0,63);
	uint64_t solution = literal ^ (uint64_t(1) << sample(random));
	Bit64* typed_solution = reinterpret_cast<Bit64*>(&solution);
	return *typed_solution;
}

template<typename Bit64, typename RNG>
requires UniformRandomBitGenerator<RNG> && (sizeof(Bit64) == 8)
Bit64 crossover64bit_onepoint(const Bit64& b1, const Bit64& b2, RNG& random) {
	uint64_t literal(*reinterpret_cast<const uint64_t*>(&b1));
	uint64_t literal2(*reinterpret_cast<const uint64_t*>(&b2));
	std::uniform_int_distribution<int> sample(1,62);
	int splitat = sample(random);
	uint64_t solution = (literal & ((~0) << splitat)) & (literal2 & ((~0) >> (64 - splitat)));
	Bit64* typed_solution = reinterpret_cast<Bit64*>(&solution);
	return *typed_solution;
}



} // namespace opt
