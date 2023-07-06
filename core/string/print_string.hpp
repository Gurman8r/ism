#ifndef _ISM_PRINT_STRING_HPP_
#define _ISM_PRINT_STRING_HPP_

#include <core/string/string.hpp>

#define PRINTV			(Ism::printv)
#define PRINTF			(Ism::printf)

#define ERR_PRINTV		(Ism::err_printv)
#define ERR_PRINTF		(Ism::err_printf)

#define PRINT			(Ism::print)
#define PRINT_LINE		(Ism::print_line)
#define PRINT_INFO		(Ism::print_info)
#define PRINT_WARNING	(Ism::print_warning)
#define PRINT_ERROR		(Ism::print_error)

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ISM_API_FUNC(void) printv(cstring fmt, va_list args);

	ISM_API_FUNC(void) printf(cstring fmt, ...);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ISM_API_FUNC(void) err_printv(cstring fmt, va_list args);
	
	ISM_API_FUNC(void) err_printf(cstring fmt, ...);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ISM_API_FUNC(void) print(String const & s);
	
	ISM_API_FUNC(void) print_line(String const & s);
	
	ISM_API_FUNC(void) print_info(String const & s);
	
	ISM_API_FUNC(void) print_warning(String const & s);
	
	ISM_API_FUNC(void) print_error(String const & s);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum ErrorHandlerType_ {
		ErrorHandlerType_Error,
		ErrorHandlerType_Warning,
		ErrorHandlerType_MAX
	};

	ISM_API_FUNC(void) _err_print_error(cstring func, cstring file, u32 line, cstring desc, ErrorHandlerType_ log_type = ErrorHandlerType_Error);
	ISM_API_FUNC(void) _err_print_error(cstring func, cstring file, u32 line, cstring desc, cstring m_message, ErrorHandlerType_ log_type = ErrorHandlerType_Error);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_PRINT_STRING_HPP_
