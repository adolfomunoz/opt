#include "../../../opt.h"

int main(int argc, char** argv) {
	float minx, miny;
	std::tie(minx,miny) = opt::minimize([=] (float x, float y) { return std::sqrt((x-1)*(x-1) + (y-2)*(y-2)); });
	std::cout<<"Minima are "<<minx<<" and "<<miny<<std::endl;
}
