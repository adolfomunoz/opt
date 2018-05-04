#pragma once

#include <type_traits>
#include <tuple>
#include <array>

namespace detail {
	template<typename T, std::size_t N, typename... Args, std::size_t... I>
	constexpr std::array<T,N> tuple_to_array_aux(const std::tuple<Args...>& t, std::index_sequence<I...>) {
		return std::array<T,N>{ std::get<I>(t)... };
	}

	template<typename T, std::size_t N, std::size_t... I>
	constexpr auto array_to_tuple_aux(const std::array<T,N>& a, std::index_sequence<I...>) {
		return std::make_tuple(a[I]...);
	}
}

template<typename T, typename... Rest>
constexpr auto tuple_to_array(const std::tuple<T,Rest...>& t) {
	return detail::tuple_to_array_aux<T, 1 + sizeof...(Rest)>(t, std::index_sequence_for<T, Rest...>{});
}

template<typename T, std::size_t N>
constexpr auto array_to_tuple(const std::array<T,N>& a) {
	return detail::array_to_tuple_aux(a, std::make_index_sequence<N>{});
}

