#ifndef _ISM_UTILITY_HPP_
#define _ISM_UTILITY_HPP_

#include <core/os/copymem.hpp>
#include <core/templates/type_traits.hpp>

#include <stdarg.h>
#include <algorithm>
#include <functional>
#include <utility>

#include <gcem.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// "a" is a power of 2:

// round down size "n" to be a multiple of "a"
#define SIZE_ROUND_DOWN(num, alignment) \
		((size_t)(num) & ~(size_t)((alignment) - 1))

// round up size "num" to be alignment multiple of "alignment"
#define SIZE_ROUND_UP(num, alignment) \
		(((size_t)(num) + (size_t)((alignment) - 1)) & ~(size_t)((alignment) - 1))

// round pointer "ptr" down to the closest "alignment"-aligned address <= "ptr"
#define ALIGN_DOWN(ptr, alignment) \
		((void *)((uintptr_t)(ptr) & ~(uintptr_t)((alignment) - 1)))

// round pointer "ptr" up to the closest "alignment"-aligned address >= "ptr"
#define ALIGN_UP(ptr, alignment) \
		((void *)(((uintptr_t)(ptr) + (uintptr_t)((alignment) - 1)) & ~(uintptr_t)((alignment) - 1)))

// check if pointer "ptr" is aligned to "alignment"-bytes boundary
#define IS_ALIGNED(ptr, alignment) \
		(!((uintptr_t)(ptr) & (uintptr_t)((alignment) - 1)))

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// map enum to array of constant values
#define MAKE_ENUM_MAPPING(m_func, m_from, m_to, ...)			\
		static constexpr m_to _MAP_##m_from##_TO_##m_to##_[] =	\
		{														\
			##__VA_ARGS__										\
		};														\
		static constexpr m_to m_func(m_from i) noexcept			\
		{														\
			return _MAP_##m_from##_TO_##m_to##_[(size_t)i];		\
		}														\

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// fancy macros
namespace ism::priv
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// scope enter

	template <class F> struct _OnScopeEnter final
	{
		explicit _OnScopeEnter(F fn) noexcept { fn(); }
	};

	enum class _OnScopeEnter_Tag {};

	template <class F
	> auto operator+(_OnScopeEnter_Tag, F fn) noexcept
	{
		return _OnScopeEnter<F>{ fn };
	}

#define ON_SCOPE_ENTER(...) \
		auto ANON = (ism::priv::_OnScopeEnter_Tag{}) + [##__VA_ARGS__]() noexcept -> void

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// scope exit

	template <class F> struct _OnScopeExit final
	{
		F const m_fn;

		explicit _OnScopeExit(F fn) noexcept : m_fn{ fn } {}

		~_OnScopeExit() noexcept { m_fn(); }
	};

	enum class _OnScopeExit_Tag {};

	template <class F
	> auto operator+(_OnScopeExit_Tag, F fn) noexcept
	{
		return _OnScopeExit<F>{ fn };
	}

#define ON_SCOPE_EXIT(...) \
		auto ANON = (ism::priv::_OnScopeExit_Tag{}) + [##__VA_ARGS__]() noexcept -> void

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// defs
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> struct Plus
	{
		using result_type = typename T;
		using first_argument_type = typename T;
		using second_argument_type = typename T;
		constexpr T operator()(T const & lhs, T const & rhs) const { return lhs + rhs; }
	};

	template <class T> struct Minus
	{
		using result_type = typename T;
		using first_argument_type = typename T;
		using second_argument_type = typename T;
		constexpr T operator()(T const & lhs, T const & rhs) const { return lhs - rhs; }
	};

	template <class T> struct Multiplies
	{
		using result_type = typename T;
		using first_argument_type = typename T;
		using second_argument_type = typename T;
		constexpr T operator()(T const & lhs, T const & rhs) const { return lhs * rhs; }
	};

	template <class T> struct EqualTo
	{
		using result_type = typename bool;
		using first_argument_type = typename T;
		using second_argument_type = typename T;
		constexpr bool operator()(T const & lhs, T const & rhs) const { return lhs == rhs; }
	};

	template <class T> struct NotEqualTo
	{
		using result_type = typename bool;
		using first_argument_type = typename T;
		using second_argument_type = typename T;
		constexpr bool operator()(T const & lhs, T const & rhs) const { return lhs != rhs; }
	};

	template <class T> struct Less
	{
		using result_type = typename bool;
		using first_argument_type = typename T;
		using second_argument_type = typename T;
		constexpr bool operator()(T const & lhs, T const & rhs) const { return lhs < rhs; }
	};

	template <class T> struct Greater
	{
		using result_type = typename bool;
		using first_argument_type = typename T;
		using second_argument_type = typename T;
		constexpr bool operator()(T const & lhs, T const & rhs) const { return lhs > rhs; }
	};

	template <class T> struct LessEqual
	{
		using result_type = typename bool;
		using first_argument_type = typename T;
		using second_argument_type = typename T;
		constexpr bool operator()(T const & lhs, T const & rhs) const { return lhs <= rhs; }
	};

	template <class T> struct GreaterEqual
	{
		using result_type = typename bool;
		using first_argument_type = typename T;
		using second_argument_type = typename T;
		constexpr bool operator()(T const & lhs, T const & rhs) const { return lhs >= rhs; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// pair
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// pair
	template <class First, class Second
	> ALIAS(Pair) std::pair<First, Second>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// utility
namespace ism::util
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class To, class From, class = std::enable_if_t<mpl::is_trivially_convertible_v<To, From>>
	> To bit_cast(From const & value) noexcept
	{
		To temp;
		copymem(&temp, &value, sizeof(To));
		return temp;
	}

	template <class A, class B
	> constexpr int32_t compare(A const & a, B const & b) noexcept
	{
		return CMP(a, b);
	}

	template <class T
	> constexpr void swap(T & lhs, T & rhs) noexcept
	{
		T tmp = std::move(lhs);
		lhs = std::move(rhs);
		rhs = std::move(tmp);
	}

	template <class LI, class RI
	> constexpr bool range_equal(LI lBegin, LI lEnd, RI rBegin, RI rEnd)
	{
		return (lBegin != lEnd && rBegin != rEnd)
			? ((*lBegin == *rBegin) && util::range_equal(lBegin + 1, lEnd, rBegin + 1, rEnd))
			: (lBegin == lEnd && rBegin == rEnd);
	}

	template <class LI, class RI
	> constexpr bool range_less(LI lBegin, LI lEnd, RI rBegin, RI rEnd)
	{
		return (lBegin != lEnd && rBegin != rEnd)
			? ((*lBegin < *rBegin) && util::range_less(lBegin + 1, lEnd, rBegin + 1, rEnd))
			: (lBegin == lEnd && rBegin == rEnd);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ISM_UTILITY_HPP_
