#include <scene/main/window.hpp>
#include <scene/main/scene_tree.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OBJECT_EMBED(Window, t) {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Window::Window()
		: Viewport{}
		, m_window{ DISPLAY_SERVER->get_current_context() }
	{
	}

	Window::~Window()
	{
		m_window = nullptr;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	String Window::get_title() const
	{
		return DISPLAY_SERVER->window_get_title(m_window);
	}

	void Window::set_title(String const & value)
	{
		DISPLAY_SERVER->window_set_title(m_window, value);
	}

	DS::MonitorID Window::get_monitor() const
	{
		return DISPLAY_SERVER->window_get_monitor(m_window);
	}

	void Window::set_monitor(DS::MonitorID value, IntRect const & rect, int32_t refresh_rate)
	{
		DISPLAY_SERVER->window_set_monitor(m_window, value, rect, refresh_rate);
	}

	Vec2i Window::get_position() const
	{
		return DISPLAY_SERVER->window_get_position(m_window);
	}

	void Window::set_position(Vec2i const & value)
	{
		DISPLAY_SERVER->window_set_position(m_window, value);
	}

	Vec2i Window::get_size() const
	{
		return DISPLAY_SERVER->window_get_size(m_window);
	}

	void Window::set_size(Vec2i const & value)
	{
		DISPLAY_SERVER->window_set_size(m_window, value);
	}

	DS::WindowMode_ Window::get_mode() const
	{
		return DISPLAY_SERVER->window_get_mode(m_window);
	}

	void Window::set_mode(DS::WindowMode_ mode)
	{
		DISPLAY_SERVER->window_set_mode(m_window, mode);
	}

	Vec2i Window::get_real_size() const
	{
		return DISPLAY_SERVER->window_get_real_size(m_window);
	}

	bool Window::get_flag(int32_t flag) const
	{
		return DISPLAY_SERVER->window_get_flag(m_window, flag);
	}

	void Window::set_flag(int32_t flag, bool enabled)
	{
		DISPLAY_SERVER->window_set_flag(m_window, flag, enabled);
	}

	void Window::request_attention()
	{
		DISPLAY_SERVER->request_window_attention(m_window);
	}

	void Window::move_to_foreground()
	{
		DISPLAY_SERVER->move_window_to_foreground(m_window);
	}

	void Window::set_visible(bool value)
	{
		DISPLAY_SERVER->window_set_visible(m_window, value);
	}

	bool Window::is_visible() const
	{
		return DISPLAY_SERVER->window_is_visible(m_window);
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
		return DISPLAY_SERVER->window_get_content_scale(m_window);
	}

	void Window::grab_focus()
	{
		DISPLAY_SERVER->window_grab_focus(m_window);
	}

	bool Window::has_focus() const
	{
		return DISPLAY_SERVER->window_has_focus(m_window);
	}

	bool Window::should_close() const
	{
		return DISPLAY_SERVER->window_should_close(m_window);
	}

	void Window::set_should_close(bool value)
	{
		DISPLAY_SERVER->window_set_should_close(m_window, value);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}
