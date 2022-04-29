#include <core/string/print_string.hpp>
#include <core/os/os.hpp>

using namespace ism;

void ism::print_line()
{
	OS->print("\n");
}

void ism::print_line(String const & s)
{
	OS->print("%s\n", s.c_str());
}

void ism::print_error(String const & s)
{
	OS->printerr("%s\n", s.c_str());
}