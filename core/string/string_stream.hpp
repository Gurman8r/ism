#ifndef _ISM_STRING_STREAM_HPP_
#define _ISM_STRING_STREAM_HPP_

#include <core/string/string.hpp>

#include <sstream>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// string stream base
	template <class Ch = char
	> ALIAS(_StringStreamBase) std::basic_stringstream<Ch, std::char_traits<Ch>, PolymorphicAllocator<Ch>>;

	// basic string stream
	template <class Ch = char
	> class BasicStringStream : public _StringStreamBase<Ch>
	{
	public:
		using base_type = _StringStreamBase<Ch>;
		using base_type::base_type;
		using base_type::operator=;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// string stream
	ALIAS(StringStream) BasicStringStream<char>;

	// wide string stream
	ALIAS(WideStringStream) BasicStringStream<wchar_t>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_STRING_STREAM_HPP_
