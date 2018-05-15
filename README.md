# opt
Alibrary for optimization methods written in C++17 with concepts.

This library provides a set of numerical optimization routines (for now, just genetic algorithms). Thanks to [C++ concepts](http://en.cppreference.com/w/cpp/language/constraints) each routine is able to select adequate strategies and optimization methods according to the involved data types and the provided information. 

## Installation

This library is **header-only** (except for the examples). To obtain it you can use the command line:
```
git clone https://github.com/adolfomunoz/opt.git
```

To compile any code that depends on this library, however, you require both the C++17 standard and concepts activated:
```
g++ --std=c++17 -fconcepts -O3 yoursourcecode.cc
```

These are only available in **gcc >= 7.0**. This library has been tested with gcc 7.2.0. There are several examples provided, which you can compile from the command line as stated.

If you'd rather use [cmake](https://cmake.org/) there is a `CMakeLists.txt` provided, which can be used as follows:
```
mkdir build
cd build
cmake -G "Unix Makefiles" .. -DCMAKE_BUILD_TYPE=Release
make
```

This downloads (or updates) all the required dependencies into the `external` subfolder and compiles all the provided examples and tests and moves the executable files to the `bin` subfolder. Other cmake generators (such as Visual Studio) have not been tested.

## Documentation

This library tries to minimize any function you want through the `minimize` command, which has the following syntax:

```
opt::minimize(<function>, <method>, <optimization parameters>)
```
where
* `<function>` represents the function to minimize.
* `<method>` represents the method to use in the optimization.
* `<optimization parameters>` represents the per-minimization specific parameters (which vary depending on the optimization method).

Both `<method>` and `<optimization parameters>` can be omitted, and the `minimize` function will try to deduce the best optimization method (generally, genetic algorithms) together with the most adequate parameters, according to the data types involved in the fitting function:

```cpp
int main(int argc, char** argv) {
	std::cout<<opt::minimize([=] (float x) { return (121.0f - x*x)*(121.0f - x*x); })<<std::endl;
}
```

* Fitting functions.
   * [Passing function parameters](doc/function_parameters.md)
   * [Fitting functions with multiple parameters.](doc/fitting_multiple_parameters.md)
* Optimization methods.
   * Genetic algorithms. (default)
   * [Pattern search.](doc/pattern_search.md)

## Dependencies

This library depends on [callable.hpp](https://github.com/sth/callable.hpp) for function traits.

## Disclaimer
This code is released under the [GPL-3.0 license](https://www.gnu.org/licenses/gpl-3.0.en.html). In practice, if you use this code I'd like the following to happen:
* Let me know that you are using it.
* Let me know how can it be improved.


