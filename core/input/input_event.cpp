#include <core/input/input_event.hpp>

using namespace ism;

ISM_OBJECT_IMPLEMENTATION(InputEvent, t, "input_event")
{
	t.tp_base = typeof<Resource>();

	t.tp_new = (newfunc)[](TYPE type, OBJ args)->OBJ { return memnew(InputEvent); };
}

InputEvent::~InputEvent()
{
}