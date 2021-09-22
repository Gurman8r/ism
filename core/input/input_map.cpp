#include <core/input/input_map.hpp>

using namespace ism;

ISM_OBJECT_IMPL(InputMap, t, "input_map", TypeFlags_BaseType)
{
	t.tp_new = (newfunc)[](TYPE type, OBJ args)->OBJ { return memnew(InputMap); };
}

InputMap::~InputMap()
{
}