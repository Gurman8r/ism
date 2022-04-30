#include <core/string/print_string.hpp>
#include <core/os/os.hpp>

using namespace ism;

void ism::print_line()
{
	SYS->print("\n");
}

void ism::print_line(String const & s)
{
	SYS->print("%s\n", s.c_str());
}

void ism::print_error(String const & s)
{
	SYS->printerr("%s\n", s.c_str());
}