#ifndef _ISM_PRINT_STRING_HPP_
#define _ISM_PRINT_STRING_HPP_

#include <core/string/string_utility.hpp>

#define PRINT_LINE(s) ism::print_line(s)

#define PRINT_ERROR(s) ism::print_error(S)

namespace ism
{
	ISM_API_FUNC(void) print_line(String const & s);

	ISM_API_FUNC(void) print_error(String const & s);
}

#endif // !_ISM_PRINT_STRING_HPP_
