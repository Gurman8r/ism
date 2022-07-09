#include <scene/main/window.hpp>
#include <scene/main/scene_tree.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OBJECT_EMBED(Window, t) {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Window::Window()
	{
		m_window = DS::get_singleton()->get_current_context();
	}

	Window::~Window()
	{
		m_window = nullptr;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	String Window::get_title() const
	{
		return DS::get_singleton()->window_get_title(m_window);
	}

	void Window::set_title(String const & value)
	{
		DS::get_singleton()->window_set_title(m_window, value);
	}

	DS::MonitorID Window::get_monitor() const
	{
		return DS::get_singleton()->window_get_monitor(m_window);
	}

	void Window::set_monitor(DS::MonitorID value)
	{
		DS::get_singleton()->window_set_monitor(m_window, value);
	}

	Vec2i Window::get_position() const
	{
		return DS::get_singleton()->window_get_position(m_window);
	}

	void Window::set_position(Vec2i const & value)
	{
		DS::get_singleton()->window_set_position(m_window, value);
	}

	Vec2i Window::get_size() const
	{
		return DS::get_singleton()->window_get_size(m_window);
	}

	void Window::set_size(Vec2i const & value)
	{
		DS::get_singleton()->window_set_size(m_window, value);
	}

	Vec2i Window::get_real_size() const
	{
		return DS::get_singleton()->window_get_real_size(m_window);
	}

	DS::WindowMode_ Window::get_mode() const
	{
		return DS::get_singleton()->window_get_mode(m_window);
	}

	void Window::set_mode(DS::WindowMode_ mode)
	{
		DS::get_singleton()->window_set_mode(m_window, mode);
	}

	bool Window::get_flag(int32_t flag) const
	{
		return DS::get_singleton()->window_get_flag(m_window, flag);
	}

	void Window::set_flag(int32_t flag, bool enabled)
	{
		DS::get_singleton()->window_set_flag(m_window, flag, enabled);
	}

	void Window::request_attention()
	{
		DS::get_singleton()->request_window_attention(m_window);
	}

	void Window::move_to_foreground()
	{
		DS::get_singleton()->move_window_to_foreground(m_window);
	}

	void Window::set_visible(bool value)
	{
		DS::get_singleton()->window_set_visible(m_window, value);
	}

	bool Window::is_visible() const
	{
		return DS::get_singleton()->window_is_visible(m_window);
	}

	void Window::hide()
	{
		set_visible(false);
	}

	void Window::show()
	{
		set_visible(true);
	}

	Vec2 Window::get_content_scale() const
	{
		return DS::get_singleton()->window_get_content_scale(m_window);
	}

	void Window::grab_focus()
	{
		DS::get_singleton()->window_grab_focus(m_window);
	}

	bool Window::has_focus() const
	{
		return DS::get_singleton()->window_has_focus(m_window);
	}

	bool Window::should_close() const
	{
		return DS::get_singleton()->window_should_close(m_window);
	}

	void Window::set_should_close(bool value)
	{
		DS::get_singleton()->window_set_should_close(m_window, value);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}
