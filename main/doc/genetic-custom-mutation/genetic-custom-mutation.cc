#include "../../../opt.h"
#include <cmath>
#include <vector>
#include <random>


//The vector of floats represents a polynomial going where the float at index i represents the coefficient to the power of x
float eval(const std::vector<float>& p, float x) {
	float powx = 1.0f;
	float sol  = 0.0f;
	for (std::size_t i = 0; i<p.size(); ++i) {
		sol += p[i]*powx; 
		powx*=x;	
	}
	return sol;	
}

//This does a MonteCarlo comparison between a function and a polynomial.
template<typename F>
class PolynomialCompare {
	F f; float xmin; float xmax; unsigned int nsamples;
	std::mt19937 random;
public:
	CompareToPolynomial(const F& f, float xmin = 0.0f, float xmax = 1.0f, unsigned int nsamples = 100, unsigned long seed = (std::random_device())()) :
		f(f), xmin(xmin), xmax(xmax), nsamples(nsamples), random(seed) { }

	float operator(const std::vector<float>& p) {
		float sum = 0.0f;
		std::uniform_real_distribution sample(xmin, xmax);
		for (unsigned int i = 0; i<nsamples; ++i) {
			float x = sample(random);
			float e = f(x) - eval(p,x);
			sum += e*e;
		}
		return sum/float(nsamples);
	}

};
 
int main(int argc, char** argv) {
	float minx, miny;
	//Ackley function (minima in 0,0)
	std::tie(minx,miny) = opt::minimize([] (float x, float y) 
		{ return -20.0f*std::exp(-0.2f*std::sqrt(0.5f*(x*x + y*y))) - 
		          std::exp(0.5f*(std::cos(2*M_PI*x) + std::cos(2*M_PI*y))) +
			  std::exp(1.0f) + 20.0f; });
	std::cout<<"Minima are "<<minx<<" and "<<miny<<std::endl;

	//With integers and arrays
	//Obtain two positive numbers which are not divisible between each other but are divisible by the same number
	std::array<int, 2> undivisible; int divisible;

	std::tie(undivisible, divisible) = opt::minimize([] (const std::array<int, 2>& n, int d)
			{  if( (n[0] <= d) || (n[1] <= d) || (d <= 1)) return 1.e6f;
			   else if ( ((n[0]%n[1])==0) || ((n[1]%n[0])==0) ) return 1.e3f;
			   else return float(n[0]%d) + float(n[1]%d); });
	std::cout<<undivisible[0]<<" and "<<undivisible[1]<<" are not divisible between each other but are divisible by "<<divisible<<std::endl;	
}
