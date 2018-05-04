#include "../../../opt.h"
#include <array>
#include <iostream>

template<typename real>
constexpr real sqr(real r) { return r*r; } 

float distance(const std::array<float,3>& v) { 
	return sqr(v[0] - 3.25) + sqr(v[1] + 1.5) + sqr(v[2]);
}

int main(int argc, char** argv) {
	std::array<float,3> amin; 
	amin = opt::minimize(distance, opt::pattern_search(), std::array<float,3>{0.0f,0.0f,0.0f});
	std::cout<<"["<<amin[0]<<","<<amin[1]<<","<<amin[2]<<"] should be close to [3.25,-1.5,0]"<<std::endl;

	//We test with Beale function (minima in 3, 0.5) and tuples of the same datatype
	double xmin, ymin;	
	std::tie(xmin, ymin) = opt::minimize([] (double x, double y) { return sqr(1.5-x+x*y) + sqr(2.25-x+x*y*y) + sqr(2.625-x+x*y*y*y); }, opt::pattern_search(1000, 1.0, 1.e-8));
	std::cout<<"("<<xmin<<","<<ymin<<")"<<" should be close to (3,0.5)"<<std::endl;
}
