#ifndef _ISM_MAIN_HPP_
#define _ISM_MAIN_HPP_

#include <core/os/os.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API Main final
	{
		static i32 m_iterating;
		static u64 m_frame;

	public:
		static Error_ setup(cstring exepath, i32 argc, char * argv[]);

		static bool start();

		static bool iteration();

		static bool is_iterating() { return m_iterating > 0; }

		static void cleanup();
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_MAIN_HPP_
