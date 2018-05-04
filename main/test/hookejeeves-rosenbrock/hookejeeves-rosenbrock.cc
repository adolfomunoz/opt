#include "../../../opt.h"
#include "../../../utils/null-ostream.h"
#include "../../../utils/test-functions.h"
#include <iostream>
#include <cstring>
#include <chrono>

std::ostream& operator<<(std::ostream& os, const std::array<float,2>& a) {
	os<<" <"<<a[0]<<" , "<<a[1]<<"> ";
	return os;
}

template<typename Method>
void test_method(const char* name, const testfunction::rosenbrock& f, const Method& method) {
	auto start = std::chrono::system_clock::now();
	std::array<float,2> sol = method.minimize(
			std::array<float, 2>{0.0f,0.0f},
			f, 
			std::cout); 
	auto stop = std::chrono::system_clock::now();
	std::chrono::duration<float> duration = stop - start;
	std::cout<<std::setw(20)<<name<<"\t| Time = "<<std::setw(10)<<std::setprecision(3)<<duration.count()<<" sec.\t| Result = "
						<<sol<<"\t| Value at result = "<<f(sol)<<"\t| Error = "<< f.error(sol)<<std::endl;
}


int main(int argc, char** argv) {
	float a = 1.0; 
	float b = 100.0;
	unsigned int iters = 1000;
	float        step       = 1.0f;
	float        epsilon    = 0.0001f;

	for (int i = 0; i<argc-1; ++i) {
		if (strcmp("-a", argv[i])==0)      a = atof(argv[++i]);
		else if (strcmp("-b", argv[i])==0) b = atof(argv[++i]);
		else if (strcmp("-iterations", argv[i])==0) iters = atoi(argv[++i]);
		else if (strcmp("-step", argv[i])==0)       step = atof(argv[++i]);
		else if (strcmp("-epsilon", argv[i])==0)    epsilon = atof(argv[++i]);
		
	}
	
	testfunction::rosenbrock f(a,b);
	
	test_method("HookeJeeves", f, opt::HookeJeeves(iters, step, epsilon));
}
