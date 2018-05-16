# Genetic algorithm

A [genetic algorithm](https://en.wikipedia.org/wiki/Genetic_algorithm) is an optimization algorithm that is inspired by the process of natural selection, in which a population of solutions evolves through mutation, crossover and selection operators. It is called as follows:
```
opt::genetic(<iterations>, <population>, <mutations>, <crossovers>)
```
where:
* `<iterations>` represents the maximum number of iterations of the method.
* `<population>` represents the population size. Bigger size will enable smarter selection but much slower convergence.
* `<mutations>` is the number of mutations for each iteration. It is related to the exploratory nature of the algorithm. A bigger value will explore further but will be slower.
* `<crossovers>` is the number of crossovers for each iteration. It is related to the convergence of the algorithm given two good solutions. A bigger value will explore closer to previous solutions but will be slower.

The minimization with this method can include the following: 
```
opt::minimize(<function>, opt::genetic(), <initial_population>, <mutation_operator>, <crossover_operator>)
```
where:
* `<initial_population>` is a collection of the element to optimize that represents the initial population from which the algorithm will start.
* `<mutation_operator>` is a function that given an element and a random number generator gives a mutated element.
* `<crossover_operator>` is a function that given two elements and a random number generator gives a crossover between the two.

Any (or all) of these three parameters can be omitted and will be set to a default option adapted to the data type of the element to optimize. 

## Example

The following example helps:

```cpp
int main(int argc, char** argv) { 
	//Calculates the cubic root of 27 (should be 3) with double precision and specific strategies
	std::cout<< opt::minimize(
		//Function to minimize
		[] (double r) { return std::abs(27.0 - r*r*r); }, 
		// Genetic method with 100 iterations, 10 population, 10 mutations and 5 crossovers per iteration
		opt::genetic(1000,10,10,5),
		//Starting population: uniformly distributed values from -100 to 100
		std::vector<double>{-100.0,-50.0, 0.0, 50.0, 100.0}, 
		// Mutation strategy: linear probability from distance 0 to 10
		[] (double r, auto& random) -> double {	
			std::array<double,3> values{r-10.0, r, r+10.0};
			std::array<double,3> probs{0.0, 1.0, 0.0};
			std::piecewise_linear_distribution<double> d(values.begin(), values.end(), probs.begin());
			return d(random);		
		},
		// Crossover strategy: random linear interpolation
		[] (double r1, double r2, auto& random) -> double {
			std::uniform_real_distribution<double> d(r1,r2);
			return d(random);
		}
	)<<std::endl;
}
```

TO-DO - Comment more

## Library of mutation and crossover strategies.

TO-DO

## Custom data types

TO-DO

