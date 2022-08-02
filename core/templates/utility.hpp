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

	template <class F> struct OnScopeEnter final
	{
		explicit OnScopeEnter(F fn) noexcept { fn(); }
	};

	enum class OnScopeEnter_Tag {};

	template <class F
	> auto operator+(OnScopeEnter_Tag, F fn) noexcept
	{
		return OnScopeEnter<F>{ fn };
	}

#define ON_SCOPE_ENTER(...) \
		auto ANON(temp) = (ism::priv::OnScopeEnter_Tag{}) + [##__VA_ARGS__]() noexcept -> void

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// scope exit

	template <class F> struct OnScopeExit final
	{
		F const m_fn;

		explicit OnScopeExit(F fn) noexcept : m_fn{ fn } {}

		~OnScopeExit() noexcept { m_fn(); }
	};

	enum class OnScopeExit_Tag {};

	template <class F
	> auto operator+(OnScopeExit_Tag, F fn) noexcept
	{
		return OnScopeExit<F>{ fn };
	}

#define ON_SCOPE_EXIT(...) \
		auto ANON(temp) = (ism::priv::OnScopeExit_Tag{}) + [##__VA_ARGS__]() noexcept -> void

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

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T
	> constexpr void swap(T & lhs, T & rhs) noexcept
	{
		T tmp = std::move(lhs);
		lhs = std::move(rhs);
		rhs = std::move(tmp);
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

	template <class A, class B
	> constexpr i32 compare(A const & a, B const & b) noexcept
	{
		return CMP(a, b);
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

	template <class X, class In0, class In1, class Out0, class Out1
	> constexpr decltype(auto) map(X const x, In0 const in_min, In1 const in_max, Out0 const out_min, Out1 const out_max)
	{
		return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ISM_UTILITY_HPP_
