#ifndef _ISM_INPUT_EVENT_HPP_
#define _ISM_INPUT_EVENT_HPP_

#include <core/io/resource.hpp>

namespace ism
{
	// key modifiers
	enum keymods_ : int32_t
	{
		keymods_none	= 0 << 0,	// nullptr
		keymods_shift	= 0 << 1,	// shift
		keymods_ctrl	= 1 << 1,	// ctrl
		keymods_alt		= 1 << 2,	// alt
		keymods_super	= 1 << 3,	// super
		keymods_caps	= 1 << 4,	// caps
		keymods_numlk	= 1 << 5	// numlock
	};

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
}

namespace ism
{
	class ISM_API InputEvent : public Resource
	{
	public:
		virtual ~InputEvent() override;

		InputEvent() {}
	};
}

#endif // !_ISM_INPUT_EVENT_HPP_
