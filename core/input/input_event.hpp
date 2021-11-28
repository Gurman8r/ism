#ifndef _ISM_INPUT_EVENT_HPP_
#define _ISM_INPUT_EVENT_HPP_

#include <core/io/event_system.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EVENT_CLASS(InputEvent)
	{
		OBJECT_COMMON(InputEvent, Event);

	public:
		explicit InputEvent() noexcept = default;

		virtual ~InputEvent() noexcept override = default;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_INPUT_EVENT_HPP_
