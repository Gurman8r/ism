#ifndef _ISM_INPUT_HPP_
#define _ISM_INPUT_HPP_

#include <core/input/keyboard.hpp>
#include <core/input/input_event.hpp>
#include <core/input/input_map.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// cursor handle
	MAKE_OPAQUE(CursorID);

	// cursor mode
	MAKE_ENUM(CursorMode)
	{
		CursorMode_Normal		, // normal
		CursorMode_Hidden		, // hidden
		CursorMode_Disabled		, // disabled
	};

	// cursor shape
	MAKE_ENUM(CursorShape)
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
		explicit Input() noexcept : Object{} { singleton = this; }

	public:
		virtual ~Input() override {}

		FORCE_INLINE static Input * get_singleton() noexcept { return singleton; }

		NODISCARD bool get_key_down(int32_t) const { return false; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	NODISCARD inline Input & get_input() noexcept { return *VALIDATE(Input::get_singleton()); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_INPUT_HPP_
