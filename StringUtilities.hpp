#pragma once

#include <string>
#include <sstream>

using namespace std;

template <typename T>
struct remove_ref_and_const
{
    typedef T type;
};

template <typename T>
struct remove_ref_and_const< T const & >
{
    typedef T type;
};

template <typename T>
struct remove_ref_and_const< T & >
{
    typedef T type;
};

template <typename T>
struct remove_ref_and_const< T const >
{
    typedef T type;
};

template < typename T >
typename remove_ref_and_const<T>::type fromString( string const& str )
{
	stringstream ss;
	ss << str;
    // needed in case the user passes in a
    // reference type as T
    auto t = typename remove_ref_and_const<T>::type ();
	ss >> t;
	return t;
}

template < typename T >
string toString( T t )
{
	stringstream ss;
	ss.precision(15);
	ss << t;
	return ss.str();
}

