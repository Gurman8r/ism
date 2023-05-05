#ifndef _ISM_PRINT_STRING_HPP_
#define _ISM_PRINT_STRING_HPP_

#include <core/string/string.hpp>

#define PRINT_LINE (Ism::print_line)

#define PRINT_ERROR (Ism::print_error)

namespace Ism
{
	ISM_API_FUNC(void) print_line(String const & s);

	ISM_API_FUNC(void) print_error(String const & s);
}

#endif // !_ISM_PRINT_STRING_HPP_
