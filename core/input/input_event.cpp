#include <core/input/input_event.hpp>

using namespace ism;

OBJECT_IMP(InputEvent, t)
{
	t.tp_new = (newfunc)[](TYPE type, OBJ args)->OBJ { return memnew(InputEvent); };
}

InputEvent::~InputEvent()
{
}