#ifndef _ISM_MAIN_HPP_
#define _ISM_MAIN_HPP_

#include <core/os/os.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API Main final
	{
		static i32 m_iterating;
		static u64 m_frame;

	public:
		static void print_help();
		static bool is_cmdline_tool();

	public:
		static Error_ setup(cstring exec_path, i32 argc, char * argv[]);
		static bool start();
		static bool iteration();
		static bool is_iterating() { return m_iterating > 0; }
		static void cleanup(bool force = false);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_MAIN_HPP_
