#ifndef _ISM_PREPROCESSOR_HPP_
#define _ISM_PREPROCESSOR_HPP_

#include <core/language_features.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// concat implementation
#define _IMPL_CAT(a, b) \
		a##b

// concatenate
#define CAT(a, b) \
		_IMPL_CAT(a, b)

// compare
#define CMP(lhs, rhs) \
		(((lhs) != (rhs)) ? (((lhs) < (rhs)) ? -1 : 1) : 0)

// min
#define MIN(lhs, rhs) \
		((lhs) < (rhs) ? (lhs) : (rhs))

// max
#define MAX(lhs, rhs) \
		((lhs) < (rhs) ? (rhs) : (lhs))

// expression to string
#define TOSTR(expr) \
		#expr

// evaluate to string
#define XSTR(expr) \
		TOSTR(expr)

// wide string
#define WIDE(str) \
		CAT(L, str)

// string variable
#define STRVAR(name, str) \
		static char const name[] = str

// automatic forward
#define FWD(expr) \
		(std::forward<decltype(expr)>(expr))

// fixed array size
#define ARRAY_SIZE(arr) \
		(sizeof(arr) / sizeof(*arr))

// static member variable helper
#define MEMBER_IMPL(expr) \
		decltype(expr) expr

// sentinal
#define SENTINAL \
		{}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// map enum to array of constant values
#define MAKE_ENUM_MAPPING(m_func, m_from, m_to, ...)				\
		static constexpr m_to _MAP_##m_from##_TO_##m_to##_[] =		\
		{															\
			##__VA_ARGS__											\
		};															\
		static constexpr m_to m_func(m_from i) noexcept	\
		{															\
			return _MAP_##m_from##_TO_##m_to##_[(size_t)i];			\
		}															\

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// below "a" is a power of 2:

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

// make anonymous
#if defined(__COUNTER__)
#	define MAKE_ANON(expr) CAT(_, CAT(expr, CAT(_, CAT(__COUNTER__, _))))
#elif defined(__LINE__)
#	define MAKE_ANON(expr) CAT(_, CAT(expr, CAT(_, CAT(__LINE__, _))))
#else
#	define MAKE_ANON(expr) CAT(_, CAT(expr, _))
#endif

// anonymous
#define ANON \
	MAKE_ANON(anonymous)

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// unused
#define UNUSED(expr) \
		((void)(expr))

// sink implementation
#define _IMPL_SINK(var, ...)		\
		int var[] = { ##__VA_ARGS__ };	\
		UNUSED(var);

// sink
#define SINK(...) \
		_IMPL_SINK(ANON, ##__VA_ARGS__)

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// return static implementation
#define _RETURN_STATIC(var, expr)	\
		static auto var = expr;			\
		return var;						\

// return static
#define RETURN_STATIC(expr) \
		_RETURN_STATIC(ANON, (expr))

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// scope enter
namespace ism::impl
{
	// invoke function in Constructor
	template <class Fn> struct OnScopeEnter final
	{
		OnScopeEnter(Fn fn) noexcept { fn(); }
	};

	enum class OnScopeEnterTag {};

	template <class Fn> auto operator+(OnScopeEnterTag, Fn fn) noexcept
	{
		return OnScopeEnter<Fn>{ fn };
	}
}

// scope enter ex
#define SCOPE_ENTER_EX(...) \
		(::ism::impl::OnScopeEnterTag{}) + [##__VA_ARGS__]() noexcept -> void

// scope enter
#define SCOPE_ENTER(...) \
		auto ANON = SCOPE_ENTER_EX(##__VA_ARGS__)

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// scope exit
namespace ism::impl
{
	// invoke function in destructor
	template <class Fn> struct OnScopeExit final
	{
		OnScopeExit(Fn fn) noexcept : m_fn{ fn } {}

		~OnScopeExit() noexcept { m_fn(); }

	private: Fn const m_fn;
	};

	enum class OnScopeExitTag {};

	template <class Fn> auto operator+(OnScopeExitTag, Fn fn) noexcept
	{
		return OnScopeExit<Fn>{ fn };
	}
}

// scope exit ex
#define SCOPE_EXIT_EX(...) \
		(::ism::impl::OnScopeExitTag{}) + [##__VA_ARGS__]() noexcept -> void

// scope exit
#define SCOPE_EXIT(...) \
		auto ANON = SCOPE_EXIT_EX(##__VA_ARGS__)

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// non-copyable
#define NON_COPYABLE(T)									\
		T(T const &) = delete;							\
		T & operator=(T const &) = delete;				\

// non-movable
#define NON_MOVABLE(T)									\
		T(T &&) noexcept = delete;						\
		T & operator=(T &&) noexcept = delete;			\

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ISM_PREPROCESSOR_HPP_
