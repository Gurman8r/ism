#ifndef _ISM_UTILITY_HPP_
#define _ISM_UTILITY_HPP_

#include <core/os/copymem.hpp>
#include <core/templates/type_traits.hpp>

#include <stdarg.h>
#include <algorithm>
#include <functional>
#include <utility>

#include <gcem.hpp>

namespace ism
{
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
}

namespace ism::util
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T = f32, class = std::enable_if_t<std::is_floating_point_v<T>>
	> constexpr T pi_v{ static_cast<T>(3.14159265358979323846) };

	template <class T = f32, class = std::enable_if_t<mpl::is_number_v<T>>
	> constexpr auto radians(T const value) noexcept
	{
		if constexpr (std::is_floating_point_v<T>)
		{
			return value * static_cast<T>(0.01745329251994329576923690768489);
		}
		else
		{
			return radians(static_cast<f32>(value));
		}
	}

	template <class T = f32, class = std::enable_if_t<mpl::is_number_v<T>>
	> constexpr auto degrees(T const value) noexcept
	{
		if constexpr (std::is_floating_point_v<T>)
		{
			return value * static_cast<T>(57.295779513082320876798154814105);
		}
		else
		{
			return degrees(static_cast<f32>(value));
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class To, class From, class = std::enable_if_t<mpl::is_trivially_convertible_v<To, From>>
	> To bit_cast(From const & value) noexcept
	{
		To temp;
		copymem(&temp, &value, sizeof(To));
		return temp;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T
	> constexpr void swap(T & a, T & b) noexcept
	{
		T c{ std::move(a) };
		a = std::move(b);
		b = std::move(c);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class A, class B
	> constexpr i32 compare(A const & a, B const & b) noexcept {
		return CMP(a, b);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Left, class Right, class ... Rest
	> constexpr decltype(auto) min(Left && lhs, Right && rhs, Rest && ... rest) noexcept
	{
		return lhs < rhs ? min(FWD(lhs), FWD(rest)...) : min(FWD(rhs), FWD(rest)...);
	}

	template <class Only
	> constexpr decltype(auto) min(Only && only) noexcept
	{
		return FWD(only);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Left, class Right, class ... Rest
	> constexpr decltype(auto) max(Left && lhs, Right && rhs, Rest && ... rest) noexcept
	{
		return lhs > rhs ? max(FWD(lhs), FWD(rest)...) : max(FWD(rhs), FWD(rest)...);
	}

	template <class Only
	> constexpr decltype(auto) max(Only && only) noexcept
	{
		return FWD(only);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// range equal
	template <class LI, class RI
	> constexpr bool range_equal(LI lBegin, LI lEnd, RI rBegin, RI rEnd) {
		return (lBegin != lEnd && rBegin != rEnd)
			? ((*lBegin == *rBegin) && util::range_equal(lBegin + 1, lEnd, rBegin + 1, rEnd))
			: (lBegin == lEnd && rBegin == rEnd);
	}

	// range not equal
	template <class LI, class RI
	> constexpr bool range_nequal(LI lBegin, LI lEnd, RI rBegin, RI rEnd) {
		return (lBegin != lEnd && rBegin != rEnd)
			? ((*lBegin != *rBegin) && util::range_nequal(lBegin + 1, lEnd, rBegin + 1, rEnd))
			: (lBegin == lEnd && rBegin == rEnd);
	}

	// range less
	template <class LI, class RI
	> constexpr bool range_less(LI lBegin, LI lEnd, RI rBegin, RI rEnd) {
		return (lBegin != lEnd && rBegin != rEnd)
			? ((*lBegin < *rBegin) && util::range_less(lBegin + 1, lEnd, rBegin + 1, rEnd))
			: (lBegin == lEnd && rBegin == rEnd);
	}

	// range greater
	template <class LI, class RI
	> constexpr bool range_greater(LI lBegin, LI lEnd, RI rBegin, RI rEnd) {
		return (lBegin != lEnd && rBegin != rEnd)
			? ((*lBegin > *rBegin) && util::range_greater(lBegin + 1, lEnd, rBegin + 1, rEnd))
			: (lBegin == lEnd && rBegin == rEnd);
	}

	// range less or equal
	template <class LI, class RI
	> constexpr bool range_lequal(LI lBegin, LI lEnd, RI rBegin, RI rEnd) {
		return (lBegin != lEnd && rBegin != rEnd)
			? ((*lBegin <= *rBegin) && util::range_lequal(lBegin + 1, lEnd, rBegin + 1, rEnd))
			: (lBegin == lEnd && rBegin == rEnd);
	}

	// range greater or equal
	template <class LI, class RI
	> constexpr bool range_gequal(LI lBegin, LI lEnd, RI rBegin, RI rEnd) {
		return (lBegin != lEnd && rBegin != rEnd)
			? ((*lBegin >= *rBegin) && util::range_gequal(lBegin + 1, lEnd, rBegin + 1, rEnd))
			: (lBegin == lEnd && rBegin == rEnd);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ISM_UTILITY_HPP_
