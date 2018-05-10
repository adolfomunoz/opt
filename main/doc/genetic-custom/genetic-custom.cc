#include "../../../opt.h"
#include <cmath>
#include <iostream>


class Ponger {
	int value_;
public:
	Ponger(int value = 1) : value_(value) { }
	void ping() { ++value_; if (value_ == 0) ++value_; }
	void pong() { --value_; if (value_ == 0) --value_; }
	int value() const { return value_; }
};

int main(int argc, char** argv) {
	Ponger p1, p2, p3;

	//Lambda function for mutating Pongers
	auto mutpon = [] (const Ponger& p, auto& random) {
		std::uniform_int_distribution<int> choice(0,1);
		Ponger s = p;
		switch (choice(random)) {
			case 0: s.ping(); break;
			case 1: s.pong(); break;
		}
		return s;
	};
	
	std::tie(p1, p2, p3) = opt::minimize(
		// Function to minimize with 3 pongers (will return a tuple)
		[] (const Ponger& a, const Ponger& b, const Ponger& c) {
			return std::abs(float( (a.value() + b.value() + c.value()) - 
					(a.value() * b.value() * c.value() ) ) );
		},
		// Genetic method with 100 iterations, 10 population, 10 mutations and 5 crossovers per iteration
		opt::genetic(100,10,10,5),
		// Each mutation: mutate one ponger, either ping or pong.
		// The same mutation strategy for the three function parameters
		opt::mutation::single_parameter(mutpon, mutpon, mutpon)
	);
	
	std::cout<<p1.value()<<" - "<<p2.value()<<" - "<<p3.value()<<std::endl;
}
