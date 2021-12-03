#include <core/io/event.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EMBED_CLASS(Event, t, TypeFlags_IsAbstract)
{
	t.tp_bind = CLASS_BINDER(Event, t)
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

EMBED_CLASS(EventHandler, t, TypeFlags_IsAbstract)
{
	t.tp_bind = CLASS_BINDER(EventHandler, t)
	{
		return t
			.def("handle_event", &EventHandler::handle_event)
			;
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EMBED_CLASS(DummyHandler, t) {}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EMBED_CLASS(EventDelegate<Event>, t) {}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

MEMBER_IMPL(EventBus::singleton) {};

EMBED_CLASS(EventBus, t) {}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */