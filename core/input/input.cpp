#include <core/input/input.hpp>

using namespace ism;

DECLEXPR(Input::singleton) {};

ISM_OBJECT_IMPLEMENTATION(Input, t, "input", TypeFlags_BaseType)
{
	t.tp_base = typeof<Object>();

	t.tp_new = (newfunc)[](TYPE type, OBJ args)->OBJ { return memnew(Input); };
}

Input::~Input()
{
}