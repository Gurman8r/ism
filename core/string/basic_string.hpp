#ifndef _ISM_STRING_HPP_
#define _ISM_STRING_HPP_

#include <core/os/memory.hpp>
#include <core/templates/hash.hpp>
#include <core/io/json.hpp>

#include <string>
#include <sstream>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if 1
	// basic string
	template <class Ch = char
	> ALIAS(BasicString) std::basic_string<Ch, std::char_traits<Ch>, PolymorphicAllocator<Ch>>;
#else
	// string base
	template <class Ch = char
	> ALIAS(_StringBase) std::basic_string<Ch, std::char_traits<Ch>, PolymorphicAllocator<Ch>>;

	// basic string
	template <class Ch = char
	> class BasicString : public _StringBase<Ch> {
	public:
		using base_type = _StringBase<Ch>;
		using base_type::base_type;
	};
#endif

	// string
	ALIAS(String) BasicString<char>;

	// wide string
	ALIAS(WideString) BasicString<wchar_t>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// basic string stream
	template <class Ch = char
	> ALIAS(BasicStringStream) std::basic_stringstream<Ch, std::char_traits<Ch>, PolymorphicAllocator<Ch>>;

	// string stream
	ALIAS(StringStream) BasicStringStream<char>;

	// wide string stream
	ALIAS(WideStringStream) BasicStringStream<wchar_t>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Ch = char
	> inline void to_json(Json & json, BasicString<Ch> const & value)
	{
		json = (std::pmr::basic_string<Ch, std::char_traits<Ch>> const &)value;
	}

	template <class Ch = char
	> inline void from_json(Json const & json, BasicString<Ch> & value)
	{
		json.get_to((std::pmr::basic_string<Ch, std::char_traits<Ch>> &)value);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_STRING_HPP_
