#ifndef _ISM_INPUT_EVENT_HPP_
#define _ISM_INPUT_EVENT_HPP_

// PLACEHOLDER

#include <core/io/resource.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// input event
	class ISM_API InputEvent : public Resource
	{
		OBJECT_CLASS(InputEvent, Resource);

		friend class Input;

	public:
		InputEvent() noexcept;

		virtual ~InputEvent() noexcept override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_INPUT_EVENT_HPP_
