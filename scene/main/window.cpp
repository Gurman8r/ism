#include <scene/main/window.hpp>
#include <scene/main/scene_tree.hpp>
#include <scene/gui/imgui.hpp>

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

	void Window::make_current_context()
	{
		DISPLAY_SERVER->set_current_context(m_window);
	}

	void Window::poll_events()
	{
		DISPLAY_SERVER->poll_events();
	}

	void Window::swap_buffers()
	{
		DISPLAY_SERVER->swap_buffers(m_window);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Window::focus()
	{
		DISPLAY_SERVER->focus_window(m_window);
	}

	void Window::hide()
	{
		DISPLAY_SERVER->hide_window(m_window);
	}

	void Window::iconify()
	{
		DISPLAY_SERVER->iconify_window(m_window);
	}

	void Window::maximize()
	{
		DISPLAY_SERVER->maximize_window(m_window);
	}

	void Window::restore()
	{
		DISPLAY_SERVER->restore_window(m_window);
	}

	void Window::request_attention()
	{
		DISPLAY_SERVER->request_window_attention(m_window);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	String Window::get_clipboard() const
	{
		return DISPLAY_SERVER->window_get_clipboard(m_window);
	}

	Vec2f Window::get_content_scale() const
	{
		return DISPLAY_SERVER->window_get_content_scale(m_window);
	}

	IntRect Window::get_frame_size() const
	{
		return DISPLAY_SERVER->window_get_frame_size(m_window);
	}

	Vec2i Window::get_framebuffer_size() const
	{
		return DISPLAY_SERVER->window_get_framebuffer_size(m_window);
	}

	int32_t Window::get_input_mode(InputMode_ value) const
	{
		return DISPLAY_SERVER->window_get_input_mode(m_window, value);
	}

	InputAction_ Window::get_key(KeyCode_ value) const
	{
		return DISPLAY_SERVER->window_get_key(m_window, value);
	}

	InputAction_ Window::get_mouse_button(MouseButton_ value) const
	{
		return DISPLAY_SERVER->window_get_mouse_button(m_window, value);
	}

	Vec2d Window::get_mouse_position() const
	{
		return DISPLAY_SERVER->window_get_mouse_position(m_window);
	}

	void * Window::get_native_handle() const
	{
		return DISPLAY_SERVER->window_get_native_handle(m_window);
	}

	float_t Window::get_opacity() const
	{
		return DISPLAY_SERVER->window_get_opacity(m_window);
	}

	Vec2i Window::get_position() const
	{
		return DISPLAY_SERVER->window_get_position(m_window);
	}

	Vec2i Window::get_size() const
	{
		return DISPLAY_SERVER->window_get_size(m_window);
	}

	bool Window::get_should_close() const
	{
		return DISPLAY_SERVER->window_get_should_close(m_window);
	}

	void * Window::get_user_pointer() const
	{
		return DISPLAY_SERVER->window_get_user_pointer(m_window);
	}

	bool Window::is_decorated() const
	{
		return DISPLAY_SERVER->window_is_decorated(m_window);
	}

	bool Window::is_floating() const
	{
		return DISPLAY_SERVER->window_is_floating(m_window);
	}

	bool Window::is_focused() const
	{
		return DISPLAY_SERVER->window_is_focused(m_window);
	}

	bool Window::is_hovered() const
	{
		return DISPLAY_SERVER->window_is_hovered(m_window);
	}

	bool Window::is_iconified() const
	{
		return DISPLAY_SERVER->window_is_iconified(m_window);
	}

	bool Window::is_maximized() const
	{
		return DISPLAY_SERVER->window_is_maximized(m_window);
	}

	bool Window::is_resizable() const
	{
		return DISPLAY_SERVER->window_is_resizable(m_window);
	}

	bool Window::is_transparent() const
	{
		return DISPLAY_SERVER->window_is_transparent(m_window);
	}

	bool Window::is_visible() const
	{
		return DISPLAY_SERVER->window_is_visible(m_window);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Window::set_clipboard(String const & value)
	{
		DISPLAY_SERVER->window_set_clipboard(m_window, value);
	}

	void Window::set_cursor(CursorID value)
	{
		DISPLAY_SERVER->window_set_cursor(m_window, value);
	}

	void Window::set_input_mode(InputMode_ value)
	{
		DISPLAY_SERVER->window_set_cursor_mode(m_window, value);
	}

	void Window::set_icons(int32_t width, int32_t height, uint8_t * pixels, int32_t count)
	{
		DISPLAY_SERVER->window_set_icons(m_window, width, height, pixels, count);
	}

	void Window::set_input_mode(int32_t mode, int32_t value)
	{
		DISPLAY_SERVER->window_set_input_mode(m_window, mode, value);
	}

	void Window::set_monitor(MonitorID monitor, IntRect const & bounds)
	{
		DISPLAY_SERVER->window_set_monitor(m_window, monitor, bounds);
	}

	void Window::set_mouse_position(Vec2d const & value)
	{
		DISPLAY_SERVER->window_set_mouse_position(m_window, value);
	}

	void Window::set_opacity(float_t value)
	{
		DISPLAY_SERVER->window_set_opacity(m_window, value);
	}

	void Window::set_position(Vec2i const & value)
	{
		DISPLAY_SERVER->window_set_position(m_window, value);
	}

	void Window::set_should_close(bool value)
	{
		DISPLAY_SERVER->window_set_should_close(m_window, value);
	}

	void Window::set_size(Vec2i const & value)
	{
		DISPLAY_SERVER->window_set_size(m_window, value);
	}

	void Window::set_title(String const & value)
	{
		DISPLAY_SERVER->window_set_title(m_window, value);
	}

	void Window::set_user_pointer(void * value)
	{
		DISPLAY_SERVER->window_set_user_pointer(m_window, value);
	}

	void Window::set_decorated(bool value)
	{
		DISPLAY_SERVER->window_set_decorated(m_window, value);
	}

	void Window::set_floating(bool value)
	{
		DISPLAY_SERVER->window_set_floating(m_window, value);
	}

	void Window::set_resizable(bool value)
	{
		DISPLAY_SERVER->window_set_resizable(m_window, value);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	WindowCharCallback Window::set_char_callback(WindowCharCallback value)
	{
		return DISPLAY_SERVER->window_set_char_callback(m_window, value);
	}

	WindowCharModsCallback Window::set_char_mods_callback(WindowCharModsCallback value)
	{
		return DISPLAY_SERVER->window_set_char_mods_callback(m_window, value);
	}

	WindowCloseCallback Window::set_close_callback(WindowCloseCallback value)
	{
		return DISPLAY_SERVER->window_set_close_callback(m_window, value);
	}

	WindowContentCallback Window::set_content_scale_callback(WindowContentCallback value)
	{
		return DISPLAY_SERVER->window_set_content_scale_callback(m_window, value);
	}

	WindowDropCallback Window::set_drop_callback(WindowDropCallback value)
	{
		return DISPLAY_SERVER->window_set_drop_callback(m_window, value);
	}

	WindowFocusCallback Window::set_focus_callback(WindowFocusCallback value)
	{
		return DISPLAY_SERVER->window_set_focus_callback(m_window, value);
	}

	WindowFramebufferResizeCallback Window::set_framebuffer_resize_callback(WindowFramebufferResizeCallback value)
	{
		return DISPLAY_SERVER->window_set_framebuffer_resize_callback(m_window, value);
	}

	WindowIconifyCallback Window::set_iconify_callback(WindowIconifyCallback value)
	{
		return DISPLAY_SERVER->window_set_iconify_callback(m_window, value);
	}

	WindowKeyCallback Window::set_key_callback(WindowKeyCallback value)
	{
		return DISPLAY_SERVER->window_set_key_callback(m_window, value);
	}

	WindowMaximizeCallback Window::set_maximize_callback(WindowMaximizeCallback value)
	{
		return DISPLAY_SERVER->window_set_maximize_callback(m_window, value);
	}

	WindowMouseButtonCallback Window::set_mouse_button_callback(WindowMouseButtonCallback value)
	{
		return DISPLAY_SERVER->window_set_mouse_button_callback(m_window, value);
	}

	WindowMouseEnterCallback Window::set_mouse_enter_callback(WindowMouseEnterCallback value)
	{
		return DISPLAY_SERVER->window_set_mouse_enter_callback(m_window, value);
	}

	WindowMousePositionCallback Window::set_mouse_position_callback(WindowMousePositionCallback value)
	{
		return DISPLAY_SERVER->window_set_mouse_position_callback(m_window, value);
	}

	WindowPositionCallback Window::set_position_callback(WindowPositionCallback value)
	{
		return DISPLAY_SERVER->window_set_position_callback(m_window, value);
	}

	WindowRefreshCallback Window::set_refresh_callback(WindowRefreshCallback value)
	{
		return DISPLAY_SERVER->window_set_refresh_callback(m_window, value);
	}

	WindowScrollCallback Window::set_scroll_callback(WindowScrollCallback value)
	{
		return DISPLAY_SERVER->window_set_scroll_callback(m_window, value);
	}

	WindowSizeCallback Window::set_size_callback(WindowSizeCallback value)
	{
		return DISPLAY_SERVER->window_set_size_callback(m_window, value);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}
