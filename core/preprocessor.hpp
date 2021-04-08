#ifndef _ISM_PREPROCESSOR_HPP_
#define _ISM_PREPROCESSOR_HPP_

#include <core/language_features.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define _ISM				::ISM::

// concat implementation
#define IMPL_CONCAT(a, b)	a##b

// concatenate
#define CAT(a, b)			IMPL_CONCAT(a, b)

// ternary comparison
#define CMP(lhs, rhs)		(((lhs) != (rhs)) ? (((lhs) < (rhs)) ? -1 : 1) : 0)

// min
#define MIN(lhs, rhs)		((lhs) < (rhs) ? (lhs) : (rhs))

// max
#define MAX(lhs, rhs)		((lhs) > (rhs) ? (lhs) : (rhs))

// unused expression
#define UNUSED(expr)		((void)(expr))

// expression to string
#define TOSTR(expr)			#expr

// evaluate to string
#define XSTR(expr)			TOSTR(expr)

// crt wide string
#define WIDE(str)			CAT(L, str)

// string variable
#define STRVAR(name, str)	static char const name[] = { str }

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define SIZE_ROUND_DOWN(n, a)	((size_t)(n) & ~(size_t)((a) - 1))
#define SIZE_ROUND_UP(n, a)		(((size_t)(n) + (size_t)((a) - 1)) & ~(size_t)((a) - 1))
#define ALIGN_DOWN(p, a)		((void *)((uintptr_t)(p) & ~(uintptr_t)((a) - 1)))
#define ALIGN_UP(p, a)			((void *)(((uintptr_t)(p) + (uintptr_t)((a) - 1)) & ~(uintptr_t)((a) - 1)))
#define IS_ALIGNED(p, a)		(!((uintptr_t)(p) & (uintptr_t)((a) - 1)))

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// anonymous expression
#if defined(__COUNTER__)
#	define MAKE_ANON(expr)	CAT(_, CAT(expr, CAT(_, CAT(__COUNTER__, _))))
#elif defined(__LINE__)
#	define MAKE_ANON(expr)	CAT(_, CAT(expr, CAT(_, CAT(__LINE__, _))))
#else
#	define MAKE_ANON(expr)	CAT(_, CAT(expr, _))
#endif

// anonymous
#define ANONYMOUS			MAKE_ANON(anonymous)

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ISM::IMPL
{
	// compose
	template <class T> struct CompositionHelper {};
	template <class T, class Fn = void(*)(T &)
	> constexpr auto operator+(CompositionHelper<T>, Fn fn) { T x{}; fn((T &)x); return x; }
#define COMPOSE(T, v, ...) (ISM::IMPL::CompositionHelper<T>{}) + [##__VA_ARGS__](T & v) noexcept->void
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define IMPL_RETURN_STATIC(name, expr) static auto name{ expr }; return name

#define RETURN_STATIC(expr) IMPL_RETURN_STATIC(ANONYMOUS, expr)

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// SCOPE_ENTER
namespace ISM::IMPL
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
#define SCOPE_ENTER_EX(...) (ISM::IMPL::OnScopeEnterTag{}) + [##__VA_ARGS__]() noexcept

// scope enter
#define SCOPE_ENTER(...) auto ANONYMOUS = SCOPE_ENTER_EX(##__VA_ARGS__)

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// SCOPE_EXIT
namespace ISM::IMPL
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
#define SCOPE_EXIT_EX(...) (ISM::IMPL::OnScopeExitTag{}) + [##__VA_ARGS__]() noexcept

// scope exit
#define SCOPE_EXIT(...) auto ANONYMOUS = SCOPE_EXIT_EX(##__VA_ARGS__)

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define DEFAULT_COPY_AND_MOVE_CONSTRUCTABLE(type) \
public: \
	type() noexcept = default; \
	type(type const &) = default; \
	type & operator=(type const &) = default; \
	type(type &&) noexcept = default; \
	type & operator=(type &&) noexcept = default;

#define CONSTEXPR_DEFAULT_COPY_AND_MOVE_CONSTRUCTABLE(type) \
public: \
	constexpr type() noexcept = default; \
	constexpr type(type const &) = default; \
	constexpr type(type &&) noexcept = default; \
	constexpr type & operator=(type const &) = default; \
	constexpr type & operator=(type &&) noexcept = default;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ISM_PREPROCESSOR_HPP_
