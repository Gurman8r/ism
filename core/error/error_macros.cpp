#include <core/error/error_macros.hpp>
#include <core/os/os.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Ism::_crash(cwstring message, cwstring file, u32 line)
	{
#if CC_MSVC
		_CSTD _wassert(message, file, line);
#else
#error "crash implementation not found"
#endif
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}