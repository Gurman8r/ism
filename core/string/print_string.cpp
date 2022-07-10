#include <core/string/print_string.hpp>
#include <core/os/os.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void ism::print_line(String const & s)
	{
		OS::get_singleton()->printf("%.*s\n", s.size(), s.data());
	}

	void ism::print_error(String const & s)
	{
		OS::get_singleton()->printerrf("%.*s\n", s.size(), s.data());
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}