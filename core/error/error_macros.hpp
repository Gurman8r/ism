#ifndef _ISM_ERROR_MACROS_HPP_
#define _ISM_ERROR_MACROS_HPP_

#include <core/error/error.hpp>
#include <core/string/print_string.hpp>
#include <cassert>
#include <stdexcept>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace priv
	{
		// crash implementation
		ISM_API_FUNC(void) _crash(cwstring message, cwstring file, u32 line);
	}

	// crash
#define CRASH(m_message) \
		(Ism::priv::_crash)(WIDE(m_message), WIDE(__FILE__), __LINE__)

	// crashw
#define CRASHW(m_message) \
		(Ism::priv::_crash)(m_message, WIDE(__FILE__), __LINE__)

	// assert
#define ASSERT(m_expr) \
		BRANCHLESS_IF(!(m_expr), CRASH(TOSTR(m_expr)))

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace priv
	{
		// validate implementation
		template <class T
		> auto _validate(T && expr, cwstring message, cwstring file, u32 line) noexcept -> decltype(FWD(expr))
		{
			return BRANCHLESS_IF(!(expr), _crash(message, file, line)), FWD(expr);
		}
	}

	// validate
#define VALIDATE(m_expr) \
		(Ism::priv::_validate)((m_expr), WIDE(TOSTR(m_expr)), WIDE(__FILE__), __LINE__)

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// verify
#define VERIFY(m_expr, m_message)												\
		do {																	\
			if (m_expr) { /* contextually convertible to bool paranoia */ }		\
			else {																\
				Ism::priv::_crash(WIDE(m_message), WIDE(__FILE__), __LINE__);	\
			}																	\
		} while (0)

	// verify range
#define VERIFY_RANGE(m_expr, m_min, m_max) \
		(UNUSED((!!(( \
		static_cast<decltype(m_expr)>(m_min) < (m_expr)) && \
		((m_expr) < static_cast<decltype(m_expr)>(m_max)))) || \
		(CRASH("range error: \'" TOSTR(m_min) "\' < \'" TOSTR(m_expr) "\' < \'" TOSTR(m_max) "\'"), 0) \
		))

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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

#endif // !_ERROR_MACROS_HPP_
