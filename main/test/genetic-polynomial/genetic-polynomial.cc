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
	F f; float xmin; float xmax; unsigned int nsamples; unsigned long seed;
public:
	PolynomialCompare(const F& f, float xmin = 0.0f, float xmax = 1.0f, unsigned int nsamples = 100, unsigned long seed = (std::random_device())()) :
		f(f), xmin(xmin), xmax(xmax), nsamples(nsamples), seed(seed) { }

	float operator()(const std::vector<float>& p) const {
		std::mt19937 random(seed);
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

template<typename FMutation, typename X>
class AddAndMutate {
	FMutation mutate_default;
	X x_to_add;
public:
	AddAndMutate(const FMutation& mutate_default, const X& x_to_add) :
		mutate_default(mutate_default), x_to_add(x_to_add) { }

	template<typename RNG>
	requires opt::MutationFunction<FMutation, std::vector<X>, RNG>
	std::vector<X> operator()(const std::vector<X>& vx, RNG& random) const {
		std::vector<X> sol = vx;
		std::discrete_distribution<int> sample({0.5, 0.5, double(vx.size())});
		switch (sample(random)) {
			case 0: sol.push_back(x_to_add);          break;
			case 1: if (sol.size()>1) sol.pop_back(); break;
		}
		return mutate_default(sol, random);
	}
};
 
int main(int argc, char** argv) {
	std::vector<float> polynomial;

	polynomial = opt::minimize(PolynomialCompare([] (float x) { return 2.0f*x + 1.0f; }), opt::genetic(), 
			AddAndMutate(opt::mutation::vector_single(opt::mutation::real_normal(1.0f)), 1.0f));

	for (float coef : polynomial) std::cout<<coef<<" ";
	std::cout<<std::endl;

	polynomial = opt::minimize(PolynomialCompare([] (float x) { return x*x - 2.0f*x + 1.0f; }), opt::genetic(), 
			AddAndMutate(opt::mutation::vector_single(opt::mutation::real_normal(1.0f)), 1.0f));

	for (float coef : polynomial) std::cout<<coef<<" ";
	std::cout<<std::endl;

	polynomial = opt::minimize(PolynomialCompare([] (float x) { return std::cos(x); }, 0.0f, float(M_PI)), opt::genetic(100000), 
			AddAndMutate(opt::mutation::vector_single(opt::mutation::real_normal(1.0f)), 1.0f));

	for (float coef : polynomial) std::cout<<coef<<" ";
	std::cout<<std::endl;
}
