#include <core/input/input.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Input * Input::__singleton{};

	Input::Funcs Input::m_funcs{};

	EMBED_CLASS(Input, t) {}

	Input::Input() { __singleton = this; }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Input::is_key_down(i32 key) const
	{
		VERIFY_RANGE(key, -1, Key_MAX);
		return m_key_down[key];
	}

	bool Input::is_mouse_button_down(i32 button) const
	{
		VERIFY_RANGE(button, -1, MouseButton_MAX);
		return m_mouse_down[button];
	}

	bool Input::is_joy_button_down(i32 device, i32 button) const
	{
		VERIFY_RANGE(device, -1, Joy_MAX);
		VERIFY_RANGE(button, -1, JoyButton_MAX);
		return m_joy_down[device][button];
	}

	bool Input::is_nav_input_down(i32 nav_input) const
	{
		VERIFY_RANGE(nav_input, -1, NavInput_MAX);
		return m_nav_input_down[nav_input];
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Input::is_any_key_down() const
	{
		for (i32 key = 0; key < Key_MAX; ++key) {
			if (m_key_down[key]) {
				return true;
			}
		}
		return false;
	}

	bool Input::is_any_mouse_button_down() const
	{
		for (i32 button = 0; button < MouseButton_MAX; ++button) {
			if (m_mouse_down[button]) {
				return true;
			}
		}
		return false;
	}

	bool Input::is_any_joy_button_down(i32 device) const
	{
		if (is_joy_known(device)) {
			for (i32 button = 0; button < JoyButton_MAX; ++button) {
				if (m_joy_down[device][button]) {
					return true;
				}
			}
		}
		return false;
	}

	bool Input::is_any_nav_input_down() const
	{
		for (i32 nav_input = 0; nav_input < NavInput_MAX; ++nav_input) {
			if (m_nav_input_down[nav_input]) {
				return true;
			}
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	f32 Input::get_key_down_duration(i32 key) const
	{
		VERIFY_RANGE(key, -1, Key_MAX);
		return m_key_down_duration[key];
	}

	f32 Input::get_mouse_button_down_duration(i32 button) const
	{
		VERIFY_RANGE(button, -1, MouseButton_MAX);
		return m_mouse_down_duration[button];
	}

	f32 Input::get_joy_button_down_duration(i32 device, i32 button) const
	{
		VERIFY_RANGE(device, -1, Joy_MAX);
		VERIFY_RANGE(button, -1, JoyButton_MAX);
		return m_joy_down_duration[device][button];
	}

	f32 Input::get_nav_input_down_duration(i32 nav_input) const
	{
		VERIFY_RANGE(nav_input, -1, NavInput_MAX);
		return m_nav_input_down_duration[nav_input];
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Input::is_key_past_threshold(i32 key, f32 threshold) const
	{
		VERIFY_RANGE(key, -1, Key_MAX);
		if (threshold < 0.f) { threshold = 0.1f; }
		return m_key_down_duration[key] >= threshold  * threshold;
	}

	bool Input::is_mouse_button_past_threshold(i32 button, f32 threshold) const
	{
		VERIFY_RANGE(button, -1, MouseButton_MAX);
		if (threshold < 0.f) { threshold = 0.1f; }
		return m_mouse_down_duration[button] >= threshold * threshold;
	}

	bool Input::is_joy_button_past_threshold(i32 device, i32 button, f32 threshold) const
	{
		VERIFY_RANGE(device, -1, Joy_MAX);
		VERIFY_RANGE(button, -1, JoyButton_MAX);
		if (threshold < 0.f) { threshold = 0.1f; }
		return m_joy_down_duration[device][button] >= threshold * threshold;
	}

	bool Input::is_nav_input_past_threshold(i32 nav_input, f32 threshold) const
	{
		VERIFY_RANGE(nav_input, -1, NavInput_MAX);
		if (threshold < 0.f) { threshold = 0.1f; }
		return m_nav_input_down_duration[nav_input] >= threshold * threshold;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Input::set_key(i32 key, bool pressed)
	{
		VERIFY_RANGE(key, -1, Key_MAX);
		m_key_down[key] = pressed;
		m_is_shift = m_key_down[Input::Key_LeftShift] || m_key_down[Input::Key_RightShift];
		m_is_ctrl = m_key_down[Input::Key_LeftCtrl] || m_key_down[Input::Key_RightCtrl];
		m_is_alt = m_key_down[Input::Key_LeftAlt] || m_key_down[Input::Key_RightAlt];
		m_is_super = m_key_down[Input::Key_LeftSuper] || m_key_down[Input::Key_RightSuper];
	}

	void Input::set_mouse_button(i32 button, bool pressed)
	{
		VERIFY_RANGE(button, -1, MouseButton_MAX);
		m_mouse_down[button] = pressed;
	}

	void Input::set_joy_button(i32 device, i32 button, bool pressed)
	{
		VERIFY_RANGE(device, -1, Joy_MAX);
		VERIFY_RANGE(button, -1, JoyButton_MAX);
		m_joy_down[device][button] = pressed;
	}

	void Input::set_nav_input(i32 nav_input, bool pressed)
	{
		VERIFY_RANGE(nav_input, -1, NavInput_MAX);
		m_nav_input_down[nav_input] = pressed;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Vector<Input::Joy_> Input::get_connected_joys() const
	{
		Vector<Joy_> out{};
		for (i32 device = 0; device < Joy_MAX; ++device)
		{
			if (m_joy_known[device]) {
				out.push_back((Joy_)device);
			}
		}
		return out;
	}

	Input::Joy_ Input::get_unused_joy_id()
	{
		return { /* TODO */ };
	}

	bool Input::is_joy_known(i32 device) const
	{
		return ((u32)device < (u32)Joy_MAX) && m_joy_known[device];
	}

	String Input::get_joy_name(i32 device) const
	{
		VERIFY_RANGE(device, -1, Joy_MAX);
		return m_joy_name[device];
	}

	String Input::get_joy_guid(i32 device) const
	{
		VERIFY_RANGE(device, -1, Joy_MAX);
		return m_joy_guid[device];
	}

	f32 Input::get_joy_axis(i32 device, i32 axis) const
	{
		VERIFY_RANGE(device, -1, Joy_MAX);
		VERIFY_RANGE(axis, -1, JoyAxis_MAX);
		return m_joy_axes[device][axis];
	}

	void Input::set_joy_axis(i32 device, i32 axis, f32 value)
	{
		VERIFY_RANGE(device, -1, Joy_MAX);
		VERIFY_RANGE(axis, -1, JoyAxis_MAX);
		m_joy_axes[device][axis] = value;
	}

	void Input::joy_connection_changed(i32 device, bool connected, String const & name, String const & guid)
	{
		VERIFY_RANGE(device, -1, Joy_MAX);
		m_joy_known[device] = connected;

		if (connected)
		{
			m_joy_name[device] = name;
			m_joy_guid[device] = guid ? guid : name;
		}
		else
		{
			m_joy_name[device] = ""_s;
			m_joy_guid[device] = ""_s;

			for (i32 button = 0; button < JoyButton_MAX; ++button)
			{
				m_joy_down[device][button] = false;
				m_joy_down_duration[device][button] = 0_s;
			}

			for (i32 axis = 0; axis < JoyAxis_MAX; ++axis)
			{
				m_joy_axes[device][axis] = 0.f;
			}
		}
	}

	bool Input::parse_mapping(String const & mapping)
	{
		// TODO
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Input::MouseMode_ Input::get_mouse_mode() const
	{
		ASSERT(m_funcs.get_mouse_mode);
		return m_funcs.get_mouse_mode();
	}

	void Input::set_mouse_mode(MouseMode_ mode)
	{
		ASSERT(m_funcs.set_mouse_mode);
		m_funcs.set_mouse_mode(mode);
	}

	Vec2 Input::get_mouse_position() const
	{
		return m_mouse_position;
	}

	void Input::set_mouse_position(Vec2 const & position)
	{
		m_mouse_position = position;

		if (m_funcs.set_mouse_position) {
			m_funcs.set_mouse_position(position);
		}
	}

	Vec2 Input::get_mouse_delta() const
	{
		return m_mouse_delta;
	}

	Vec2 Input::get_last_mouse_delta() const
	{
		return m_mouse_delta_prev;
	}

	bool Input::is_mouse_dragging(i32 button, f32 threshold) const
	{
		VERIFY_RANGE(button, -1, MouseButton_MAX);
		return m_mouse_down[button]
			&& is_mouse_button_past_threshold(button, threshold);
	}

	Vec2 Input::get_mouse_wheel() const
	{
		return m_mouse_wheel;
	}

	void Input::set_mouse_wheel(Vec2 const & wheel)
	{
		m_mouse_wheel = wheel;
	}

	Input::CursorShape_ Input::get_cursor_shape() const
	{
		ASSERT(m_funcs.get_cursor_shape);
		return m_funcs.get_cursor_shape();
	}

	void Input::set_cursor_shape(CursorShape_ shape)
	{
		VERIFY_RANGE(shape, -1, CursorShape_MAX);
		ASSERT(m_funcs.set_cursor_shape);
		m_funcs.set_cursor_shape(shape);
	}

	void Input::set_custom_cursor_image(RES const & cursor, CursorShape_ shape, Vec2 const & hotspot)
	{
		VERIFY_RANGE(shape, -1, CursorShape_MAX);
		ASSERT(m_funcs.set_custom_cursor_image);
		m_funcs.set_custom_cursor_image(cursor, shape, hotspot);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Input::iteration(Duration const delta_time)
	{
		/* UPDATE MOUSE */

		// round mouse position
		if (is_valid_mouse_position(&m_mouse_position)) {
			m_mouse_position = m_last_valid_mouse_position = {
				floorf(m_mouse_position[0]), floorf(m_mouse_position[1])
			};
		}

		m_mouse_delta_prev = m_mouse_delta;

		if (is_valid_mouse_position(&m_mouse_position) && is_valid_mouse_position(&m_mouse_position_prev)) {
			m_mouse_delta = m_mouse_position - m_mouse_position_prev;
		}
		else {
			m_mouse_delta = {};
		}

		m_mouse_position_prev = m_mouse_position;

		for (i32 button = 0; button < MouseButton_MAX; ++button)
		{
			m_mouse_double_clicked[button] = false;

			m_mouse_down_duration_prev[button] = m_mouse_down_duration[button];

			m_mouse_down_duration[button] = m_mouse_down[button]
				? (m_mouse_down_duration[button] < 0.f ? 0.f : m_mouse_down_duration[button] + delta_time)
				: -1.f;
		}

		/* UPDATE KEYBOARD */

		for (i32 key = 0; key < Key_MAX; ++key)
		{
			m_key_down_duration_prev[key] = m_key_down_duration[key];

			m_key_down_duration[key] = m_key_down[key]
				? (m_key_down_duration[key] < 0.f ? 0.f : m_key_down_duration[key] + delta_time)
				: -1.f;
		}

		/* UPDATE JOYSTICKS */

		for (i32 device = 0; device < Joy_MAX; ++device)
		{
			if (!m_joy_known[device]) { continue; }

			for (i32 button = 0; button < JoyButton_MAX; ++button)
			{
				m_joy_down_duration_prev[device][button] = m_joy_down_duration[device][button];

				m_joy_down_duration[device][button] = m_joy_down[device][button]
					? (m_joy_down_duration[device][button] < 0.f ? 0.f : m_joy_down_duration[device][button] + delta_time)
					: -1.f;
			}
		}

		/* UPDATE NAV INPUTS */

		for (i32 nav_input = 0; nav_input < NavInput_MAX; ++nav_input)
		{
			m_nav_input_down_duration_prev[nav_input] = m_nav_input_down_duration[nav_input];

			m_nav_input_down_duration[nav_input] = m_nav_input_down[nav_input]
				? (m_nav_input_down_duration[nav_input] < 0.f ? 0.f : m_nav_input_down_duration[nav_input] + delta_time)
				: -1.f;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}