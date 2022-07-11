#ifndef _ISM_INPUT_HPP_
#define _ISM_INPUT_HPP_

#include <core/input/input_event.hpp>
#include <core/input/input_map.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// input
	class ISM_API Input : public Object
	{
		OBJECT_COMMON(Input, Object);

		friend class Main;

		static Input * __singleton;

	public:
		Input() noexcept { __singleton = this; }
		virtual ~Input() noexcept override = default;
		FORCE_INLINE static Input * get_singleton() noexcept { return __singleton; }

	public:
		enum { PRESS, RELEASE, REPEAT };

		enum Key_
		{
			Key_Space			= 32,	// space
			Key_Apostrophe		= 39,	// apostrophe
			Key_Comma			= 44,	// comma
			Key_Minus			= 45,	// minus
			Key_Period			= 46,	// period
			Key_Slash			= 47,	// slash
			Key_Num0			= 48,	// 0
			Key_Num1			= 49,	// 1
			Key_Num2			= 50,	// 2
			Key_Num3			= 51,	// 3
			Key_Num4			= 52,	// 4
			Key_Num5			= 53,	// 5
			Key_Num6			= 54,	// 6
			Key_Num7			= 55,	// 7
			Key_Num8			= 56,	// 8
			Key_Num9			= 57,	// 9
			Key_Semicolon		= 59,	// semicolon
			Key_Equal			= 61,	// equal
			Key_A				= 65,	// a
			Key_B				= 66,	// b
			Key_C				= 67,	// c
			Key_D				= 68,	// d
			Key_E				= 69,	// e
			Key_F				= 70,	// f
			Key_G				= 71,	// g
			Key_H				= 72,	// h
			Key_I				= 73,	// i
			Key_J				= 74,	// j
			Key_K				= 75,	// k
			Key_L				= 76,	// l
			Key_M				= 77,	// m
			Key_N				= 78,	// n
			Key_O				= 79,	// o
			Key_P				= 80,	// p
			Key_Q				= 81,	// q
			Key_R				= 82,	// r
			Key_S				= 83,	// s
			Key_T				= 84,	// t
			Key_U				= 85,	// u
			Key_V				= 86,	// v
			Key_W				= 87,	// w
			Key_X				= 88,	// x
			Key_Y				= 89,	// y
			Key_Z				= 90,	// z
			Key_LeftBracket		= 91,	// left bracket
			Key_Backslash		= 92,	// backslash
			Key_RightBracket	= 93,	// right bracket
			Key_GraveAccent		= 96,	// grave accent
			Key_World1			= 161,	// world 1
			Key_World2			= 162,	// world 2
			Key_Escape			= 256,	// escape
			Key_Enter			= 257,	// enter
			Key_Tab				= 258,	// tab
			Key_Backspace		= 259,	// backspace
			Key_Insert			= 260,	// insert
			Key_Del				= 261,	// del
			Key_Right			= 262,	// right
			Key_Left			= 263,	// left
			Key_Down			= 264,	// down
			Key_Up				= 265,	// up
			Key_PageUp			= 266,	// page up
			Key_PageDown		= 267,	// page down
			Key_Home			= 268,	// home
			Key_End				= 269,	// end
			Key_CapsLock		= 280,	// caps lock
			Key_ScrollLock		= 281,	// scroll lock
			Key_NumLock			= 282,	// num lock
			Key_PrintScreen		= 283,	// print screen
			Key_Pause			= 284,	// pause
			Key_Fn1				= 290,	// f1
			Key_Fn2				= 291,	// f2
			Key_Fn3				= 292,	// f3
			Key_Fn4				= 293,	// f4
			Key_Fn5				= 294,	// f5
			Key_Fn6				= 295,	// f6
			Key_Fn7				= 296,	// f7
			Key_Fn8				= 297,	// f8
			Key_Fn9				= 298,	// f9
			Key_Fn10			= 299,	// f10
			Key_Fn11			= 300,	// f11
			Key_Fn12			= 301,	// f12
			Key_Fn13			= 302,	// f13
			Key_Fn14			= 303,	// f14
			Key_Fn15			= 304,	// f15
			Key_Fn16			= 305,	// f16
			Key_Fn17			= 306,	// f17
			Key_Fn18			= 307,	// f18
			Key_Fn19			= 308,	// f19
			Key_Fn20			= 309,	// f20
			Key_Fn21			= 310,	// f21
			Key_Fn22			= 311,	// f22
			Key_Fn23			= 312,	// f23
			Key_Fn24			= 313,	// f24
			Key_Fn25			= 314,	// f25
			Key_Kp0				= 320,	// keypad 0
			Key_Kp1				= 321,	// keypad 1
			Key_Kp2				= 322,	// keypad 2
			Key_Kp3				= 323,	// keypad 3
			Key_Kp4				= 324,	// keypad 4
			Key_Kp5				= 325,	// keypad 5
			Key_Kp6				= 326,	// keypad 6
			Key_Kp7				= 327,	// keypad 7
			Key_Kp8				= 328,	// keypad 8
			Key_Kp9				= 329,	// keypad 9
			Key_KpDecimal		= 330,	// keypad decimal
			Key_KpDivide		= 331,	// keypad divide
			Key_KpMultiply		= 332,	// keypad multiply
			Key_KpSubtract		= 333,	// keypad subtract
			Key_KpAdd			= 334,	// keypad add
			Key_KpEnter			= 335,	// keypad enter
			Key_KpEqual			= 336,	// keypad equal
			Key_LeftShift		= 340,	// left shift
			Key_LeftCtrl		= 341,	// left control
			Key_LeftAlt			= 342,	// left alt
			Key_LeftSuper		= 343,	// left super
			Key_RightShift		= 344,	// right shift
			Key_RightCtrl		= 345,	// right control
			Key_RightAlt		= 346,	// right alt
			Key_RightSuper		= 347,	// right super
			Key_Menu			= 348,	// menu

			Key_MAX
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

		enum MouseMode_
		{
			MouseMode_Normal,
			MouseMode_Hidden,
			MouseMode_Disabled,
			MouseMode_MAX
		};

		enum CursorShape_
		{
			CursorShape_Arrow,
			CursorShape_IBeam,
			CursorShape_Crosshair,
			CursorShape_PointingHand,
			CursorShape_EW,
			CursorShape_NS,
			CursorShape_NESW,
			CursorShape_NWSE,
			CursorShape_ResizeAll,
			CursorShape_NotAllowed,
			CursorShape_HResize,
			CursorShape_VResize,
			CursorShape_Hand,
			CursorShape_MAX
		};

		enum Joystick_
		{
			Joystick_0,
			Joystick_1,
			Joystick_2,
			Joystick_3,
			Joystick_4,
			Joystick_5,
			Joystick_6,
			Joystick_7,
			Joystick_8,
			Joystick_9,
			Joystick_10,
			Joystick_11,
			Joystick_12,
			Joystick_13,
			Joystick_14,
			Joystick_15,
			Joystick_MAX
		};

		enum GamepadButton_
		{
			GamepadButton_A,
			GamepadButton_B,
			GamepadButton_X,
			GamepadButton_Y,
			GamepadButton_Left_Bumper,
			GamepadButton_Right_Bumper,
			GamepadButton_Back,
			GamepadButton_Start,
			GamepadButton_Guide,
			GamepadButton_Left_Thumb,
			GamepadButton_Right_Thumb,
			GamepadButton_DPAD_Up,
			GamepadButton_DPAD_Right,
			GamepadButton_DPAD_Down,
			GamepadButton_DPAD_Left,
			GamepadButton_MAX
		};

		enum GamepadAxis_
		{
			GamepadAxis_Left_X,
			GamepadAxis_Left_Y,
			GamepadAxis_Right_X,
			GamepadAxis_Right_Y,
			GamepadAxis_Left_Trigger,
			GamepadAxis_Right_Trigger,
			GamepadAxis_MAX
		};

		struct GamepadState
		{
			Array<uint8_t, GamepadButton_MAX> buttons{};
			Array<float_t, GamepadAxis_MAX> axes{};
		};

	protected:
		bool m_is_shift : 1, m_is_ctrl : 1, m_is_alt : 1, m_is_super : 1;

		uint32_t m_typed{ '0' };

		Vec2 m_mouse_position{};
		Vec2 m_prev_mouse_position{};
		Vec2 m_mouse_delta{};
		Vec2 m_scroll{};

		Array<uint8_t, MouseButton_MAX> m_mouse_down{};
		Array<uint8_t, Key_MAX> m_keys_down{};

		Array<Duration, MouseButton_MAX> m_mouse_down_duration{};
		Array<Duration, Key_MAX> m_keys_down_duration{};

	public:
		NODISCARD bool is_shift() const noexcept { return m_is_shift; }
		NODISCARD bool is_shift(Key_ key) const noexcept { return m_is_shift && m_keys_down[key]; }
		
		NODISCARD bool is_ctrl() const noexcept { return m_is_ctrl; }
		NODISCARD bool is_ctrl(Key_ key) const noexcept { return m_is_ctrl && m_keys_down[key]; }
		
		NODISCARD bool is_alt() const noexcept { return m_is_alt; }
		NODISCARD bool is_alt(Key_ key) const noexcept { return m_is_alt && m_keys_down[key]; }
		
		NODISCARD bool is_super() const noexcept { return m_is_super; }
		NODISCARD bool is_super(Key_ key) const noexcept { return m_is_super && m_keys_down[key]; }
		
		NODISCARD char get_char() const noexcept { return (char)m_typed; }
		NODISCARD bool get_key_up(Key_ key) const noexcept { return !m_keys_down[key]; }
		NODISCARD bool get_key_down(Key_ key) const noexcept { return m_keys_down[key]; }
		NODISCARD auto get_key_down_duration(Key_ key) const noexcept { return m_keys_down_duration[key]; }

		NODISCARD Vec2 get_mouse_pos() const noexcept { return m_mouse_position; }
		NODISCARD bool get_mouse_up(MouseButton_ button) const noexcept { return !m_mouse_down[button]; }
		NODISCARD bool get_mouse_down(MouseButton_ button) const noexcept { return m_mouse_down[button]; }
		NODISCARD auto get_mouse_down_duration(MouseButton_ button) const noexcept { return m_mouse_down_duration[button]; }
		NODISCARD Vec2 get_mouse_delta() const noexcept { return m_mouse_delta; }
		NODISCARD Vec2 get_scroll() const noexcept { return m_scroll; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_INPUT_HPP_
