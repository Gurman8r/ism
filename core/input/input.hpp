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

		enum Key_
		{
			Key_Any = -1,
			Key_Space = 32,
			Key_Apostrophe = 39,
			Key_Comma = 44,
			Key_Minus = 45,
			Key_Period = 46,
			Key_Slash = 47,
			Key_Num0 = 48,
			Key_Num1 = 49,
			Key_Num2 = 50,
			Key_Num3 = 51,
			Key_Num4 = 52,
			Key_Num5 = 53,
			Key_Num6 = 54,
			Key_Num7 = 55,
			Key_Num8 = 56,
			Key_Num9 = 57,
			Key_Semicolon = 59,
			Key_Equal = 61,
			Key_A = 65,
			Key_B = 66,
			Key_C = 67,
			Key_D = 68,
			Key_E = 69,
			Key_F = 70,
			Key_G = 71,
			Key_H = 72,
			Key_I = 73,
			Key_J = 74,
			Key_K = 75,
			Key_L = 76,
			Key_M = 77,
			Key_N = 78,
			Key_O = 79,
			Key_P = 80,
			Key_Q = 81,
			Key_R = 82,
			Key_S = 83,
			Key_T = 84,
			Key_U = 85,
			Key_V = 86,
			Key_W = 87,
			Key_X = 88,
			Key_Y = 89,
			Key_Z = 90,
			Key_LeftBracket = 91,
			Key_Backslash = 92,
			Key_RightBracket = 93,
			Key_GraveAccent = 96,
			Key_World1 = 161,
			Key_World2 = 162,
			Key_Escape = 256,
			Key_Enter = 257,
			Key_Tab = 258,
			Key_Backspace = 259,
			Key_Insert = 260,
			Key_Del = 261,
			Key_Right = 262,
			Key_Left = 263,
			Key_Down = 264,
			Key_Up = 265,
			Key_PageUp = 266,
			Key_PageDown = 267,
			Key_Home = 268,
			Key_End = 269,
			Key_CapsLock = 280,
			Key_ScrollLock = 281,
			Key_NumLock = 282,
			Key_PrintScreen = 283,
			Key_Pause = 284,
			Key_F1 = 290,
			Key_F2 = 291,
			Key_F3 = 292,
			Key_F4 = 293,
			Key_F5 = 294,
			Key_F6 = 295,
			Key_F7 = 296,
			Key_F8 = 297,
			Key_F9 = 298,
			Key_F10 = 299,
			Key_F11 = 300,
			Key_F12 = 301,
			Key_F13 = 302,
			Key_F14 = 303,
			Key_F15 = 304,
			Key_F16 = 305,
			Key_F17 = 306,
			Key_F18 = 307,
			Key_F19 = 308,
			Key_F20 = 309,
			Key_F21 = 310,
			Key_F22 = 311,
			Key_F23 = 312,
			Key_F24 = 313,
			Key_F25 = 314,
			Key_Pad0 = 320,
			Key_Pad1 = 321,
			Key_Pad2 = 322,
			Key_Pad3 = 323,
			Key_Pad4 = 324,
			Key_Pad5 = 325,
			Key_Pad6 = 326,
			Key_Pad7 = 327,
			Key_Pad8 = 328,
			Key_Pad9 = 329,
			Key_PadDecimal = 330,
			Key_PadDivide = 331,
			Key_PadMultiply = 332,
			Key_PadSubtract = 333,
			Key_PadAdd = 334,
			Key_PadEnter = 335,
			Key_PadEqual = 336,
			Key_LeftShift = 340,
			Key_LeftCtrl = 341,
			Key_LeftAlt = 342,
			Key_LeftSuper = 343,
			Key_RightShift = 344,
			Key_RightCtrl = 345,
			Key_RightAlt = 346,
			Key_RightSuper = 347,
			Key_Menu = 348,
			Key_MAX = 512
		};

		enum KeyMods_
		{
			KeyMods_None = 0,
			KeyMods_Ctrl = 1 << 0,
			KeyMods_Shift = 1 << 1,
			KeyMods_Alt = 1 << 2,
			KeyMods_Super = 1 << 3
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

		enum NavInput_
		{
			NavInput_Activate,
			NavInput_Cancel,
			NavInput_Input,
			NavInput_Menu,
			NavInput_DpadLeft,
			NavInput_DpadRight,
			NavInput_DpadUp,
			NavInput_DpadDown,
			NavInput_LStickLeft,
			NavInput_LStickRight,
			NavInput_LStickUp,
			NavInput_LStickDown,
			NavInput_FocusPrev,
			NavInput_FocusNext,

			NavInput_MAX
		};

		struct GamepadState
		{
			byte buttons[JoyButton_MAX]{};
			f32 axes[JoyAxis_MAX]{};
		};

	public:
		Input();
		virtual ~Input() noexcept override = default;
		FORCE_INLINE static Input * get_singleton() noexcept { return __singleton; }

	protected:
		friend class DisplayServer;

		static struct Funcs
		{
			MouseMode_(*get_mouse_mode)() = 0;
			void (*set_mouse_mode)(MouseMode_) = 0;
			void (*set_mouse_position)(Vec2 const &) = 0;

			CursorShape_(*get_cursor_shape)() = 0;
			void (*set_cursor_shape)(CursorShape_) = 0;
			void (*set_custom_cursor_image)(RES const &, CursorShape_, Vec2 const &) = 0;
		}
		m_funcs;

		/* MOUSE */
		Vec2	m_mouse_position{}, m_mouse_position_prev{};
		Vec2	m_last_valid_mouse_position{};
		Vec2	m_mouse_delta{}, m_mouse_delta_prev{};
		Vec2	m_mouse_drag{}, m_mouse_drag_prev{};
		Vec2	m_mouse_wheel{};
		bool	m_mouse_down[MouseButton_MAX]{};
		bool	m_mouse_clicked[MouseButton_MAX]{};
		bool	m_mouse_released[MouseButton_MAX]{};
		bool	m_mouse_double_clicked[MouseButton_MAX]{};
		f32		m_mouse_down_duration[MouseButton_MAX]{};
		f32		m_mouse_down_duration_prev[MouseButton_MAX]{};
		Vec2	m_mouse_clicked_position[MouseButton_MAX]{};
		Vec2	m_mouse_released_position[MouseButton_MAX]{};

		/* KEYBOARD */
		bool	m_is_shift : 1, m_is_ctrl : 1, m_is_alt : 1, m_is_super : 1;
		i32		m_key_mods{ KeyMods_None };
		bool	m_key_down[Key_MAX]{};
		f32		m_key_down_duration[Key_MAX]{};
		f32		m_key_down_duration_prev[Key_MAX]{};

		/* JOYSTICKS */
		bool	m_joy_known[Joy_MAX]{};
		String	m_joy_name[Joy_MAX]{};
		String	m_joy_guid[Joy_MAX]{};
		f32		m_joy_axes[Joy_MAX][JoyAxis_MAX]{};
		bool	m_joy_down[Joy_MAX][JoyButton_MAX]{};
		f32		m_joy_down_duration[Joy_MAX][JoyButton_MAX]{};
		f32		m_joy_down_duration_prev[Joy_MAX][JoyButton_MAX]{};

		/* NAV INPUTS */
		bool	m_nav_input_down[NavInput_MAX]{};
		f32		m_nav_input_down_duration[NavInput_MAX]{};
		f32		m_nav_input_down_duration_prev[NavInput_MAX]{};

	public:
		bool is_key_down(i32 key) const;
		bool is_mouse_button_down(i32 button) const;
		bool is_joy_button_down(i32 device, i32 button) const;
		bool is_nav_input_down(i32 nav_input) const;

		bool is_any_key_down() const;
		bool is_any_mouse_button_down() const;
		bool is_any_joy_button_down(i32 device) const;
		bool is_any_nav_input_down() const;

		f32 get_key_down_duration(i32 key) const;
		f32 get_mouse_button_down_duration(i32 button) const;
		f32 get_joy_button_down_duration(i32 device, i32 button) const;
		f32 get_nav_input_down_duration(i32 nav_input) const;

		bool is_key_past_threshold(i32 key, f32 threshold) const;
		bool is_mouse_button_past_threshold(i32 button, f32 threshold) const;
		bool is_joy_button_past_threshold(i32 device, i32 button, f32 threshold) const;
		bool is_nav_input_past_threshold(i32 nav_input, f32 threshold) const;

		void set_key(i32 key, bool pressed);
		void set_mouse_button(i32 button, bool pressed);
		void set_joy_button(i32 device, i32 button, bool pressed);
		void set_nav_input(i32 nav_input, bool pressed);

	public:
		Vector<Joy_> get_connected_joys() const;
		Joy_ get_unused_joy_id();
		bool is_joy_known(i32 device) const;
		String get_joy_name(i32 device) const;
		String get_joy_guid(i32 device) const;
		f32 get_joy_axis(i32 device, i32 axis) const;
		void set_joy_axis(i32 device, i32 axis, f32 value);
		void joy_connection_changed(i32 device, bool connected, String const & name = {}, String const & guid = {});
		bool parse_mapping(String const & mapping);

	public:
		MouseMode_ get_mouse_mode() const;
		void set_mouse_mode(MouseMode_ mode);

		Vec2 get_mouse_position() const;
		void set_mouse_position(Vec2 const & position);

		Vec2 get_mouse_delta() const;
		Vec2 get_last_mouse_delta() const;
		bool is_mouse_dragging(i32 button, f32 threshold = -1.f) const;

		Vec2 get_mouse_wheel() const;
		void set_mouse_wheel(Vec2 const & wheel);

		CursorShape_ get_cursor_shape() const;
		void set_cursor_shape(CursorShape_ shape);
		void set_custom_cursor_image(RES const & cursor, CursorShape_ shape = CursorShape_Arrow, Vec2 const & hotspot = {});

	public:
		void iteration(Duration const delta_time);

	public:
		NODISCARD static constexpr bool is_valid_mouse_position(Vec2 const * value = nullptr) noexcept
		{
			return (value ? *value : __singleton->m_mouse_position) >= Vec2{ -256000.f, -256000.f };
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_INPUT_HPP_
