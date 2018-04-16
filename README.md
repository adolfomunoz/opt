# opt
A library for optimization methods written in C++17 with concepts.

This library provides a set of numerical optimization routines (for now, just genetic algorithms). Thanks to [C++ concepts](http://en.cppreference.com/w/cpp/language/constraints) each routine is able to select adequate strategies and optimization methods according to the involved data types and the provided information. 

## Installation

This library is **header-only** (except for the different examples). To obtain it you can use the command line:
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

This downloads (or updates) all the required dependencies into de `external` subfolder and compiles all the provided examples and tests and moves the executable files to the `bin` subfolder. Other cmake generators (such as Visual Studio) have not been tested.

## Usage

This library tries to minimize any function you want through the `minimize` command. 

It can be applied to standard C++ functions:
```
float error_sqr11(float x) { return (121.0f - x*x)*(121.0f - x*x); }
int main(int argc, char** argv) {
	std::cout<<opt::minimize(error_sqr11)<<std::endl;
}
```

It can also be applied to callable objects:
```
class CallableSqrError {
	float t;
public:
	CallableSqrError(float t) : t(t) { }
	float operator()(float x) const { return (t-x*x)*(t-x*x); }
};
int main(int argc, char** argv) {
	std::cout<<opt::minimize(CallableSqrError(121))<<std::endl;
}

```

Last, it becomes rather useful when applied to lambda functions:
```
int main(int argc, char** argv) {
	std::cout<<opt::minimize([=] (float x) { return (121.0f - x*x)*(121.0f - x*x); })<<std::endl;
}
```

The data type to be minimize is automatically deduced from the function parameter. Therefore, it cannot be applied to polymorphic lambda functions (with an *auto* parameter) or to callable objects with a templated `operator()`.


## Documentation

It is a work in progress.

## Dependencies

This library depends on [callable.hpp](https://github.com/sth/callable.hpp) for function traits.

## Disclaimer
This code is released under the [MIT License](https://en.wikipedia.org/wiki/MIT_License). In practice, if you use this code I'd like the following to happen:
* Let me know that you are using it.
* Let me know how can it be improved.


