#ifndef _ISM_PRINT_STRING_HPP_
#define _ISM_PRINT_STRING_HPP_

#include <core/string/string.hpp>

#define PRINTV			(Ism::_printv)
#define PRINTF			(Ism::_printf)

#define ERR_PRINTV		(Ism::_err_printv)
#define ERR_PRINTF		(Ism::_err_printf)

#define PRINT			(Ism::_print)
#define PRINT_LINE		(Ism::_print_line)
#define PRINT_INFO		(Ism::_print_info)
#define PRINT_WARNING	(Ism::_print_warning)
#define PRINT_ERROR		(Ism::_print_error)

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ISM_API_FUNC(void) _printv(cstring fmt, va_list args);

	ISM_API_FUNC(void) _printf(cstring fmt, ...);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ISM_API_FUNC(void) _err_printv(cstring fmt, va_list args);
	
	ISM_API_FUNC(void) _err_printf(cstring fmt, ...);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ISM_API_FUNC(void) _print(String const & s);
	
	ISM_API_FUNC(void) _print_line(String const & s);
	
	ISM_API_FUNC(void) _print_info(String const & s);
	
	ISM_API_FUNC(void) _print_warning(String const & s);
	
	ISM_API_FUNC(void) _print_error(String const & s);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum ErrorHandlerType_ {
		ErrorHandlerType_Error,
		ErrorHandlerType_Warning,
		ErrorHandlerType_MAX
	};

	ISM_API_FUNC(void) _err_print_error(cstring func, cstring file, u32 line, cstring desc, ErrorHandlerType_ log_type = ErrorHandlerType_Error);
	ISM_API_FUNC(void) _err_print_error(cstring func, cstring file, u32 line, cstring desc, cstring m_message, ErrorHandlerType_ log_type = ErrorHandlerType_Error);

#define ERR_PRINT_ERROR(m_desc) \
		(Ism::_err_print_error(__PRETTY_FUNCTION__, __FILE__, __LINE__, (m_desc), Ism::ErrorHandlerType_Error))

#define ERR_PRINT_ERROR_MSG(m_desc, m_message) \
		(Ism::_err_print_error(__PRETTY_FUNCTION__, __FILE__, __LINE__, (m_desc), (m_message), Ism::ErrorHandlerType_Error))

#define ERR_PRINT_WARNING(m_desc) \
		(Ism::_err_print_error(__PRETTY_FUNCTION__, __FILE__, __LINE__, (m_desc), Ism::ErrorHandlerType_Warning))

#define ERR_PRINT_WARNING_MSG(m_desc, m_message) \
		(Ism::_err_print_error(__PRETTY_FUNCTION__, __FILE__, __LINE__, (m_desc), (m_message), Ism::ErrorHandlerType_Error))

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_PRINT_STRING_HPP_
