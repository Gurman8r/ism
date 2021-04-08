#ifndef _ISM_ERROR_MACROS_HPP_
#define _ISM_ERROR_MACROS_HPP_

#include <core/error/error.hpp>

#include <cassert>
#include <stdexcept>

namespace ISM
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class BuiltinException : public std::runtime_error {
	public:
		using std::runtime_error::runtime_error;
		virtual void set_error() const = 0;
	};

#define ISM_RUNTIME_EXCEPTION(name, type) \
	class name : public BuiltinException { public: \
		using BuiltinException::BuiltinException; \
		name() : name{ "" } { } \
		void set_error() const override { _err_set_string(type, what()); } \
	};

	ISM_API_FUNC(void *) _err_occurred();
	ISM_API_FUNC(void) _err_set_string(void * exception, cstring message);
	ISM_API_FUNC(void) _err_format(void * exception, cstring message, ...);
	ISM_API_FUNC(void) _err_fetch(void ** type, void ** value, void ** trace);
	ISM_API_FUNC(void) _err_restore(void * type, void * value, void * trace);

	struct ErrorScope final
	{
		void * type, * value, * trace;
		ErrorScope() { ISM::_err_fetch(&type, &value, &trace); }
		~ErrorScope() { ISM::_err_restore(type, value, trace); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum ErrorHandlerType
	{
		ERR_HANDLER_ERROR,
		ERR_HANDLER_WARNING,
	};

	ISM_API_FUNC(void) _err_print_error(cstring func, cstring file, uint32_t line, cstring desc, ErrorHandlerType log_type = ERR_HANDLER_ERROR);
	ISM_API_FUNC(void) _err_print_error(cstring func, cstring file, uint32_t line, cstring desc, cstring message, ErrorHandlerType log_type = ERR_HANDLER_ERROR);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// generate trap
#if defined(ISM_CC_MSVC)
#	define DEBUG_TRAP()	_CSTD __debugbreak()
#elif defined(ISM_CC_CLANG)
#	define DEBUG_TRAP()	_CSTD __builtin_debugtrap()
#else
#	define DEBUG_TRAP()	_CSTD raise(SIGTRAP)
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ISM
{
	inline void _ism_wassert(cwstring message, cwstring file, uint32_t line) noexcept
	{
#if defined(ISM_CC_MSVC)
		_CSTD _wassert(message, file, line);

#elif defined(assert)
		assert(message);

#else
#		error "wassert is unavailable"
#endif
	}

// wide assert
#define WIDE_ASSERT(message, file, line) \
	(ISM::_ism_wassert)(message, file, line)
}

// assert
#define ASSERT(message, file, line) \
	WIDE_ASSERT(WIDE(message), WIDE(file), (unsigned)line)

// fatal
#define FATAL(message) \
	ASSERT(message, __FILE__, __LINE__)

// verify
#define VERIFY(expr) \
	((void)((!!(expr)) || (FATAL(TOSTR(expr)), 0)))

namespace ISM
{
	template <class T
	> auto _ism_check(T && expr, cwstring message, cwstring file, uint32_t line) noexcept -> decltype(FWD(expr))
	{
		return ((void)((!!(expr)) || (WIDE_ASSERT(message, file, line), 0))), FWD(expr);
	}

// check
#define CHECK(expr) \
	(ISM::_ism_check)(expr, WIDE(TOSTR(expr)), WIDE(__FILE__), (unsigned)__LINE__)
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// debug
#if ISM_IS_DEBUG
#define DEBUG_FATAL(message)	FATAL(message)
#define DEBUG_VERIFY(expr)		VERIFY(expr)
#define DEBUG_CHECK(expr)		CHECK(expr)
#else
#define DEBUG_FATAL(message)	UNUSED(0)
#define DEBUG_VERIFY(expr)		UNUSED(0)
#define DEBUG_CHECK(expr)		UNUSED(0)
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define ERR_PRINT_ERROR(desc) \
	ISM::_err_print_error(__FUNCTION__, __FILE__, __LINE__, (desc), ISM::ERR_HANDLER_ERROR)

#define ERR_PRINT_ERROR_MSG(desc, message) \
	ISM::_err_print_error(__FUNCTION__, __FILE__, __LINE__, (desc), (message), ISM::ERR_HANDLER_ERROR)

#define ERR_PRINT_WARNING(desc) \
	ISM::_err_print_error(__FUNCTION__, __FILE__, __LINE__, (desc), ISM::ERR_HANDLER_WARNING)

#define ERR_PRINT_WARNING_MSG(desc, message) \
	ISM::_err_print_error(__FUNCTION__, __FILE__, __LINE__, (desc), (message), ISM::ERR_HANDLER_ERROR)


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ERROR_MACROS_HPP_
