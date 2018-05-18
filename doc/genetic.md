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

In the following example we find the cubic root of 27 with custom mutation and crossover operators and a custom crossover strategy:
* The initial population are 5 double values distributed uniformly between -100 and 100.
* The mutation strategy is a "cone" strategy: given the double value, a new value is generated with higher probability in the same value and linearly smaller probability up to a distance of 10 from the original value.
* The crossover strategy is a random linear interpolation between the two double parents.

```cpp
int main(int argc, char** argv) { 
	//Calculates the cubic root of 27 (should be 3) with double precision and specific strategies
	std::cout<< opt::minimize(
		//Function to minimize
		[] (double r) { return std::abs(27.0 - r*r*r); }, 
		// Genetic method with 100 iterations, 10 population, 10 mutations and 5 crossovers per iteration
		opt::genetic(100,10,10,5),
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

Note that these strategies and arbitrary and maybe suboptimal. This example just illustrates the possiblity of devising new problem-specific strategies. 

## Library of mutation and crossover strategies.

Besides custom strategies, `opt` provides a set of standard strageties (from which the default strategies are chosen), depending on the data type of the element to optimize.

These are the provided mutation strategies:
* `int`:
   * `opt::mutation::bit32_swap()` (default): represents the data as 32 bits and swaps one of them randomly.
   * `opt::mutation::numeric_uniform(<min>, <max>)`: chooses a random uniformly distributed between `<min>` and `<max>`. 
* `long int`:
   * `opt::mutation::bit64_swap()` (default): represents the data as 64 bits and swaps one of them randomly.
   * `opt::mutation::numeric_uniform(<min>, <max>)`: chooses a random uniformly distributed between `<min>` and `<max>`. 
   * `opt::mutation::real_normal(<stdev>)`: chooses a random number following a normal distribution centered on the input real number with standard deviation `<stdev>`. 
* `float`:
   * `opt::mutation::bit32_swap()` (default): represents the data as 32 bits and swaps one of them randomly.
   * `opt::mutation::numeric_uniform(<min>, <max>)`: chooses a random uniformly distributed between `<min>` and `<max>`. 
* `double`:
   * `opt::mutation::bit64_swap()` (default): represents the data as 64 bits and swaps one of them randomly.
   * `opt::mutation::numeric_uniform(<min>, <max>)`: chooses a random uniformly distributed between `<min>` and `<max>`. 
   * `opt::mutation::real_normal(<stdev>)`: chooses a random number following a normal distribution centered on the input real number with standard deviation `<stdev>`. 
* `std::array<T,N>` or `std::vector<T>`:
   * `opt::mutation::vector_single(<mutation>)` (default): applies the mutation strategy in `<mutation>` to a single random element `T` in the vector (or array).
   * `opt::mutation::vector_all(<mutation>)`: applies the mutation strategy in `<mutation>` to a all elements `T` in the vector (or array).
* `std::tuple<...>`:
   * `opt::mutation::tuple_single(<mutation1>, <mutation2>, ...)` (default): applies the mutation strategy to a random tuple element by applying the mutation strategy in the corresponding position of the parameter list (`<mutation1>, <mutation2>, ...`).
   * `opt::mutation::tuple_all(<mutation1>, <mutation2>, ...)`: applies each mutation strategy of the parameter list (`<mutation1>, <mutation2>, ...`) to the corresponding element in the tuple.
* Optimization function with multiple parameters:
   * `opt::mutation::single_parameter(<mutation1>, <mutation2>, ...)` (default): applies the mutation strategy to a random input parameter by applying the mutation strategy in the corresponding position of the parameter list (`<mutation1>, <mutation2>, ...`).
   * `opt::mutation::all_parameters(<mutation1>, <mutation2>, ...)`: applies each mutation strategy of the parameter list (`<mutation1>, <mutation2>, ...`) to the corresponding parameter of the optimization function.
* General, available to all data types:
   * `opt::mutation::repeat(<times>, <mutation>)`: applies the mutation strategy in `<mutation>` a number of times `<times>`.
  
These are the provided crossover strategies:
 * `int`:
   * `opt::crossover::bit32_onepoint()` (default): represents the data as 32 bits and combines both bit streams at a random single point (left part from one parent, right part from the other parent).
* `long int`:
   * `opt::crossover::bit64_onepoint()` (default): represents the data as 64 bits and combines both bit streams at a random single point (left part from one parent, right part from the other parent).
* `float`:
   * `opt::crossover::bit32_onepoint()` (default): represents the data as 32 bits and combines both bit streams at a random single point (left part from one parent, right part from the other parent).
* `double`:
   * `opt::crossover::bit64_onepoint()` (default): represents the data as 64 bits and combines both bit streams at a random single point (left part from one parent, right part from the other parent).
* `std::array<T,N>` or `std::vector<T>`:
   * `opt::crossover::vector_onepoint()` (default): combines both vectors (or arrays) at a random single point (left part from one parent, right part from the other parent).
   * `opt::crossover::vector_uniform()`: combines both vectors (or arrays) by choosing the element `T` at each position of the vector by randomly choosing the one of the parent's elements at the same poisiton.
* `std::tuple<...>` (or optimization function with multiple parameters):
   * `opt::crossover::tuple_onepoint()` (default for big tuples): combines both tuples at a random single point (left part from one parent, right part from the other parent).
   * `opt::crossover::tuple_uniform()` (default for small tuples): combines both tuples by choosing the element at each position of the tuple by randomly choosing the one of the parent's elements at the same poisiton.


## Custom data types

TO-DO

