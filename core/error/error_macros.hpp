#ifndef _ISM_ERROR_MACROS_HPP_
#define _ISM_ERROR_MACROS_HPP_

#include <core/error/error.hpp>

#include <cassert>
#include <stdexcept>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// generate trap
#if defined(COMPILER_MSVC)
#	define DEBUG_BREAK			(_CSTD __debugbreak)
#elif defined(COMPILER_CLANG)
#	define DEBUG_BREAK			(_CSTD __builtin_debugtrap)
#else
#	define DEBUG_BREAK()		(_CSTD raise(SIGTRAP))
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ism
{
	// wide assert implementation
	inline void _ism_wassert(cwstring message, cwstring file, uint32_t line) noexcept
	{
#if defined(COMPILER_MSVC)
		_CSTD _wassert(message, file, line);

#elif defined(assert)
		assert(message);

#else
#		error "wassert is unavailable"
#endif
	}
}

// wide assert
#define WASSERT(message, file, line) \
	(ism::_ism_wassert)(message, file, line)

// narrow assert
#define ASSERT(message, file, line) \
	WASSERT(WIDE(message), WIDE(file), (unsigned)line)

// fatal
#define FATAL(message) \
	ASSERT(message, __FILE__, __LINE__)

// verify
#define VERIFY(expr) \
	((void)((!!(expr)) || (FATAL(TOSTR(expr)), 0)))

namespace ism
{
	// validate implementation
	template <class T
	> auto _ism_validate(T && expr, cwstring message, cwstring file, uint32_t line) noexcept -> decltype(FWD(expr))
	{
		return ((void)((!!(expr)) || (WASSERT(message, file, line), 0))), FWD(expr);
	}
}

// validate message
#define VALIDATE_MSG(expr, msg) \
	(ism::_ism_validate)(expr, WIDE(msg), WIDE(__FILE__), (unsigned)__LINE__)

// validate
#define VALIDATE(expr) \
	(ism::_ism_validate)(expr, WIDE(TOSTR(expr)), WIDE(__FILE__), (unsigned)__LINE__)

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ism
{
	enum ErrorHandlerType
	{
		ERR_HANDLER_ERROR,
		ERR_HANDLER_WARNING,
	};

	ISM_API_FUNC(void) _err_print_error(cstring func, cstring file, uint32_t line, cstring desc, ErrorHandlerType log_type = ERR_HANDLER_ERROR);
	ISM_API_FUNC(void) _err_print_error(cstring func, cstring file, uint32_t line, cstring desc, cstring message, ErrorHandlerType log_type = ERR_HANDLER_ERROR);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define ERR_PRINT_ERROR(desc) \
	ism::_err_print_error(__FUNCTION__, __FILE__, __LINE__, (desc), ism::ERR_HANDLER_ERROR)

#define ERR_PRINT_ERROR_MSG(desc, message) \
	ism::_err_print_error(__FUNCTION__, __FILE__, __LINE__, (desc), (message), ism::ERR_HANDLER_ERROR)

#define ERR_PRINT_WARNING(desc) \
	ism::_err_print_error(__FUNCTION__, __FILE__, __LINE__, (desc), ism::ERR_HANDLER_WARNING)

#define ERR_PRINT_WARNING_MSG(desc, message) \
	ism::_err_print_error(__FUNCTION__, __FILE__, __LINE__, (desc), (message), ism::ERR_HANDLER_ERROR)


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ERROR_MACROS_HPP_
