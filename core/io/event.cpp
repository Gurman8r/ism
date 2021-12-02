#include <core/io/event.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EMBEDED_CLASS(Event, t, TypeFlags_IsAbstract)
{
	CLASS_DEF(Event, t)
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

EMBEDED_CLASS(EventHandler, t, TypeFlags_IsAbstract)
{
	CLASS_DEF(EventHandler, t)
	{
		return t
			.def("handle_event", &EventHandler::handle_event)
			;
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EMBEDED_CLASS(DummyHandler, t) {}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EMBEDED_CLASS(EventDelegate<Event>, t) {}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

MEMBER_IMPL(EventBus::singleton) {};

EMBEDED_CLASS(EventBus, t) {}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */