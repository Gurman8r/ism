#include <core/input/input.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Input * Input::__singleton{};

	OBJECT_EMBED(Input, t) {}

	Input::Input()
	{
		__singleton = this;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Input::is_key_pressed(i32 key) const
	{
		ASSERT(key < Key_MAX);
		return m_key_down[key];
	}

	bool Input::is_mouse_button_pressed(i32 button) const
	{
		ASSERT(button < MouseButton_MAX);
		return m_mouse_button_down[button];
	}

	bool Input::is_joy_button_pressed(i32 device, i32 button) const
	{
		ASSERT(button < JoyButton_MAX);
		return m_joy_button_down[device][button];
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Duration Input::get_key_down_duration(i32 key) const
	{
		return m_key_down_duration[key];
	}

	Duration Input::get_mouse_button_down_duration(i32 button) const
	{
		return m_mouse_button_down_duration[button];
	}

	Duration Input::get_joy_button_down_duration(i32 device, i32 button) const
	{
		return m_joy_button_down_duration[device][button];
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Input::set_key(i32 key, i32 action)
	{
		m_key_down[key] = action != Input::Action_Release;
		m_is_shift = m_key_down[Input::Key_LeftShift] || m_key_down[Input::Key_RightShift];
		m_is_ctrl = m_key_down[Input::Key_LeftCtrl] || m_key_down[Input::Key_RightCtrl];
		m_is_alt = m_key_down[Input::Key_LeftAlt] || m_key_down[Input::Key_RightAlt];
		m_is_super = m_key_down[Input::Key_LeftSuper] || m_key_down[Input::Key_RightSuper];
	}

	void Input::set_mouse_button(i32 button, i32 action)
	{
		m_mouse_button_down[button] = action != Input::Action_Release;
	}

	void Input::set_joy_button(i32 device, i32 button, i32 action)
	{
		m_joy_button_down[device][button] = action != Input::Action_Release;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Input::is_alt(i32 key) const
	{
		return m_is_alt && (key == Key_Any || is_key_pressed(key));
	}

	bool Input::is_ctrl(i32 key) const
	{
		return m_is_ctrl && (key == Key_Any || is_key_pressed(key));
	}

	bool Input::is_shift(i32 key) const
	{
		return m_is_shift && (key == Key_Any || is_key_pressed(key));
	}

	bool Input::is_super(i32 key) const
	{
		return m_is_super && (key == Key_Any || is_key_pressed(key));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Vector<Input::Joy_> Input::get_connected_joys() const
	{
		return {};
	}

	String Input::get_joy_name(i32 device) const
	{
		return String();
	}

	f32 Input::get_joy_axis(i32 device, i32 axis) const
	{
		return m_joy_axes[device][axis];
	}

	void Input::joy_connection_changed(i32 device, bool connected, String const & guid)
	{
	}

	bool Input::is_joy_gamepad(i32 device) const
	{
		return false;
	}

	String Input::get_gamepad_name(i32 device) const
	{
		return String();
	}

	bool Input::parse_joy_mapping(String const & mapping)
	{
		return false;
	}

	Input::Joy_ Input::get_unused_joy_id()
	{
		return Joy_();
	}

	bool Input::is_joy_present(i32 device) const
	{
		return false;
	}

	String Input::get_joy_guid(i32 device) const
	{
		return String();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Input::MouseMode_ Input::get_mouse_mode() const
	{
		return m_mouse_mode;
	}

	void Input::set_mouse_mode(MouseMode_ mode)
	{
		m_mouse_mode = mode;
	}

	Vec2 Input::get_mouse_position() const
	{
		return m_mouse_position;
	}

	void Input::set_mouse_position(Vec2 const & position)
	{
		m_mouse_position = position;
	}

	Vec2 Input::get_mouse_delta() const
	{
		return m_mouse_delta;
	}

	Vec2 Input::get_mouse_wheel() const
	{
		return m_mouse_wheel;
	}

	void Input::set_mouse_wheel(Vec2 const & wheel)
	{
		m_mouse_wheel = wheel;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Input::Cursor_ Input::get_cursor_shape() const
	{
		return m_cursor_shape;
	}

	void Input::set_cursor_shape(Cursor_ shape)
	{
		m_cursor_shape = shape;
	}

	void Input::set_custom_cursor(RES const & cursor, Cursor_ shape, Vec2 const & hotspot)
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Input::iteration(Duration const & delta_time)
	{
		m_mouse_delta = m_mouse_position - m_prev_mouse_position;
		m_prev_mouse_position = m_mouse_position;

		for (i32 i = 0; i < MouseButton_MAX; ++i)
		{
			m_mouse_button_down_duration[i] = m_mouse_button_down[i]
				? (m_mouse_button_down_duration[i] < 0.f
					? 0.f
					: m_mouse_button_down_duration[i] + delta_time)
				: -1.f;
		}

		for (i32 i = 0; i < Key_MAX; ++i)
		{
			m_key_down_duration[i] = m_key_down[i]
				? (m_key_down_duration[i] < 0.f
					? 0.f
					: m_key_down_duration[i] + delta_time)
				: -1.f;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}