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

EMBEDED_CLASS(EventHandler, t, TypeFlags_IsAbstract) {}

EventHandler::EventHandler(EventBus * bus) noexcept
	: m_bus{ bus ? bus : VALIDATE(EventBus::get_singleton()) }
	, m_order{ get_bus()->next_id() }
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EMBEDED_CLASS(DummyHandler, t) {}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EMBEDED_CLASS(EventDelegate<void>, t) {}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

MEMBER_IMPL(EventBus::singleton) {};

EMBEDED_CLASS(EventBus, t) {}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */