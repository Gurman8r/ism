#include <core/error/error_macros.hpp>
#include <core/os/os.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void * ISM::_err_occurred()
{
	return nullptr;
}

void ISM::_err_set_string(void * exception, cstring message)
{
}

void ISM::_err_format(void * exception, cstring message, ...)
{
}

void ISM::_err_fetch(void ** type, void ** value, void ** trace)
{
}

void ISM::_err_restore(void * type, void * value, void * trace)
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void ISM::_err_print_error(cstring func, cstring file, uint32_t line, cstring desc, ErrorHandlerType log_type)
{
	get_os().print_error(func, file, line, desc, "", (Logger::Err)log_type);
}

void ISM::_err_print_error(cstring func, cstring file, uint32_t line, cstring desc, cstring message, ErrorHandlerType log_type)
{
	get_os().print_error(func, file, line, desc, message, (Logger::Err)log_type);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */