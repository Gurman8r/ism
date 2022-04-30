#include <core/input/input.hpp>

using namespace ism;

MEMBER_IMPL(Input::singleton) {};

OBJECT_EMBED(Input, t)
{
	t.tp_bind = CLASS_BINDER(Input, t)
	{
		return t;
	};
}