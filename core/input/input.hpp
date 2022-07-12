#ifndef _ISM_INPUT_HPP_
#define _ISM_INPUT_HPP_

#include <core/input/input_event.hpp>
#include <core/input/input_map.hpp>
#include <core/io/resource.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// input
	class ISM_API Input : public Object
	{
		OBJECT_COMMON(Input, Object);

		static Input * __singleton;

	public:
		enum Action_
		{
			Action_Press,
			Action_Release,
			Action_Repeat,
			Action_MAX
		};

		enum Cursor_
		{
			Cursor_Arrow,
			Cursor_IBeam,
			Cursor_Crosshair,
			Cursor_PointingHand,
			Cursor_EW,
			Cursor_NS,
			Cursor_NESW,
			Cursor_NWSE,
			Cursor_ResizeAll,
			Cursor_NotAllowed,
			Cursor_HResize,
			Cursor_VResize,
			Cursor_Hand,
			Cursor_MAX
		};

		enum MouseMode_
		{
			MouseMode_Normal,
			MouseMode_Hidden,
			MouseMode_Disabled,
			MouseMode_MAX
		};

		enum MouseButton_
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

		enum Key_
		{
			Key_Any				= -1,
			Key_None			= 0,
			Key_Space			= 32,
			Key_Apostrophe		= 39,
			Key_Comma			= 44,
			Key_Minus			= 45,
			Key_Period			= 46,
			Key_Slash			= 47,
			Key_Num0			= 48,
			Key_Num1			= 49,
			Key_Num2			= 50,
			Key_Num3			= 51,
			Key_Num4			= 52,
			Key_Num5			= 53,
			Key_Num6			= 54,
			Key_Num7			= 55,
			Key_Num8			= 56,
			Key_Num9			= 57,
			Key_Semicolon		= 59,
			Key_Equal			= 61,
			Key_A				= 65,
			Key_B				= 66,
			Key_C				= 67,
			Key_D				= 68,
			Key_E				= 69,
			Key_F				= 70,
			Key_G				= 71,
			Key_H				= 72,
			Key_I				= 73,
			Key_J				= 74,
			Key_K				= 75,
			Key_L				= 76,
			Key_M				= 77,
			Key_N				= 78,
			Key_O				= 79,
			Key_P				= 80,
			Key_Q				= 81,
			Key_R				= 82,
			Key_S				= 83,
			Key_T				= 84,
			Key_U				= 85,
			Key_V				= 86,
			Key_W				= 87,
			Key_X				= 88,
			Key_Y				= 89,
			Key_Z				= 90,
			Key_LeftBracket		= 91,
			Key_Backslash		= 92,
			Key_RightBracket	= 93,
			Key_GraveAccent		= 96,
			Key_World1			= 161,
			Key_World2			= 162,
			Key_Escape			= 256,
			Key_Enter			= 257,
			Key_Tab				= 258,
			Key_Backspace		= 259,
			Key_Insert			= 260,
			Key_Del				= 261,
			Key_Right			= 262,
			Key_Left			= 263,
			Key_Down			= 264,
			Key_Up				= 265,
			Key_PageUp			= 266,
			Key_PageDown		= 267,
			Key_Home			= 268,
			Key_End				= 269,
			Key_CapsLock		= 280,
			Key_ScrollLock		= 281,
			Key_NumLock			= 282,
			Key_PrintScreen		= 283,
			Key_Pause			= 284,
			Key_F1				= 290,
			Key_F2				= 291,
			Key_F3				= 292,
			Key_F4				= 293,
			Key_F5				= 294,
			Key_F6				= 295,
			Key_F7				= 296,
			Key_F8				= 297,
			Key_F9				= 298,
			Key_F10				= 299,
			Key_F11				= 300,
			Key_F12				= 301,
			Key_F13				= 302,
			Key_F14				= 303,
			Key_F15				= 304,
			Key_F16				= 305,
			Key_F17				= 306,
			Key_F18				= 307,
			Key_F19				= 308,
			Key_F20				= 309,
			Key_F21				= 310,
			Key_F22				= 311,
			Key_F23				= 312,
			Key_F24				= 313,
			Key_F25				= 314,
			Key_Pad0			= 320,
			Key_Pad1			= 321,
			Key_Pad2			= 322,
			Key_Pad3			= 323,
			Key_Pad4			= 324,
			Key_Pad5			= 325,
			Key_Pad6			= 326,
			Key_Pad7			= 327,
			Key_Pad8			= 328,
			Key_Pad9			= 329,
			Key_PadDecimal		= 330,
			Key_PadDivide		= 331,
			Key_PadMultiply		= 332,
			Key_PadSubtract		= 333,
			Key_PadAdd			= 334,
			Key_PadEnter		= 335,
			Key_PadEqual		= 336,
			Key_LeftShift		= 340,
			Key_LeftCtrl		= 341,
			Key_LeftAlt			= 342,
			Key_LeftSuper		= 343,
			Key_RightShift		= 344,
			Key_RightCtrl		= 345,
			Key_RightAlt		= 346,
			Key_RightSuper		= 347,
			Key_Menu			= 348,
			Key_MAX				= 512
		};

		enum Joy_
		{
			Joy_0,
			Joy_1,
			Joy_2,
			Joy_3,
			Joy_4,
			Joy_5,
			Joy_6,
			Joy_7,
			Joy_8,
			Joy_9,
			Joy_10,
			Joy_11,
			Joy_12,
			Joy_13,
			Joy_14,
			Joy_15,
			Joy_MAX
		};

		enum JoyButton_
		{
			JoyButton_A,
			JoyButton_B,
			JoyButton_X,
			JoyButton_Y,
			JoyButton_Left_Bumper,
			JoyButton_Right_Bumper,
			JoyButton_Back,
			JoyButton_Start,
			JoyButton_Guide,
			JoyButton_Left_Thumb,
			JoyButton_Right_Thumb,
			JoyButton_DPAD_Up,
			JoyButton_DPAD_Right,
			JoyButton_DPAD_Down,
			JoyButton_DPAD_Left,
			JoyButton_MAX,

			JoyButton_Cross = JoyButton_A,
			JoyButton_Circle = JoyButton_B,
			JoyButton_Square = JoyButton_X,
			JoyButton_Triangle = JoyButton_Y,
		};

		enum JoyAxis_
		{
			JoyAxis_Left_X,
			JoyAxis_Left_Y,
			JoyAxis_Right_X,
			JoyAxis_Right_Y,
			JoyAxis_Left_Trigger,
			JoyAxis_Right_Trigger,
			JoyAxis_MAX
		};

		struct GamepadState
		{
			byte buttons[JoyButton_MAX]{};
			f32 axes[JoyAxis_MAX]{};
		};

	public:
		Input();
		NODISCARD ~Input() noexcept override = default;
		FORCE_INLINE static Input * get_singleton() noexcept { return __singleton; }

	protected:
		friend class Main;
		friend class DisplayServer;

		MouseMode_	m_mouse_mode{};
		Cursor_		m_cursor_shape{};
		Vec2		m_mouse_position{}, m_prev_mouse_position{};
		Vec2		m_mouse_delta{};
		Vec2		m_mouse_wheel{};
		bool		m_mouse_button_down[MouseButton_MAX]{};
		Duration	m_mouse_button_down_duration[MouseButton_MAX]{};

		bool		m_is_shift : 1, m_is_ctrl : 1, m_is_alt : 1, m_is_super : 1;
		bool		m_key_down[Key_MAX]{};
		Duration	m_key_down_duration[Key_MAX]{};

		f32			m_joy_axes[Joy_MAX][JoyAxis_MAX]{};
		bool		m_joy_button_down[Joy_MAX][JoyButton_MAX]{};
		Duration	m_joy_button_down_duration[Joy_MAX][JoyButton_MAX]{};

	public:
		bool is_key_pressed(i32 key) const;
		bool is_mouse_button_pressed(i32 button) const;
		bool is_joy_button_pressed(i32 device, i32 button) const;
		
		Duration get_key_down_duration(i32 key) const;
		Duration get_mouse_button_down_duration(i32 button) const;
		Duration get_joy_button_down_duration(i32 device, i32 button) const;

		void set_key(i32 key, i32 action);
		void set_mouse_button(i32 button, i32 action);
		void set_joy_button(i32 device, i32 button, i32 action);

		bool is_alt(i32 key = Key_Any) const;
		bool is_ctrl(i32 key = Key_Any) const;
		bool is_shift(i32 key = Key_Any) const;
		bool is_super(i32 key = Key_Any) const;

		Vector<Joy_> get_connected_joys() const;
		String get_joy_name(i32 device) const;
		f32 get_joy_axis(i32 device, i32 axis) const;
		void joy_connection_changed(i32 device, bool connected, String const & guid = {});
		
		bool is_joy_gamepad(i32 device) const;
		String get_gamepad_name(i32 device) const;
		bool parse_joy_mapping(String const & mapping);

		Joy_ get_unused_joy_id();
		bool is_joy_present(i32 device) const;
		String get_joy_guid(i32 device) const;

		MouseMode_ get_mouse_mode() const;
		void set_mouse_mode(MouseMode_ mode);

		Vec2 get_mouse_position() const;
		void set_mouse_position(Vec2 const & position);
		
		Vec2 get_mouse_delta() const;
		
		Vec2 get_mouse_wheel() const;
		void set_mouse_wheel(Vec2 const & wheel);

		Cursor_ get_cursor_shape() const;
		void set_cursor_shape(Cursor_ shape);
		void set_custom_cursor(RES const & cursor, Cursor_ shape = {}, Vec2 const & hotspot = {});

		void iteration(Duration const & delta_time);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_INPUT_HPP_
