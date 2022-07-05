#include <core/error/error_macros.hpp>
#include <core/os/os.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void ism::_crash(cwstring message, cwstring file, uint32_t line)
{
#if CC_MSVC
	_CSTD _wassert(message, file, line);
#else

#endif
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void ism::_err_print_error(cstring func, cstring file, uint32_t line, cstring desc, ErrorHandlerType_ log_type)
{
	SYSTEM->printerr(func, file, line, desc, "", log_type);
}

void ism::_err_print_error(cstring func, cstring file, uint32_t line, cstring desc, cstring message, ErrorHandlerType_ log_type)
{
	SYSTEM->printerr(func, file, line, desc, message, log_type);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */