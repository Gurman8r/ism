#ifndef _ISM_PREPROCESSOR_HPP_
#define _ISM_PREPROCESSOR_HPP_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// concat implementation
#define _IMPL_CAT(a, b) \
		a##b

// concat
#define CAT(a, b) \
		_IMPL_CAT(a, b)

// compare
#define CMP(lhs, rhs) \
		(((lhs) != (rhs)) ? (((lhs) < (rhs)) ? -1 : 1) : 0)

// minimum
#define MIN(lhs, rhs) \
		((lhs) < (rhs) ? (lhs) : (rhs))

// maximum
#define MAX(lhs, rhs) \
		((lhs) < (rhs) ? (rhs) : (lhs))

// token literal to string
#define TOSTR(expr) \
		#expr

// macro contents to string
#define XSTR(expr) \
		TOSTR(expr)

// wide string
#define WIDE(str) \
		CAT(L, str)

// fixed array size
#define ARRAY_SIZE(arr) \
		(sizeof(arr) / sizeof(*arr))

// unused
#define UNUSED(expr) \
		((void)(expr))

// static string variable
#define STRVAR(name, str) \
		static char const name[] = str

// static member variable helper
#define MEMBER_IMPL(expr) \
		decltype(expr) expr

// sentinal
#define SENTINAL \
		{}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// non-copyable
#define NON_COPYABLE(T)							\
		T(T const &) = delete;					\
		T & operator=(T const &) = delete;		\

// non-movable
#define NON_MOVABLE(T)							\
		T(T &&) noexcept = delete;				\
		T & operator=(T &&) noexcept = delete;	\

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

// sink implementation
#define _IMPL_SINK(var, ...)			\
		int var[] = { ##__VA_ARGS__ };	\
		UNUSED(var);

// sink
#define SINK(...) \
		_IMPL_SINK(ANON, ##__VA_ARGS__)

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ISM_PREPROCESSOR_HPP_
