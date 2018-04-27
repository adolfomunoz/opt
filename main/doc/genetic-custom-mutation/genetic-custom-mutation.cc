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
	PolynomialCompare(const F& f, float xmin = 0.0f, float xmax = 1.0f, unsigned int nsamples = 100, unsigned long seed = (std::random_device())()) :
		f(f), xmin(xmin), xmax(xmax), nsamples(nsamples), random(seed) { }

	float operator()(const std::vector<float>& p) const {
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
	std::vector<float> polynomial;

	polynomial = opt::minimize(PolynomialCompare([] (float x) { return 2.0f*x + 1.0f; }), opt::genetic(), 
			[] (const std::vector<float>& p, auto& random) { 
				std::vector<float> sol = p;
				std::discrete_distribution<int> sample({0.2, 0.8});
				switch(sample(random)) {
					case 0:  sol.push_back(1.0f);
					default: return  opt::mutation::vector_single(opt::mutation::real_normal(1.0f))(sol, random);
				}
			});

	for (float coef : polynomial) std::cout<<coef<<" ";
	std::cout<<std::endl;
}
