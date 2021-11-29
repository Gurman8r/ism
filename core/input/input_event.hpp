#ifndef _ISM_INPUT_EVENT_HPP_
#define _ISM_INPUT_EVENT_HPP_

#include <core/io/event.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API InputEvent : public EventClass<InputEvent>
	{
		OBJECT_COMMON(InputEvent, Event);

	public:
		InputEvent() noexcept {}

		virtual ~InputEvent() noexcept override = default;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_INPUT_EVENT_HPP_
