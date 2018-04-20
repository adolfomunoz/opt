# Fitting functions

This library tries to minimize any function you want through the `minimize` command. The fitting function (to minimize) can be passed as parameters in different ways.

It can be applied to standard C++ functions (obtaining the square root of 121 through least squares):
```
float error_sqr11(float x) { return (121.0f - x*x)*(121.0f - x*x); }
int main(int argc, char** argv) {
	std::cout<<opt::minimize(error_sqr11)<<std::endl;
}
```

It can also be applied to callable objects (square root of 121 again):
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

Last, it becomes rather useful when applied to lambda functions (square root of 121 again):
```
int main(int argc, char** argv) {
	std::cout<<opt::minimize([=] (float x) { return (121.0f - x*x)*(121.0f - x*x); })<<std::endl;
}
```

The data type to be minimize is automatically deduced from the function parameter. Therefore, it cannot be applied to polymorphic lambda functions (with an *auto* parameter) or to callable objects with a templated `operator()`.




