#pragma once
#include <iostream>

class null_ostream : public std::basic_ostream<char> { };

template<typename T>
constexpr null_ostream& operator<<(null_ostream& os, const T& t)
{	return os;	}
