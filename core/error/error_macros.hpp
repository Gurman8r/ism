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
	// fatal error implementation (wide)
	inline void _ism_wfatal(cwstring message, cwstring file, uint32_t line) noexcept
	{
#if defined(COMPILER_MSVC)
		_wassert(message, file, line);

#elif defined(assert)
		assert(message);

#else
#		error "wassert is unavailable"
#endif
	}

	// fatal error (wide)
#	define WFATAL(message, file, line) \
	(ism::_ism_wfatal)(message, file, (uint32_t)line)

	// fatal error
#	define FATAL(message, file, line) \
	WFATAL(WIDE(message), WIDE(file), line)

	// crash (wide)
#	define WCRASH(message) \
	WFATAL(message, __FILE__, __LINE__)

	// crash
#	define CRASH(message) \
	FATAL(message, __FILE__, __LINE__)

	// assert
#	define ASSERT(expr) \
	((void)((!!(expr)) || (CRASH(TOSTR(expr)), 0)))

	// validate implementation (wide)
	template <class T
	> auto _ism_wvalidate(T && expr, cwstring message, cwstring file, uint32_t line) noexcept -> decltype(FWD(expr))
	{
		return ((void)((!!(expr)) || (WFATAL(message, file, line), 0))), FWD(expr);
	}

	// validate
#	define VALIDATE(expr) \
	(ism::_ism_wvalidate)(expr, WIDE(TOSTR(expr)), WIDE(__FILE__), __LINE__)
}

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
