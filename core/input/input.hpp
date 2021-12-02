#ifndef _ISM_INPUT_HPP_
#define _ISM_INPUT_HPP_

#include <core/input/keyboard.hpp>
#include <core/input/input_event.hpp>
#include <core/input/input_map.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// cursor handle
	OPAQUE_TYPE(CursorID);

	// cursor mode
	ENUM_INT(CursorMode)
	{
		CursorMode_Normal		, // normal
		CursorMode_Hidden		, // hidden
		CursorMode_Disabled		, // disabled
	};

	// cursor shape
	ENUM_INT(CursorShape)
	{
		CursorShape_Arrow			, // arrow
		CursorShape_IBeam			, // ibeam
		CursorShape_Crosshair		, // crosshair
		CursorShape_PointingHand	, // pointing hand
		CursorShape_EW				, // ew
		CursorShape_NS				, // ns
		CursorShape_NESW			, // nesw
		CursorShape_NWSE			, // nwse
		CursorShape_ResizeAll		, // resize all
		CursorShape_NotAllowed		, // not allowed
		CursorShape_HResize			, // hresize
		CursorShape_VResize			, // vresize
		CursorShape_Hand			, // hand
	};

	// input action
	ENUM_INT(InputAction)
	{
		InputAction_Press,
		InputAction_Release,
		InputAction_Repeat,
	};

	// input mode
	ENUM_INT(InputMode)
	{
		InputMode_Cursor,
		InputMode_StickyKeys,
		InputMode_StickyMouseButtons,
		InputMode_LockKeyMods,
		InputMode_RawMouseMotion,
	};

	// mouse button
	ENUM_INT(MouseButton)
	{
		MouseButton_0,
		MouseButton_1,
		MouseButton_2,
		MouseButton_3,
		MouseButton_4,
		MouseButton_5,
		MouseButton_6,
		MouseButton_7,

		MouseButton_MAX
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API Input : public Object
	{
		OBJECT_COMMON(Input, Object);

		static Input * singleton;

	public:
		explicit Input() noexcept { singleton = this; }

	public:
		virtual ~Input() override {}

		FORCE_INLINE static Input * get_singleton() noexcept { return singleton; }

		NODISCARD bool get_key_down(int32_t) const { return false; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_INPUT_HPP_
