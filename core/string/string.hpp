#ifndef _ISM_STRING_HPP_
#define _ISM_STRING_HPP_

#include <core/os/memory.hpp>

#include <string>

namespace ism::util
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Ch = char
	> constexpr size_t strlen(Ch const * s)
	{
		return *s ? (1 + strlen(s + 1)) : 0;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// string base
	template <class Ch = char
	> ALIAS(_StringBase) std::basic_string<Ch, std::char_traits<Ch>, PolymorphicAllocator<Ch>>;

	// basic string
	template <class Ch = char
	> ALIAS(BasicString) _StringBase<Ch>;

	// string
	ALIAS(String) BasicString<char>;

	// wide string
	ALIAS(WideString) BasicString<wchar_t>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	inline String operator "" _s(cstring s, size_t n) noexcept { return String{ s, n }; }

	inline WideString operator "" _s(cwstring s, size_t n) noexcept { return WideString{ s, n }; }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Ch> struct Hasher<BasicString<Ch>>
	{
		hash_t operator()(BasicString<Ch> const & s) const noexcept { return hash(s.data(), s.size()); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_STRING_HPP_
