#pragma once

#include <type_traits>
#include <callable/callable.hpp>

template<typename F>
class callable_tuple {	
private:
	template<std::size_t... I>
	static constexpr auto type_aux(std::index_sequence<I...>) {
		return std::tuple<typename callable_traits<F>::template argument_type<I>...>();
	}

public:	
	using type = decltype(type_aux(std::make_index_sequence<callable_traits<F>::argc>{}));	
};
