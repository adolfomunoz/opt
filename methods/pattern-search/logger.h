#pragma once


namespace opt {
namespace pattern_search_logger {

class null {
public:
	template<typename XType, typename YType>
	void log(unsigned long iteration, float step, const XType& best, const YType& f_best) {}
};

class null {
public:
	template<typename XType, typename YType>
	void log(unsigned long iteration, float step, const XType& best, const YType& f_best) {}
};




template<typename OS>
class Stream {
	OS& os;
public:
	Stream(OS& os) : os(os) {}
	template<typename XType, typename YType>
	void log(unsigned long iteration, float step, const XType& best, const YType& f_best) {
		os << iteration << " ("<<step<<") - "<<f_best<<" - [";
		for (besti = std::begin(best); besti != std::end(best); ++besti) { os<<" "<<(*besti); }
		os<<" ]"<<std::endl;

	}
};

template<typename OS>
Stream<OS> stream(OS& os) { return Stream<OS>(os); }


}
}
