#include <core/io/event.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OBJECT_EMBED(Event, t, TypeFlags_IsAbstract)
	{
		t.tp_bind = CLASS_INSTALLER(Event, t)
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

	OBJECT_EMBED(EventHandler, t, TypeFlags_IsAbstract)
	{
		t.tp_bind = CLASS_INSTALLER(EventHandler, t)
		{
			return t
				.def("handle_event", &EventHandler::handle_event)
				;
		};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OBJECT_EMBED(DummyHandler, t) {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OBJECT_EMBED(EventDelegate<Event>, t) {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EventBus * EventBus::__singleton{};

	OBJECT_EMBED(EventBus, t) {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}