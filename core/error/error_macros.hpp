#ifndef _ISM_ERROR_MACROS_HPP_
#define _ISM_ERROR_MACROS_HPP_

#include <core/error/error.hpp>

#include <cassert>
#include <stdexcept>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ISM_API_FUNC(void) _crash(cwstring message, cwstring file, u32 line);

	// crash
#define CRASH(message) \
		(ism::_crash)(WIDE(message), WIDE(__FILE__), __LINE__)

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// assert
#define ASSERT(expr) \
		(UNUSED((!!(expr)) || (CRASH(TOSTR(expr)), 0)))

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// verify
#define VERIFY(expr, message)												\
		do {																\
			if (expr) { /* contextually convertible to bool paranoia */ }	\
			else {															\
				ism::_crash(WIDE(message), WIDE(__FILE__), __LINE__);		\
			}																\
		} while (0)															\

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T
	> auto _validate(T && expr, cwstring message, cwstring file, u32 line) noexcept -> decltype(FWD(expr))
	{
		return (UNUSED((!!(expr)) || (_crash(message, file, line), 0))), FWD(expr);
	}

	// validate
#define VALIDATE(expr) \
		(ism::_validate)((expr), WIDE(TOSTR(expr)), WIDE(__FILE__), __LINE__)

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum ErrorHandlerType_
	{
		ErrorHandlerType_Error,
		ErrorHandlerType_Warning,
		ErrorHandlerType_MAX
	};

	ISM_API_FUNC(void) _err_print_error(cstring func, cstring file, u32 line, cstring desc, ErrorHandlerType_ log_type = ErrorHandlerType_Error);
	ISM_API_FUNC(void) _err_print_error(cstring func, cstring file, u32 line, cstring desc, cstring message, ErrorHandlerType_ log_type = ErrorHandlerType_Error);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define ERR_PRINT_ERROR(desc) \
		(ism::_err_print_error(__PRETTY_FUNCTION__, __FILE__, __LINE__, (desc), ism::ErrorHandlerType_Error))

#define ERR_PRINT_ERROR_MSG(desc, message) \
		(ism::_err_print_error(__PRETTY_FUNCTION__, __FILE__, __LINE__, (desc), (message), ism::ErrorHandlerType_Error))

#define ERR_PRINT_WARNING(desc) \
		(ism::_err_print_error(__PRETTY_FUNCTION__, __FILE__, __LINE__, (desc), ism::ErrorHandlerType_Warning))

#define ERR_PRINT_WARNING_MSG(desc, message) \
		(ism::_err_print_error(__PRETTY_FUNCTION__, __FILE__, __LINE__, (desc), (message), ism::ErrorHandlerType_Error))

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ERROR_MACROS_HPP_
