#include <core/string/print_string.hpp>
#include <core/os/os.hpp>

using namespace ism;

void ism::print_line()
{
	OS::get_singleton()->print("\n");
}

void ism::print_line(String const & s)
{
	OS::get_singleton()->print("%s\n", s.c_str());
}

void ism::print_error(String const & s)
{
	OS::get_singleton()->printerr("%s\n", s.c_str());
}