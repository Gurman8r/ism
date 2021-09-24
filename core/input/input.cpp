#include <core/input/input.hpp>

using namespace ism;

MEMBER_IMP(Input::singleton) {};

OBJECT_IMP(Input, t, TypeFlags_BaseType)
{
	t.tp_new = (newfunc)[](TYPE type, OBJ args)->OBJ { return memnew(Input); };
}