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
	((lhs) > (rhs) ? (lhs) : (rhs))

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

// static member variable helper
#define VAR_IMPL(expr) \
	decltype(expr) expr

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

// compose
namespace ism::impl
{
	// compose helper
	template <class T> struct ComposerHelper final
	{
		T value;

		constexpr ComposerHelper(T && value) noexcept : value{ FWD(value) } {}

		template <class Fn = void(*)(T &)
		> constexpr decltype(auto) operator+(Fn && fn) && noexcept
		{
			return fn(value), std::move(value);
		}
	};

#define COMPOSE_EX(m_class, ...) \
	(ism::impl::ComposerHelper<m_class>(m_class{ ##__VA_ARGS__ }))

#define COMPOSE(m_class, m_var, ...) \
	COMPOSE_EX(m_class, ##__VA_ARGS__) + [&](m_class & m_var) -> void
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// return static implementation
#define __RETURN_STATIC(var, expr)	\
	static auto var = expr;				\
	return var;							\

// return static
#define RETURN_STATIC(expr) \
	__RETURN_STATIC(ANON, expr)

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
	(ism::impl::OnScopeEnterTag{}) + [##__VA_ARGS__]() noexcept -> void

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
	(ism::impl::OnScopeExitTag{}) + [##__VA_ARGS__]() noexcept -> void

// scope exit
#define SCOPE_EXIT(...) \
	auto ANON = SCOPE_EXIT_EX(##__VA_ARGS__)

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// size round down
#define SIZE_ROUND_DOWN(n, a) \
	((size_t)(n) & ~(size_t)((a) - 1))

// size round up
#define SIZE_ROUND_UP(n, a) \
	(((size_t)(n) + (size_t)((a) - 1)) & ~(size_t)((a) - 1))

// align down
#define ALIGN_DOWN(p, a) \
	((void *)((uintptr_t)(p) & ~(uintptr_t)((a) - 1)))

// align up
#define ALIGN_UP(p, a) \
	((void *)(((uintptr_t)(p) + (uintptr_t)((a) - 1)) & ~(uintptr_t)((a) - 1)))

// is aligned
#define IS_ALIGNED(p, a) \
	(!((uintptr_t)(p) & (uintptr_t)((a) - 1)))

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// non-copyable
#define NON_COPYABLE(T)									\
	T(T const &) = delete;								\
	T & operator=(T const &) = delete;					\

// non-movable
#define NON_MOVABLE(T)									\
	T(T &&) noexcept = delete;							\
	T & operator=(T &&) noexcept = delete;				\

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// default constructible
#define DEFAULT(T)										\
	T() noexcept = default;								\

// copy constructible
#define COPYABLE(T)										\
	T(T const &) = default;								\
	T & operator=(T const &) = default;					\

// move constructible
#define MOVABLE(T)										\
	T(T &&) noexcept = default;							\
	T & operator=(T &&) noexcept = default;				\

// copy move constructible
#define COPYABLE_MOVABLE(T)								\
	COPYABLE(T)											\
	MOVABLE(T)											\

// default copy move constructible
#define DEFAULT_COPYABLE_MOVABLE(T)						\
	DEFAULT(T)											\
	COPYABLE(T)											\
	MOVABLE(T)											\

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// constexpr default constructible
#define CONSTEXPR_DEFAULT(T)							\
	constexpr T() noexcept = default;					\

// constexpr copy constructible
#define CONSTEXPR_COPYABLE(T)							\
	constexpr T(T const &) = default;					\
	constexpr T & operator=(T const &) = default;		\

// constexpr move constructible
#define CONSTEXPR_MOVABLE(T)							\
	constexpr T(T &&) noexcept = default;				\
	constexpr T & operator=(T &&) noexcept = default;	\

// constexpr copy move constructible
#define CONSTEXPR_COPYABLE_MOVABLE(T)					\
	CONSTEXPR_COPYABLE(T)								\
	CONSTEXPR_MOVABLE(T)								\

// constexpr default copy move constructible
#define CONSTEXPR_DEFAULT_COPYABLE_MOVABLE(T)			\
	CONSTEXPR_DEFAULT(T)								\
	CONSTEXPR_COPYABLE(T)								\
	CONSTEXPR_MOVABLE(T)								\

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ISM_PREPROCESSOR_HPP_
