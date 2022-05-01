#include <scene/main/window.hpp>
#include <scene/main/scene_tree.hpp>
#include <scene/gui/imgui.hpp>

#if SYSTEM_WINDOWS
#include <platform/windows/display_server_windows.hpp>
#define WINDOW_IMPL ism::DisplayServerWindows
#endif

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJECT_EMBED(Window, t)
{
	t.tp_bind = CLASS_BINDER(Window, t)
	{
		return t;
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

Window::Window() noexcept : m_window_id{ DS->get_current_context() }
{
}

Window::~Window()
{
	m_window_id = nullptr;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void Window::make_current_context()
{
	DS->set_current_context(m_window_id);
}

void Window::poll_events()
{
	DS->poll_events();
}

void Window::swap_buffers()
{
	DS->swap_buffers(m_window_id);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void Window::focus()
{
	DS->focus_window(m_window_id);
}

void Window::hide()
{
	DS->hide_window(m_window_id);
}

void Window::iconify()
{
	DS->iconify_window(m_window_id);
}

void Window::maximize()
{
	DS->maximize_window(m_window_id);
}

void Window::restore()
{
	DS->restore_window(m_window_id);
}

void Window::request_attention()
{
	DS->request_window_attention(m_window_id);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

String Window::get_clipboard() const
{
	return DS->window_get_clipboard(m_window_id);
}

Vec2f Window::get_content_scale() const
{
	return DS->window_get_content_scale(m_window_id);
}

IntRect Window::get_frame_size() const
{
	return DS->window_get_frame_size(m_window_id);
}

Vec2i Window::get_framebuffer_size() const
{
	return DS->window_get_framebuffer_size(m_window_id);
}

int32_t Window::get_input_mode(InputMode value) const
{
	return DS->window_get_input_mode(m_window_id, value);
}

InputAction Window::get_key(KeyCode value) const
{
	return DS->window_get_key(m_window_id, value);
}

InputAction Window::get_mouse_button(MouseButton value) const
{
	return DS->window_get_mouse_button(m_window_id, value);
}

Vec2d Window::get_mouse_position() const
{
	return DS->window_get_mouse_position(m_window_id);
}

void * Window::get_native_handle() const
{
	return DS->window_get_native_handle(m_window_id);
}

float_t Window::get_opacity() const
{
	return DS->window_get_opacity(m_window_id);
}

Vec2i Window::get_position() const
{
	return DS->window_get_position(m_window_id);
}

Vec2i Window::get_size() const
{
	return DS->window_get_size(m_window_id);
}

bool Window::get_should_close() const
{
	return DS->window_get_should_close(m_window_id);
}

void * Window::get_user_pointer() const
{
	return DS->window_get_user_pointer(m_window_id);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool Window::is_decorated() const
{
	return DS->window_is_decorated(m_window_id);
}

bool Window::is_floating() const
{
	return DS->window_is_floating(m_window_id);
}

bool Window::is_focused() const
{
	return DS->window_is_focused(m_window_id);
}

bool Window::is_hovered() const
{
	return DS->window_is_hovered(m_window_id);
}

bool Window::is_iconified() const
{
	return DS->window_is_iconified(m_window_id);
}

bool Window::is_maximized() const
{
	return DS->window_is_maximized(m_window_id);
}

bool Window::is_resizable() const
{
	return DS->window_is_resizable(m_window_id);
}

bool Window::is_transparent() const
{
	return DS->window_is_transparent(m_window_id);
}

bool Window::is_visible() const
{
	return DS->window_is_visible(m_window_id);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void Window::set_clipboard(String const & value)
{
	DS->window_set_clipboard(m_window_id, value);
}

void Window::set_cursor(CursorID value)
{
	DS->window_set_cursor(m_window_id, value);
}

void Window::set_input_mode(InputMode value)
{
	DS->window_set_cursor_mode(m_window_id, value);
}

void Window::set_icons(int32_t width, int32_t height, uint8_t * pixels, int32_t count)
{
	DS->window_set_icons(m_window_id, width, height, pixels, count);
}

void Window::set_input_mode(int32_t mode, int32_t value)
{
	DS->window_set_input_mode(m_window_id, mode, value);
}

void Window::set_monitor(MonitorID monitor, IntRect const & bounds)
{
	DS->window_set_monitor(m_window_id, monitor, bounds);
}

void Window::set_mouse_position(Vec2d const & value)
{
	DS->window_set_mouse_position(m_window_id, value);
}

void Window::set_opacity(float_t value)
{
	DS->window_set_opacity(m_window_id, value);
}

void Window::set_position(Vec2i const & value)
{
	DS->window_set_position(m_window_id, value);
}

void Window::set_should_close(bool value)
{
	DS->window_set_should_close(m_window_id, value);
}

void Window::set_size(Vec2i const & value)
{
	DS->window_set_size(m_window_id, value);
}

void Window::set_title(String const & value)
{
	DS->window_set_title(m_window_id, value);
}

void Window::set_user_pointer(void * value)
{
	DS->window_set_user_pointer(m_window_id, value);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void Window::set_is_decorated(bool value)
{
	DS->window_set_is_decorated(m_window_id, value);
}

void Window::set_is_floating(bool value)
{
	DS->window_set_is_floating(m_window_id, value);
}

void Window::set_is_resizable(bool value)
{
	DS->window_set_is_resizable(m_window_id, value);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

WindowCharCallback Window::set_char_callback(WindowCharCallback value)
{
	return DS->window_set_char_callback(m_window_id, value);
}

WindowCharModsCallback Window::set_char_mods_callback(WindowCharModsCallback value)
{
	return DS->window_set_char_mods_callback(m_window_id, value);
}

WindowCloseCallback Window::set_close_callback(WindowCloseCallback value)
{
	return DS->window_set_close_callback(m_window_id, value);
}

WindowContentCallback Window::set_content_scale_callback(WindowContentCallback value)
{
	return DS->window_set_content_scale_callback(m_window_id, value);
}

WindowDropCallback Window::set_drop_callback(WindowDropCallback value)
{
	return DS->window_set_drop_callback(m_window_id, value);
}

WindowFocusCallback Window::set_focus_callback(WindowFocusCallback value)
{
	return DS->window_set_focus_callback(m_window_id, value);
}

WindowFramebufferResizeCallback Window::set_framebuffer_resize_callback(WindowFramebufferResizeCallback value)
{
	return DS->window_set_framebuffer_resize_callback(m_window_id, value);
}

WindowIconifyCallback Window::set_iconify_callback(WindowIconifyCallback value)
{
	return DS->window_set_iconify_callback(m_window_id, value);
}

WindowKeyCallback Window::set_key_callback(WindowKeyCallback value)
{
	return DS->window_set_key_callback(m_window_id, value);
}

WindowMaximizeCallback Window::set_maximize_callback(WindowMaximizeCallback value)
{
	return DS->window_set_maximize_callback(m_window_id, value);
}

WindowMouseButtonCallback Window::set_mouse_button_callback(WindowMouseButtonCallback value)
{
	return DS->window_set_mouse_button_callback(m_window_id, value);
}

WindowMouseEnterCallback Window::set_mouse_enter_callback(WindowMouseEnterCallback value)
{
	return DS->window_set_mouse_enter_callback(m_window_id, value);
}

WindowMousePositionCallback Window::set_mouse_position_callback(WindowMousePositionCallback value)
{
	return DS->window_set_mouse_position_callback(m_window_id, value);
}

WindowPositionCallback Window::set_position_callback(WindowPositionCallback value)
{
	return DS->window_set_position_callback(m_window_id, value);
}

WindowRefreshCallback Window::set_refresh_callback(WindowRefreshCallback value)
{
	return DS->window_set_refresh_callback(m_window_id, value);
}

WindowScrollCallback Window::set_scroll_callback(WindowScrollCallback value)
{
	return DS->window_set_scroll_callback(m_window_id, value);
}

WindowSizeCallback Window::set_size_callback(WindowSizeCallback value)
{
	return DS->window_set_size_callback(m_window_id, value);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */