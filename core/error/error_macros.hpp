#ifndef _ISM_ERROR_MACROS_HPP_
#define _ISM_ERROR_MACROS_HPP_

#include <core/error/error.hpp>
#include <core/templates/type_info.hpp>

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
	inline void _wide_assert(cwstring message, cwstring file, uint32_t line) noexcept
	{
#if defined(COMPILER_MSVC)
		_wassert(message, file, line);

#elif defined(assert)
		assert(message);

#else
#		error "wassert is unavailable"
#endif
	}

	// crash
#define CRASH(message) \
	(ism::_wide_assert)(WIDE(message), WIDE(__FILE__), (unsigned)__LINE__)

	// assert
#define ASSERT(expr) \
	((void)((!!(expr)) || (CRASH(TOSTR(expr)), 0)))

	// validate implementation
	template <class T
	> auto _wide_validate(T && expr, cwstring message, cwstring file, uint32_t line) noexcept -> decltype(FWD(expr))
	{
		return ((void)((!!(expr)) || (_wide_assert(message, file, line), 0))), FWD(expr);
	}

	// validate
#define VALIDATE(expr) \
	(ism::_wide_validate)(expr, WIDE(TOSTR(expr)), WIDE(__FILE__), __LINE__)
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ism
{
	enum ErrorHandlerType_
	{
		ErrorHandlerType_Error,
		ErrorHandlerType_Warning,
		ErrorHandlerType_MAX
	};

	ISM_API_FUNC(void) _err_print_error(cstring func, cstring file, uint32_t line, cstring desc, ErrorHandlerType_ log_type = ErrorHandlerType_Error);
	ISM_API_FUNC(void) _err_print_error(cstring func, cstring file, uint32_t line, cstring desc, cstring message, ErrorHandlerType_ log_type = ErrorHandlerType_Error);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define ERR_PRINT_ERROR(desc) \
	ism::_err_print_error(PRETTY_FUNCTION, __FILE__, __LINE__, (desc), ism::ErrorHandlerType_Error)

#define ERR_PRINT_ERROR_MSG(desc, message) \
	ism::_err_print_error(PRETTY_FUNCTION, __FILE__, __LINE__, (desc), (message), ism::ErrorHandlerType_Error)

#define ERR_PRINT_WARNING(desc) \
	ism::_err_print_error(PRETTY_FUNCTION, __FILE__, __LINE__, (desc), ism::ErrorHandlerType_Warning)

#define ERR_PRINT_WARNING_MSG(desc, message) \
	ism::_err_print_error(PRETTY_FUNCTION, __FILE__, __LINE__, (desc), (message), ism::ErrorHandlerType_Error)


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ERROR_MACROS_HPP_
