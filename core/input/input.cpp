#include <core/input/input.hpp>

using namespace ism;

MEMBER_IMPL(Input::singleton) {};

OBJECT_EMBED(Input, t)
{
	t.tp_install = CLASS_INSTALLER(Input, t)
	{
		return t;
	};
}