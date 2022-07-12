#include <core/error/error_macros.hpp>
#include <core/os/os.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void ism::_crash(cwstring message, cwstring file, u32 line)
{
#if CC_MSVC
	_CSTD _wassert(message, file, line);
#else

#endif
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void ism::_err_print_error(cstring func, cstring file, u32 line, cstring desc, ErrorHandlerType_ log_type)
{
	OS::get_singleton()->printerr(func, file, line, desc, "", log_type);
}

void ism::_err_print_error(cstring func, cstring file, u32 line, cstring desc, cstring message, ErrorHandlerType_ log_type)
{
	OS::get_singleton()->printerr(func, file, line, desc, message, log_type);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */