#include <core/error/error_macros.hpp>
#include <core/os/os.hpp>

using namespace Ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void Ism::priv::_crash(cwstring message, cwstring file, u32 line)
{
#if CC_MSVC
	_CSTD _wassert(message, file, line);
#else
#error "crash implementation not found"
#endif
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */