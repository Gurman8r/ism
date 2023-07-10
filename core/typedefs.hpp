#ifndef _ISM_TYPEDEFS_HPP_
#define _ISM_TYPEDEFS_HPP_

#include <core/language_features.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifdef CC_MSVC
	using i8 =	signed __int8;
	using i16 =	signed __int16;
	using i32 =	signed __int32;
	using i64 =	signed __int64;
	using u8 =	unsigned __int8;
	using u16 =	unsigned __int16;
	using u32 =	unsigned __int32;
	using u64 =	unsigned __int64;
#else
	using i8 =	signed char;
	using i16 =	signed short;
	using i32 =	signed int;
	using i64 =	signed long long;
	using u8 =	unsigned char;
	using u16 =	unsigned short;
	using u32 =	unsigned int;
	using u64 =	unsigned long long;
#endif

	using f32 =	float;
	using f64 =	double;
	using f80 =	long double;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if (ARCHITECTURE == 64)
	using ssize_t =		i64;
	using size_t =		u64;
#else
	using ssize_t =		i32;
	using size_t =		u32;
#endif

	using intptr_t =	ssize_t;
	using uintptr_t =	size_t;
	using ptrdiff_t =	ssize_t;
	using nullptr_t =	decltype(nullptr);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	using cstring =		char const *;
	using cwstring =	wchar_t const *;
#if (HAS_CXX_20)
	using c8string =	char8_t const *;
#else
	using c8string =	char const *;
#endif
	using c16string =	char16_t const *;
	using c32string =	char32_t const *;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_TYPEDEFS_HPP_
