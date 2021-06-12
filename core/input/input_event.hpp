#ifndef _ISM_INPUT_EVENT_HPP_
#define _ISM_INPUT_EVENT_HPP_

#include <core/io/resource.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// mouse buttons
	enum mouse_button_ : int32_t
	{
		mouse_button_0, // mouse button 0
		mouse_button_1, // mouse button 1
		mouse_button_2, // mouse button 2
		mouse_button_3, // mouse button 3
		mouse_button_4, // mouse button 4
		mouse_button_5, // mouse button 5
		mouse_button_6, // mouse button 6
		mouse_button_7, // mouse button 7

		mouse_button_MAX
	};
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API InputEvent : public Resource
	{
		ISM_SUPER_CLASS(InputEvent, Resource);

	public:
		virtual ~InputEvent() override;

		DEFAULT_COPY_AND_MOVE_CONSTRUCTABLE(InputEvent);

	public:
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_INPUT_EVENT_HPP_
