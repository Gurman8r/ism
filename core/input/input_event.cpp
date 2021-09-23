#include <core/input/input_event.hpp>

using namespace ism;

OBJ_IMPL(InputEvent, t, "input_event")
{
	t.tp_new = (newfunc)[](TYPE type, OBJ args)->OBJ { return memnew(InputEvent); };
}

InputEvent::~InputEvent()
{
}