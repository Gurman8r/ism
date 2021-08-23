#ifndef _ISM_INPUT_EVENT_HPP_
#define _ISM_INPUT_EVENT_HPP_

#include <core/io/resource.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API InputEvent : public Resource
	{
		ISM_OBJECT_DEFAULT(InputEvent, Resource);

	public:
		virtual ~InputEvent();

		InputEvent() noexcept : InputEvent{ get_class() } {}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_INPUT_EVENT_HPP_
