#include <scene/main/window.hpp>
#include <scene/main/scene_tree.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_OBJECT_CLASS(Window, t) {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Window::Window() : m_window{ DS::MAIN_WINDOW_ID }
	{
	}

	Window::~Window()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	String Window::get_title() const
	{
		return DS::get_singleton()->window_get_title(m_window);
	}

	void Window::set_title(String const & title)
	{
		DS::get_singleton()->window_set_title(title, m_window);
	}

	i32 Window::get_current_screen() const
	{
		return DS::get_singleton()->window_get_current_screen(m_window);
	}

	void Window::set_current_screen(i32 screen)
	{
		DS::get_singleton()->window_set_current_screen(screen, m_window);
	}

	Vec2i Window::get_position() const
	{
		return DS::get_singleton()->window_get_position(m_window);
	}

	void Window::set_position(Vec2i const & position)
	{
		DS::get_singleton()->window_set_position(position, m_window);
	}

	Vec2i Window::get_size() const
	{
		return DS::get_singleton()->window_get_size(m_window);
	}

	void Window::set_size(Vec2i const & size)
	{
		DS::get_singleton()->window_set_size(size, m_window);
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
		DS::get_singleton()->window_set_mode(mode, m_window);
	}

	bool Window::get_flag(i32 flag) const
	{
		return DS::get_singleton()->window_get_flag(flag, m_window);
	}

	void Window::set_flag(i32 flag, bool enabled)
	{
		DS::get_singleton()->window_set_flag(flag, enabled, m_window);
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
		DS::get_singleton()->window_set_visible(value, m_window);
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

	Vec2 Window::get_scale() const
	{
		return DS::get_singleton()->window_get_scale(m_window);
	}

	void Window::grab_focus()
	{
		DS::get_singleton()->window_grab_focus(m_window);
	}

	bool Window::has_focus() const
	{
		return DS::get_singleton()->window_has_focus(m_window);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}
