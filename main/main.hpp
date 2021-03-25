#ifndef _ISM_MAIN_HPP_
#define _ISM_MAIN_HPP_

#include <core/os/os.hpp>

namespace ISM
{
	class ISM_API Main final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static uint32_t	g_frame_count;
		static uint32_t	g_frame_index;
		static int32_t	g_iterating;

	public:
		static Err setup(cstring exepath, int32_t argc, char * argv[]);

		static bool start();

		static bool iteration();

		static bool is_iterating() { return g_iterating > 0; }

		static void cleanup();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ISM_MAIN_HPP_
