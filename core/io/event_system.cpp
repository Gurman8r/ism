#include <core/io/event_system.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(Event, t, TypeFlags_IsAbstract)
	{
		t.tp_bind = BIND_CLASS(Event, t)
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
		t.tp_bind = BIND_CLASS(EventHandler, t)
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

	EventBus * EventBus::__singleton{};

	EMBED_CLASS(EventBus, t) {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}