#pragma once

#include "genetic-best.h"
#include "genetic-stochastic.h"
#include "genetic-stochastic-best.h"
#include "bitwise.h"
#include "vector.h"
#include "initialization.h"
#include "concepts.h"
#include "mutation.h"
#include "real.h"
#include "minimize.h"

namespace opt {

GeneticStochasticBest genetic(unsigned int iterations = 1000, unsigned int population = 10, unsigned int mutations = 10, unsigned int crossovers = 10, unsigned long seed =  (std::random_device())()) {
	return GeneticStochasticBest(iterations, population, mutations, crossovers, seed);
}

}; // namespace opt
