#ifndef _ISM_INPUT_EVENT_HPP_
#define _ISM_INPUT_EVENT_HPP_

// PLACEHOLDER

#include <core/io/resource.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// input event
	class ISM_API InputEvent : public Resource
	{
		DEFINE_CLASS(InputEvent, Resource);

		friend class Input;

	public:
		InputEvent() noexcept;

		virtual ~InputEvent() noexcept override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_INPUT_EVENT_HPP_
