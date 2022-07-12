#ifndef _ISM_MAIN_HPP_
#define _ISM_MAIN_HPP_

#include <core/os/os.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API Main final
	{
		static i32 g_iterating;

	public:
		static Error_ setup(cstring exepath, i32 argc, char * argv[]);

		static bool start();

		static bool iteration();

		static bool is_iterating() { return g_iterating > 0; }

		static void cleanup();
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_MAIN_HPP_
