#include <core/error/error_macros.hpp>
#include <core/os/os.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void ism::_err_print_error(cstring func, cstring file, uint32_t line, cstring desc, ErrorHandlerType log_type)
{
	SINGLETON(OS)->print_error(func, file, line, desc, "", (Logger::Error)log_type);
}

void ism::_err_print_error(cstring func, cstring file, uint32_t line, cstring desc, cstring message, ErrorHandlerType log_type)
{
	SINGLETON(OS)->print_error(func, file, line, desc, message, (Logger::Error)log_type);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */