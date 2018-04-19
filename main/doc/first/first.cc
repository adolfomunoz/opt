#include "../../../opt.h"

float error_sqr11(float x) { return (121.0f - x*x)*(121.0f - x*x); }

class CallableSqrError {
	float t;
public:
	CallableSqrError(float t) : t(t) { }
	float operator()(float x) const { return (t-x*x)*(t-x*x); }
};

int main(int argc, char** argv) {
	std::cout<<opt::minimize(error_sqr11)<<std::endl;
	std::cout<<opt::minimize(CallableSqrError(121))<<std::endl;
	std::cout<<opt::minimize([=] (float x) { return (121.0f - x*x)*(121.0f - x*x); })<<std::endl;
}
