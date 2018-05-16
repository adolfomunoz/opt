# Genetic algorithm

A [genetic algorithm](https://en.wikipedia.org/wiki/Genetic_algorithm) is an optimization algorithm that is inspired by the process of natural selection, in which a population of solutions evolves through mutation, crossover and selection operators. It is called as follows:
```
opt::pattern_search(<iterations>, <step>, <epsilon>)
```
where:
* `<iterations>` represents the maximum number of iterations of the method.
* `<step>` represents the initial size of the neighbourhood to explore at each iteration.
* `<epsilon>` is the tolerance. When the step becomes smaller than this value, the algorithm stops.

The minimization with this method can include a parameter which represents the starting position for the exploration:
```
opt::minimize(<function>, opt::pattern_search(), <initial_position>)
```
where:
* `<initial_position>` is the starting point for the optimization. If omitted, it is the n-dimensional 0 of the adequate data type.

## Adequate explorable data types.

As stated above, this method explores an n-dimensional space. As such, it can only depend on data types that represent such n-dimensional space, such as collections (`std::array`, `std::list`, `std::vector`) of floating point numbers or tuples (`std::tuple`) in which all the data types are floating point numbers and the same. Other data types will lead to compilation errors.  

## Examples

In the following example we use the pattern search method with default parameters to minimize the distance to the point *[3.25,-1.5,0]* represented as an `std::array<float,3>`. Obviously, the minimum is exactly such point. 

```cpp
template<typename real>
constexpr real sqr(real r) { return r*r; } 

float distance(const std::array<float,3>& v) { 
	return sqr(v[0] - 3.25) + sqr(v[1] + 1.5) + sqr(v[2]);
}

int main(int argc, char** argv) {
	std::array<float,3> amin; 
	amin = opt::minimize(distance, opt::pattern_search(), std::array<float,3>{0.0f,0.0f,0.0f});
	std::cout<<"["<<amin[0]<<","<<amin[1]<<","<<amin[2]<<"] should be [3.25,-1.5,0]"<<std::endl;
}
```

Additionally, functions with multiple parameters are supported (as stated [here](fitting_multiple_parameters.md)) because they are translated into tuples, assuming that all the parameters are real numbers of the same data type. In the following example, we minimize the 2-dimensional [Beale function](https://en.wikipedia.org/wiki/Test_functions_for_optimization) (with minimum in *(3.0, 0.5)*), setting the three parameters of the pattern search algorithm to enforce precision:

```cpp
template<typename real>
constexpr real sqr(real r) { return r*r; } 
int main(int argc, char** argv) {
	double xmin, ymin;	
	std::tie(xmin, ymin) = opt::minimize(
            [] (double x, double y) { return sqr(1.5-x+x*y) + sqr(2.25-x+x*y*y) + sqr(2.625-x+x*y*y*y);},
            opt::pattern_search(1000, 1.0, 1.e-8)
        );
	std::cout<<"("<<xmin<<","<<ymin<<")"<<" should be (3,0.5)"<<std::endl;
}
```

