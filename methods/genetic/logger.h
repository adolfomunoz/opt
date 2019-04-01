#pragma once


namespace opt {
namespace genetic_logger {

class null {
public:
	template<typename Iterator>
	void log(unsigned long iteration, const Iterator& begin, const Iterator& end) {}
};



template<typename OS>
class Stream {
	OS& os;
public:
	Stream(OS& os) : os(os) {}
	template<typename Iterator>
	void log(unsigned long iteration, const Iterator& begin, const Iterator& end) {
		os<<"["<<std::setw(4)<<iteration<<"] -> ";
			std::for_each(begin, end, [this] (const auto& t) {
				this->os<<std::get<0>(t)<<" ("<<std::get<1>(t)<<") | "; });
		os<<std::endl;

	}
};

template<typename OS>
Stream<OS> stream(OS& os) { return Stream<OS>(os); }


}
}
