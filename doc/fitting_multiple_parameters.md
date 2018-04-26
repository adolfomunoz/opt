# Fitting functions with multiple parameters

Fitting functions can have múltiple parameters. `opt::minimize` will return the minima through a `std::tuple`, deducing the corresponding types:

```cpp
int main(int argc, char** argv) {
	float minx, miny;
	//Ackley function (minima in 0,0)
	std::tie(minx,miny) = opt::minimize([] (float x, float y) 
		{ return -20.0f*std::exp(-0.2f*std::sqrt(0.5f*(x*x + y*y))) - 
		          std::exp(0.5f*(std::cos(2*M_PI*x) + std::cos(2*M_PI*y))) +
			  std::exp(1.0f) + 20.0f; });
	std::cout<<"Minima are "<<minx<<" and "<<miny<<std::endl;
}
```

This also works with different and varied data types:

```cpp
int main(int argc, char** argv) {
	std::array<int, 2> undivisible; int divisible;
	std::tie(undivisible, divisible) = opt::minimize([] (const std::array<int, 2>& n, int d)
			{  if( (n[0] <= d) || (n[1] <= d) || (d <= 1)) return 1.e6f;
			   else if ( ((n[0]%n[1])==0) || ((n[1]%n[0])==0) ) return 1.e3f;
			   else return float(n[0]%d) + float(n[1]%d); });
	std::cout<<undivisible[0]<<" and "<<undivisible[1]<<" are not divisible between each other but are divisible by "<<divisible<<std::endl;	
}
```

Again, this deduction cannot happen with polymorphic lambdas or with callable objects with a templated or overloaded `operator()`.




