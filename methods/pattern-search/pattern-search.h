#pragma once

#include "../../utils/concepts.h"
#include <iostream>
#include <type_traits>
#include <array>

namespace opt {


class PatternSearch
{
private:
	unsigned int   iters_;			// number of iterations
	float          step_size_;		// Starting step size
	float          epsilon_;        // Minimum step size
			

public:
	PatternSearch(unsigned int iters    = 1000,
		float step_size               = 1.0f,
		float epsilon                 = 1.e-3f) :
		iters_(iters), 
		step_size_(step_size),
		epsilon_(epsilon) {}
			
	template<typename XType, typename FTarget, typename OS,
			typename YType = decltype(std::declval<FTarget>()(std::declval<XType>()))>
    		requires std::is_floating_point_v<YType> &&
	                 Container<XType> &&
	                 TargetFunction<FTarget, XType, YType> 
	XType minimize(const XType& ini, const FTarget& f, OS& os) const {
		XType best   = ini;
		YType f_best = f(best);
		decltype(std::begin(best)) besti;
		XType            x;
		YType          f_x;
		decltype(std::begin(x)) xi;

		unsigned int  i = 1;
		
		//Now the algorithm works with the precision of the input data
		typename std::remove_reference<decltype(*xi)>::type h   = step_size_; 
		typename std::remove_reference<decltype(*xi)>::type eps = epsilon_; 

		while ((eps < h) && (i <= iters_)) {
			os << i << " ("<<h<<") - "<<f_best<<" - [";
			for (besti = std::begin(best); besti != std::end(best); ++besti) { os<<" "<<(*besti); }
			os<<" ]"<<std::endl;
			//First we explore the best "neighbour".
			//This could be done in a more efficient way by avoiding the copy of XTypes and
			//just using the iterators, but it is easier and cleaner this way.
			bool has_changed = false;
			x = best;
			for (xi = std::begin(x); xi != std::end(x); ++xi)
			{
				auto xi_value = (*xi);
				(*xi) = xi_value + h;
				f_x = f(x);
				if (f_x < f_best) { best = x; f_best = f_x; has_changed = true; }
				
				(*xi) = xi_value - h;
				f_x = f(x);
				if (f_x < f_best) { best = x; f_best = f_x; has_changed = true; }
			
				(*xi) = xi_value;	
			}	

			if (has_changed) ++i; 
			else             h*=0.5f;
		}

		return best;
	}
};

PatternSearch pattern_search(unsigned int iters = 1000, float step_size = 1.0f, float epsilon = 1.e-6f) {
	return PatternSearch(iters, step_size, epsilon);
}


} // namespace opt


#include "minimize.h"
