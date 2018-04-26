# Passing function parameters

`opt` is rather flexible regarding passing function parameters, such as fitting functions (or any other method-specific function parameter). There are several ways in which it can be done.

## As a standard C++ function.

It can be applied to standard C++ functions (obtaining the square root of 121 through least squares):
```cpp
float error_sqr11(float x) { return (121.0f - x*x)*(121.0f - x*x); }
int main(int argc, char** argv) {
	std::cout<<opt::minimize(error_sqr11)<<std::endl;
}
```
## As a callable object

It can also be applied to callable objects, with `operator()` (square root of 121 again):
```cpp
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

## As a lambda function

Last, it becomes rather useful when applied to lambda functions (square root of 121 again):
```cpp
int main(int argc, char** argv) {
	std::cout<<opt::minimize([=] (float x) { return (121.0f - x*x)*(121.0f - x*x); })<<std::endl;
}
```

## Polymorphic fitting functions

`opt::minimize` deduce the data types involved in a parameter function. Specifically, the return type of `opt::minimize` is the same as the parameter of the fitting function. 

This cannot be done in the case of templated (or overloaded) `operator()`:

```cpp
class Wrong {
public:
    template<typename T>
    float operator()(const T& t) const { return ...; }
}
int main(int argc, char** argv) {
        //Cannnot deduce data type
	std::cout<<opt::minimize(Wrong())<<std::endl;
}
```

Also, this affects polymorphic lambdas:

```cpp
int main(int argc, char** argv) {
        //Cannnot deduce data type
	std::cout<<opt::minimize([] (const auto& t) { return ...; })<<std::endl;
}
```

If such data type can be deduced from the parameters of the optimization method (when not omitted) then the polymorphic function can be applied.




