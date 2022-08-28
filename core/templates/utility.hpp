#ifndef _ISM_UTILITY_HPP_
#define _ISM_UTILITY_HPP_

#include <core/os/copymem.hpp>
#include <core/math/math.hpp>

#include <stdarg.h>
#include <algorithm>
#include <functional>
#include <utility>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// bit_cast
	template <class To, class From, class = std::enable_if_t<
		sizeof(To) == sizeof(From) &&
		std::is_trivially_copyable_v<From> &&
		std::is_trivial_v<To> &&
		(std::is_copy_constructible_v<To> || std::is_move_constructible_v<To>)>
	> To bit_cast(From const & value) noexcept
	{
		To temp;
		copymem(&temp, &value, sizeof(To));
		return temp;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// plus
	template <class T> struct Plus
	{
		using result_type = typename T;
		using first_argument_type = typename T;
		using second_argument_type = typename T;
		constexpr T operator()(T const & lhs, T const & rhs) const { return lhs + rhs; }
	};

	// minus
	template <class T> struct Minus
	{
		using result_type = typename T;
		using first_argument_type = typename T;
		using second_argument_type = typename T;
		constexpr T operator()(T const & lhs, T const & rhs) const { return lhs - rhs; }
	};

	// multiplies
	template <class T> struct Multiplies
	{
		using result_type = typename T;
		using first_argument_type = typename T;
		using second_argument_type = typename T;
		constexpr T operator()(T const & lhs, T const & rhs) const { return lhs * rhs; }
	};

	// equal to
	template <class T> struct EqualTo
	{
		using result_type = typename bool;
		using first_argument_type = typename T;
		using second_argument_type = typename T;
		constexpr bool operator()(T const & lhs, T const & rhs) const { return lhs == rhs; }
	};

	// not equal to
	template <class T> struct NotEqualTo
	{
		using result_type = typename bool;
		using first_argument_type = typename T;
		using second_argument_type = typename T;
		constexpr bool operator()(T const & lhs, T const & rhs) const { return lhs != rhs; }
	};

	// less
	template <class T> struct Less
	{
		using result_type = typename bool;
		using first_argument_type = typename T;
		using second_argument_type = typename T;
		constexpr bool operator()(T const & lhs, T const & rhs) const { return lhs < rhs; }
	};

	// greater
	template <class T> struct Greater
	{
		using result_type = typename bool;
		using first_argument_type = typename T;
		using second_argument_type = typename T;
		constexpr bool operator()(T const & lhs, T const & rhs) const { return lhs > rhs; }
	};

	// less or equal
	template <class T> struct LessEqual
	{
		using result_type = typename bool;
		using first_argument_type = typename T;
		using second_argument_type = typename T;
		constexpr bool operator()(T const & lhs, T const & rhs) const { return lhs <= rhs; }
	};

	// greater or equal
	template <class T> struct GreaterEqual
	{
		using result_type = typename bool;
		using first_argument_type = typename T;
		using second_argument_type = typename T;
		constexpr bool operator()(T const & lhs, T const & rhs) const { return lhs >= rhs; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// pair
	template <class First, class Second
	> ALIAS(Pair) std::pair<First, Second>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class A, class B
	> constexpr auto compare(A const & a, B const & b) noexcept
	{
		return (a != b) ? ((a < b) ? -1 : 1) : 0;
	}

	template <class T
	> constexpr T const & constrain(T const & x, T const & min, T const & max) noexcept
	{
		return (x < min) ? min : (max < x) ? max : x;
	}

	template <class T
	> constexpr T map(T const x, T const in_min, T const in_max, T const out_min, T const out_max)
	{
		return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
	}

	namespace util
	{
		template <class T
		> constexpr void swap(T & a, T & b) noexcept
		{
			T c{ std::move(a) };
			a = std::move(b);
			b = std::move(c);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class First, class Second, class ... Rest
	> constexpr decltype(auto) minimum(First && first, Second && second, Rest && ... rest) noexcept
	{
		return first < second ? minimum(FWD(first), FWD(rest)...) : minimum(FWD(second), FWD(rest)...);
	}

	template <class Only
	> constexpr decltype(auto) minimum(Only && only) noexcept
	{
		return FWD(only);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class First, class Second, class ... Rest
	> constexpr decltype(auto) maximum(First && first, Second && second, Rest && ... rest) noexcept
	{
		return first > second ? maximum(FWD(first), FWD(rest)...) : maximum(FWD(second), FWD(rest)...);
	}

	template <class Only
	> constexpr decltype(auto) maximum(Only && only) noexcept
	{
		return FWD(only);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class It0, class It1
	> constexpr bool range_eq(It0 first0, It0 last0, It1 first1, It1 last1)
	{
		return (first0 != last0 && first1 != last1) ? ((*first0 == *first1) && range_eq(first0 + 1, last0, first1 + 1, last1)) : (first0 == last0 && first1 == last1);
	}

	template <class It0, class It1
	> constexpr bool range_neq(It0 first0, It0 last0, It1 first1, It1 last1)
	{
		return (first0 != last0 && first1 != last1) ? ((*first0 != *first1) && range_neq(first0 + 1, last0, first1 + 1, last1)) : (first0 == last0 && first1 == last1);
	}

	template <class It0, class It1
	> constexpr bool range_lt(It0 first0, It0 last0, It1 first1, It1 last1)
	{
		return (first0 != last0 && first1 != last1) ? ((*first0 < *first1) && range_lt(first0 + 1, last0, first1 + 1, last1)) : (first0 == last0 && first1 == last1);
	}

	template <class It0, class It1
	> constexpr bool range_gt(It0 first0, It0 last0, It1 first1, It1 last1)
	{
		return (first0 != last0 && first1 != last1) ? ((*first0 > *first1) && range_gt(first0 + 1, last0, first1 + 1, last1)) : (first0 == last0 && first1 == last1);
	}

	template <class It0, class It1
	> constexpr bool range_leq(It0 first0, It0 last0, It1 first1, It1 last1)
	{
		return (first0 != last0 && first1 != last1) ? ((*first0 <= *first1) && range_leq(first0 + 1, last0, first1 + 1, last1)) : (first0 == last0 && first1 == last1);
	}

	template <class It0, class It1
	> constexpr bool range_geq(It0 first0, It0 last0, It1 first1, It1 last1)
	{
		return (first0 != last0 && first1 != last1) ? ((*first0 >= *first1) && range_geq(first0 + 1, last0, first1 + 1, last1)) : (first0 == last0 && first1 == last1);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class It, class T
	> constexpr bool range_eq(It first, It last, T const & value)
	{
		return (first != last) ? ((*first == value) && range_eq(first + 1, last, value)) : (first == last);
	}

	template <class It, class T
	> constexpr bool range_neq(It first, It last, T const & value)
	{
		return (first != last) ? ((*first != value) && range_neq(first + 1, last, value)) : (first == last);
	}

	template <class It, class T
	> constexpr bool range_lt(It first, It last, T const & value)
	{
		return (first != last) ? ((*first < value) && range_lt(first + 1, last, value)) : (first == last);
	}

	template <class It, class T
	> constexpr bool range_gt(It first, It last, T const & value)
	{
		return (first != last) ? ((*first > value) && range_gt(first + 1, last, value)) : (first == last);
	}

	template <class It, class T
	> constexpr bool range_leq(It first, It last, T const & value)
	{
		return (first != last) ? ((*first <= value) && range_leq(first + 1, last, value)) : (first == last);
	}

	template <class It, class T
	> constexpr bool range_geq(It first, It last, T const & value)
	{
		return (first != last) ? ((*first >= value) && range_geq(first + 1, last, value)) : (first == last);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ISM_UTILITY_HPP_
