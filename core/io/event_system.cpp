#include <core/io/event_system.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EMBED_CLASS(Event, t, TypeFlags_IsAbstract)
{
	CLASS_DEFINITION(Event, t)
	{
		return t
			.def("get_event_id", &Event::get_event_id)
			.def("__int__", &Event::operator EventID)
			.def("__eq__", &Event::operator==)
			.def("__ne__", &Event::operator!=)
			;
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EMBED_CLASS(EventSystem, t) {}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */