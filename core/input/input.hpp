#ifndef _ISM_INPUT_HPP_
#define _ISM_INPUT_HPP_

#include <core/input/input_event.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// cursor handle
	DECL_HANDLE(CursorID);

	// cursor mode
	typedef enum CursorMode_ : int32_t
	{
		CursorMode_Normal		, // normal
		CursorMode_Hidden		, // hidden
		CursorMode_Disabled		, // disabled
	}
	CursorMode;

	// cursor shape
	typedef enum CursorShape_ : int32_t
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
	}
	CursorShape;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API Input : public Object
	{
		ISM_OBJECT_DEFAULT(Input, Object);

	private:
		static Input * singleton;

	public:
		virtual ~Input();

		Input() noexcept : Input{ get_class() } {}

	public:
		FORCE_INLINE static Input * get_singleton() { return singleton; }

		NODISCARD bool get_key_down(int32_t) const { return false; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	NODISCARD inline Input & get_input() { return *CHECK(Input::get_singleton()); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_INPUT_HPP_
