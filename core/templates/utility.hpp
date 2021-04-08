#ifndef _ISM_UTILITY_HPP_
#define _ISM_UTILITY_HPP_

#include <core/templates/type_traits.hpp>
#include <core/os/copymem.hpp>

#include <stdarg.h>
#include <algorithm>
#include <initializer_list>
#include <iterator>
#include <tuple>
#include <utility>

#include <gcem/include/gcem.hpp>

namespace ISM
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class To, class From, class = std::enable_if_t<is_trivially_convertible_v<To, From>>
	> NODISCARD To bit_cast(From const & value) noexcept
	{
		To temp{};
		copymem(&temp, &value, sizeof(To));
		return temp;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	template <class T, class SFINAE = void> struct is_copy_constructible : std::is_copy_constructible<T> {};

	template <class T> constexpr bool is_copy_constructible_v{ is_copy_constructible<T>::value };

	template <class Container> struct is_copy_constructible<Container, std::enable_if_t<all_of<
		std::is_copy_constructible<Container>,
		std::is_same<typename Container::value_type &, class Container::reference>,
		std::negation<std::is_same<Container, class Container::value_type>>
	>::value>> : is_copy_constructible<typename Container::value_type> {};

	template <class T1, class T2> struct is_copy_constructible<std::pair<T1, T2>>
		: all_of<is_copy_constructible<T1>, is_copy_constructible<T2>> {};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, class SFINAE = void> struct is_copy_assignable : std::is_copy_assignable<T> {};

	template <class Container> struct is_copy_assignable<Container, std::enable_if_t<all_of<
		std::is_copy_assignable<Container>,
		std::is_same<typename Container::value_type &, class Container::reference>
	>::value>> : is_copy_assignable<typename Container::value_type> {};

	template <class T1, class T2> struct is_copy_assignable<std::pair<T1, T2>>
		: all_of<is_copy_assignable<T1>, is_copy_assignable<T2>> {};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

namespace ISM::UTIL
{
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
			? ((*lBegin == *rBegin) && UTIL::range_equal(lBegin + 1, lEnd, rBegin + 1, rEnd))
			: (lBegin == lEnd && rBegin == rEnd);
	}

	template <class LI, class RI
	> NODISCARD constexpr bool range_less(LI lBegin, LI lEnd, RI rBegin, RI rEnd)
	{
		return (lBegin != lEnd && rBegin != rEnd)
			? ((*lBegin < *rBegin) && UTIL::range_less(lBegin + 1, lEnd, rBegin + 1, rEnd))
			: (lBegin == lEnd && rBegin == rEnd);
	}
}

namespace ISM
{
	template<class
	> struct is_ref_wrapper : std::false_type {};

	template <class T
	> struct is_ref_wrapper<std::reference_wrapper<T>> : std::true_type {};

	template <class T
	> using not_ref_wrapper = std::negation<is_ref_wrapper<std::decay_t<T>>>;

	template <class D, class...
	> struct return_type_helper { using type = D; };

	template <class... Args
	> struct return_type_helper<void, Args...> : std::common_type<Args...>
	{
		static_assert(
			std::conjunction_v<not_ref_wrapper<Args>...>,
			"Args cannot contain reference_wrappers when D is void");
	};

	template <class ... Types
	> using return_type_t = typename return_type_helper<Types...>::type;
}

#endif // !_ISM_UTILITY_HPP_
