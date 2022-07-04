#include <core/string/print_string.hpp>
#include <core/os/os.hpp>

using namespace ism;

void ism::print_line(String const & s)
{
	StringStream test{};
	test << "TEST";

	SYSTEM->printf("%.*s\n", s.size(), s.data());
}

void ism::print_error(String const & s)
{
	SYSTEM->errorf("%.*s\n", s.size(), s.data());
}