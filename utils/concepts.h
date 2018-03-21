#pragma once

template <typename RNG>
concept bool UniformRandomBitGenerator = 
    requires { typename RNG::result_type; }  && 
    requires(RNG a) {
	    a.max();
	    a.min();
	    a();
    };

template <typename C>
concept bool Container = 
    requires { typename C::value_type; }  && 
    requires(C c) {
	    c.begin();
	    c.end();
    } &&
    requires(C c1, C c2) {
	    c1=c2;
    };

template <typename C>
concept bool RandomAccessContainer =
    Container<C> && 
    requires { typename C::size_type; }  && 
    requires(C c, typename C::size_type i) {
	    c[i];
		i = c.size();
    };
