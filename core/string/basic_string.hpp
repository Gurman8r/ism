#ifndef _ISM_STRING_HPP_
#define _ISM_STRING_HPP_

#include <core/templates/hash.hpp>
#include <core/os/memory.hpp>

#include <string>
#include <sstream>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// basic string
	template <class Ch = char
	> ALIAS(BasicString) typename std::basic_string<Ch, std::char_traits<Ch>, PolymorphicAllocator<Ch>>;

	// string
	ALIAS(String) BasicString<char>;

	// wide string
	ALIAS(WideString) BasicString<wchar_t>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// basic string stream
	template <class Ch = char
	> ALIAS(BasicStringStream) typename std::basic_stringstream<Ch, std::char_traits<Ch>, PolymorphicAllocator<Ch>>;

	// string stream
	ALIAS(StringStream) BasicStringStream<char>;

	// wide string stream
	ALIAS(WideStringStream) BasicStringStream<wchar_t>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_STRING_HPP_
