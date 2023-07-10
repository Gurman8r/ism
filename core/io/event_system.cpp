#include <core/io/event_system.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OBJECT_EMBED(Event, t, TypeFlags_IsAbstract)
	{
		t.tp_bind = CLASS_(Event, klass)
		{
			return klass
				.def("get_event_id", &Event::get_event_id)
				.def("__int__", &Event::operator EventID)
				.def("__eq__", &Event::operator==)
				.def("__ne__", &Event::operator!=)
				;
		};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OBJECT_EMBED(EventListener, t, TypeFlags_IsAbstract)
	{
		t.tp_bind = CLASS_(EventListener, klass)
		{
			return klass
				.def("handle_event", &EventListener::handle_event)
				;
		};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OBJECT_EMBED(DummyListener, t) {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OBJECT_EMBED(EventDelegate<Event>, t) {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	SINGLETON_EMBED(EventBus);

	OBJECT_EMBED(EventBus, t) {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}