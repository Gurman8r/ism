#ifndef _ISM_PREPROCESSOR_HPP_
#define _ISM_PREPROCESSOR_HPP_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// concat implementation
#define _IMPL_CAT(a, b) \
		a##b

// concat
#define CAT(a, b) \
		_IMPL_CAT(a, b)

// token literal to string
#define TOSTR(expr) \
		#expr

// macro contents to string
#define XSTR(expr) \
		TOSTR(expr)

// wide string
#define WIDE(str) \
		CAT(L, str)

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ISM_PREPROCESSOR_HPP_
