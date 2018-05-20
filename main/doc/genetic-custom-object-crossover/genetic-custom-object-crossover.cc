#include "../../../opt.h"
#include <cmath>
#include <iostream>

float sqr(float f) { return f*f; }
class Point {
public:
	float x, y;
	Point(float x, float y) : x(x), y(y) { }
	float distance_to_circle(const Point& center, float radius) const {
		return fabs(sqrt(sqr(center.x - x) + sqr(center.y - y)) - radius);
	}
	template<typename RNG> //Random point in the square between both points
	Point cross(const Point& that, RNG& random) const {
		std::uniform_real_distribution<float> sample_x(x,that.x);
		std::uniform_real_distribution<float> sample_y(y,that.y);
		return Point(sample_x(random), sample_y(random));
	}
};

int main(int argc, char** argv) {
	Point p = opt::minimize(
		// Function to minimize (distance to 3 circles)
		[] (const Point& p) { return p.distance_to_circle(Point(0,0),1) + p.distance_to_circle(Point(0,0.5),1) + p.distance_to_circle(Point(2,0),3); },
		// Genetic method with 100 iterations, 10 population, 10 mutations and 5 crossovers per iteration
		opt::genetic(),
		// Initial population, because "Point" does not have a default constructor
		std::array<Point,5>{Point(0,0),Point(10,10), Point(10,-10), Point(-10,-10), Point(-10,10)}
	);
	
	std::cout<<p.x<<", "<<p.y<<std::endl;
}
