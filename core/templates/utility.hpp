#ifndef _ISM_UTILITY_HPP_
#define _ISM_UTILITY_HPP_

#include <core/templates/type_traits.hpp>
#include <core/os/copymem.hpp>

#include <stdarg.h>
#include <algorithm>
#include <functional>
#include <initializer_list>
#include <iterator>
#include <utility>

#include <gcem/include/gcem.hpp>

namespace ism::util
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, class ... Ts
	> constexpr bool is_any_of_v
	{
		std::disjunction_v<std::is_same<T, Ts>...>
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class To, class From
	> constexpr bool is_trivially_convertible_v
	{
		"requires To is trivially default constructible and is copy or move constructible"
		&& sizeof(To) == sizeof(From)
		&& std::is_trivially_copyable_v<From>
		&& std::is_trivial_v<To>
		&& (std::is_copy_constructible_v<To> || std::is_move_constructible_v<To>)
	};

	template <class To, class From, class = std::enable_if_t<is_trivially_convertible_v<To, From>>
	> NODISCARD To bit_cast(From const & value) noexcept
	{
		To temp;
		copymem(&temp, &value, sizeof(To));
		return temp;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T
	> constexpr void swap(T & lhs, T & rhs) noexcept
	{
		T tmp = std::move(lhs);
		lhs = std::move(rhs);
		rhs = std::move(tmp);
	}

	template <class A, class B
	> constexpr int32_t compare(A const & a, B const & b) noexcept
	{
		return CMP(a, b);
	}

	template <class LI, class RI
	> NODISCARD constexpr bool range_equal(LI lBegin, LI lEnd, RI rBegin, RI rEnd)
	{
		return (lBegin != lEnd && rBegin != rEnd)
			? ((*lBegin == *rBegin) && util::range_equal(lBegin + 1, lEnd, rBegin + 1, rEnd))
			: (lBegin == lEnd && rBegin == rEnd);
	}

	template <class LI, class RI
	> NODISCARD constexpr bool range_less(LI lBegin, LI lEnd, RI rBegin, RI rEnd)
	{
		return (lBegin != lEnd && rBegin != rEnd)
			? ((*lBegin < *rBegin) && util::range_less(lBegin + 1, lEnd, rBegin + 1, rEnd))
			: (lBegin == lEnd && rBegin == rEnd);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_UTILITY_HPP_
