#include <core/input/input_map.hpp>

using namespace ism;

OBJECT_IMP(InputMap, t, TypeFlags_BaseType)
{
	t.tp_new = (newfunc)[](TYPE type, OBJ args)->OBJ { return memnew(InputMap); };
}

InputMap::~InputMap()
{
}