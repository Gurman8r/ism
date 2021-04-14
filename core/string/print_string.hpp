#ifndef _ISM_PRINT_STRING_HPP_
#define _ISM_PRINT_STRING_HPP_

#include <core/string/string_utility.hpp>

#define DEBUG_PRINT(fmt, ...) _STD printf(fmt, ##__VA_ARGS__)

#define PRINT_LINE(fmt, ...) ism::print_line(fmt, ##__VA_ARGS__)

#define PRINT_ERROR(fmt, ...) ism::print_error(fmt, ##__VA_ARGS__)

namespace ism
{
	ISM_API_FUNC(void) print_line(String const & s);

	ISM_API_FUNC(void) print_error(String const & s);
}

#endif // !_ISM_PRINT_STRING_HPP_
