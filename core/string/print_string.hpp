#ifndef _ISM_PRINT_STRING_HPP_
#define _ISM_PRINT_STRING_HPP_

#include <core/string/string_utility.hpp>

#define PRINT_LINE (ism::print_line)

#define PRINT_ERROR (ism::print_error)

namespace ism
{
	ISM_API_FUNC(void) print_line(String const & s);

	ISM_API_FUNC(void) print_error(String const & s);
}

#endif // !_ISM_PRINT_STRING_HPP_
