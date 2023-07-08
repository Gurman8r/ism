#include <core/string/print_string.hpp>
#include <core/os/os.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Ism::_printv(cstring fmt, va_list args)
	{
		get_os()->printv(fmt, args);
	}

	void Ism::_printf(cstring fmt, ...)
	{
		va_list args;
		va_start(args, fmt);
		PRINTV(fmt, args);
		va_end(args);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Ism::_err_printv(cstring fmt, va_list args)
	{
		get_os()->err_printv(fmt, args);
	}

	void Ism::_err_printf(cstring fmt, ...)
	{
		va_list args;
		va_start(args, fmt);
		ERR_PRINTV(fmt, args);
		va_end(args);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Ism::_print(String const & s)
	{
		PRINTF("%.*s", s.size(), s.data());
	}

	void Ism::_print_line(String const & s)
	{
		PRINTF("%.*s\n", s.size(), s.data());
	}

	void Ism::_print_info(String const & s)
	{
		PRINTF("[INFO] %.*s\n", s.size(), s.data());
	}

	void Ism::_print_warning(String const & s)
	{
		PRINTF("[WARN] %.*s\n", s.size(), s.data());
	}

	void Ism::_print_error(String const & s)
	{
		ERR_PRINTF("[ERROR] %.*s\n", s.size(), s.data());
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Ism::_err_print_error(cstring func, cstring file, u32 line, cstring desc, ErrorHandlerType_ log_type)
	{
		get_os()->err_print(func, file, line, desc, "", log_type);
	}

	void Ism::_err_print_error(cstring func, cstring file, u32 line, cstring desc, cstring message, ErrorHandlerType_ log_type)
	{
		get_os()->err_print(func, file, line, desc, message, log_type);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}