#include <iostream>

using T = short;

struct None {};
struct True {};
struct False {};


// main struct for numbers
template <T val> struct N {};



// std::all_of() for types
template <typename ... vals> struct All_ {};

// if current value is false, set False value
template <typename ... vals> struct All_<False, vals...> {
	using result = False;
};

// if current value is true, keep recersion
template <typename ... vals> struct All_<True, vals...> {
	using result = typename All_<vals...>::result;		
};

// base of recursion
template <> struct All_<False> { using result = False; };
template <> struct All_<True> { using result = True; };



// subarray [1, 2, ..., val] for algorithm 
template <typename ... vals> struct Arr_ {};

// rescursively filling of array
template <T val, T ... vals> struct Arr_<N<val>, N<vals>...> {
	using value = typename Arr_<
		typename std::conditional<
			(val > 1), N<val - 1>, None
		>::type,
		N<val>, N<vals>...
	>::value;
};

// base of recursion
template <T ... vals> struct Arr_<None, N<vals>...> {
	using value = Arr_<N<vals>...>;
};



// check of one single number
template <typename ... vals> struct isPrime {};

// calling All_ for subarray [1 divides n, 2 divides n, ..., n divides n]
template <T n, T ... vals> struct isPrime<N<n>, Arr_<N<vals>...> > {
	using result = typename All_<
		typename std::conditional<
			 n % vals != 0 || vals == n || vals == 1,
			 True, False
		>::type ...
	>::result;
};



// main struct
template <typename ... vals> struct Primes {};

// recursively check every number (quite dummy algorithm)
template <T val, T ... vals> struct Primes<N<val>, N<vals>...> {
	using append_t_ = typename std::conditional<
		std::is_same<
			typename isPrime<
				N<val>,
				typename Arr_<N<val>, N<vals>...>::value
			>::result, True
		>::value,
		N<val>, None
	>::type;
	using value = typename Primes<
		typename std::conditional<
			(val - 1 > 1),
			N<val - 1>, None
		>::type,
		append_t_, N<vals>...
	>::value;
};

// removing None from array
template <T val, T ... vals> struct Primes<N<val>, None, N<vals>...> {
	using value = typename Primes<N<val>, N<vals>...>::value;
};
// base of recursion
template <T ... vals> struct Primes<None, N<vals>...> {
	using value = Primes<N<vals>...>;
};


int main()
{
	// get primes from 2 to 100
	Primes<N<100>>::value A;

	// throwing an error to see result
	A.error();

	return 0;	
}
