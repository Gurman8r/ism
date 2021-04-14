#include <core/string/print_string.hpp>
#include <core/os/os.hpp>

namespace ism
{
	void print_line(String const & s)
	{
		get_os().print("%s\n", s.c_str());
	}

	void print_error(String const & s)
	{
		get_os().printerr("%s\n", s.c_str());
	}
}