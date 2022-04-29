#include <core/input/input.hpp>

using namespace ism;

MEMBER_IMPL(Input::singleton) {};

EMBED_OBJECT_CLASS(Input, t)
{
	t.tp_bind = CLASS_BINDER(Input, t)
	{
		return t;
	};
}