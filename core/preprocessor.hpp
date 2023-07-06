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
#define CMP(a, b) \
		(((a) != (b)) ? (((a) < (b)) ? -1 : 1) : 0)

// minimum
#define MIN(a, b) \
		((a) < (b) ? (a) : (b))

// maximum
#define MAX(a, b) \
		((a) < (b) ? (b) : (a))

// token literal to string
#define TOSTR(expr) \
		#expr

// macro contents to string
#define XSTR(expr) \
		TOSTR(expr)

// wide string
#define WIDE(str) \
		CAT(L, str)

// size of static array
#define ARRAY_SIZE(arr) \
		(sizeof(arr) / sizeof(*arr))

// unused expression
#define UNUSED(expr) \
		((void)(expr))

// branchless if statement
#define BRANCHLESS_IF(m_condition, m_expr) \
		(UNUSED((!(m_condition)) || ((m_expr), 0)))

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// anonymous variable name
#if defined(__COUNTER__)
#	define ANON(m_expr) CAT(_, CAT(m_expr, CAT(_, CAT(__COUNTER__, _))))
#elif defined(__LINE__)
#	define ANON(m_expr) CAT(_, CAT(m_expr, CAT(_, CAT(__LINE__, _))))
#else
#	define ANON(m_expr) CAT(_, CAT(m_expr, _))
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ISM_PREPROCESSOR_HPP_
