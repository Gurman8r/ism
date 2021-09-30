#include <scene/main/window.hpp>

#ifdef SYSTEM_WINDOWS
#include <platform/windows/display_server_windows.hpp>
#define WINDOW_IMPL ism::DisplayServerWindows
#endif

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJECT_EMBED(Window, t, TypeFlags_BaseType) {}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

Window * Window::new_(WindowSettings const & settings)
{
	return get_display_server().window_new(settings);
}

void Window::make_context_current()
{
	get_display_server().make_context_current(m_window_id);
}

void Window::poll_events()
{
	get_display_server().poll_events();
}

void Window::swap_buffers()
{
	get_display_server().swap_buffers(m_window_id);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void Window::focus()
{
	get_display_server().focus_window(m_window_id);
}

void Window::hide()
{
	get_display_server().hide_window(m_window_id);
}

void Window::iconify()
{
	get_display_server().iconify_window(m_window_id);
}

void Window::maximize()
{
	get_display_server().maximize_window(m_window_id);
}

void Window::restore()
{
	get_display_server().restore_window(m_window_id);
}

void Window::request_attention()
{
	get_display_server().request_window_attention(m_window_id);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

String Window::get_clipboard() const
{
	return get_display_server().window_get_clipboard(m_window_id);
}

Vec2 Window::get_content_scale() const
{
	return get_display_server().window_get_content_scale(m_window_id);
}

Vec2 Window::get_framebuffer_size() const
{
	return get_display_server().window_get_framebuffer_size(m_window_id);
}

int32_t Window::get_input_mode(int32_t value) const
{
	return get_display_server().window_get_input_mode(m_window_id, value);
}

int32_t Window::get_key(int32_t value) const
{
	return get_display_server().window_get_key(m_window_id, value);
}

int32_t Window::get_mouse_button(int32_t value) const
{
	return get_display_server().window_get_mouse_button(m_window_id, value);
}

Vec2 Window::get_mouse_pos() const
{
	return get_display_server().window_get_mouse_pos(m_window_id);
}

WindowID Window::get_native_handle() const
{
	return get_display_server().window_get_native_handle(m_window_id);
}

float_t Window::get_opacity() const
{
	return get_display_server().window_get_opacity(m_window_id);
}

Vec2 Window::get_position() const
{
	return get_display_server().window_get_position(m_window_id);
}

Vec2 Window::get_size() const
{
	return get_display_server().window_get_size(m_window_id);
}

void * Window::get_user_pointer() const
{
	return get_display_server().window_get_user_pointer(m_window_id);
}

Rect Window::get_frame_size() const
{
	return get_display_server().window_get_frame_size(m_window_id);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool Window::is_auto_iconify() const
{
	return get_display_server().window_is_auto_iconify(m_window_id);
}

bool Window::is_decorated() const
{
	return get_display_server().window_is_decorated(m_window_id);
}

bool Window::is_floating() const
{
	return get_display_server().window_is_floating(m_window_id);
}

bool Window::is_focused() const
{
	return get_display_server().window_is_focused(m_window_id);
}

bool Window::is_focus_on_show() const
{
	return get_display_server().window_is_focus_on_show(m_window_id);
}

bool Window::is_hovered() const
{
	return get_display_server().window_is_hovered(m_window_id);
}

bool Window::is_iconified() const
{
	return get_display_server().window_is_iconified(m_window_id);
}

bool Window::is_maximized() const
{
	return get_display_server().window_is_maximized(m_window_id);
}

bool Window::is_open() const
{
	return get_display_server().window_is_open(m_window_id);
}

bool Window::is_resizable() const
{
	return get_display_server().window_is_resizable(m_window_id);
}

bool Window::is_transparent() const
{
	return get_display_server().window_is_transparent(m_window_id);
}

bool Window::is_visible() const
{
	return get_display_server().window_is_visible(m_window_id);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void Window::set_auto_iconify(bool value)
{
	get_display_server().window_set_auto_iconify(m_window_id, value);
}

void Window::set_clipboard(String const & value)
{
	get_display_server().window_set_clipboard(m_window_id, value);
}

void Window::set_cursor(CursorID value)
{
	get_display_server().window_set_cursor(m_window_id, value);
}

void Window::set_cursor_mode(int32_t value)
{
	get_display_server().window_set_cursor_mode(m_window_id, value);
}

void Window::set_mouse_pos(Vec2 const & value)
{
	get_display_server().window_set_mouse_pos(m_window_id, value);
}

void Window::set_decorated(bool value)
{
	get_display_server().window_set_decorated(m_window_id, value);
}

void Window::set_floating(bool value)
{
	get_display_server().window_set_floating(m_window_id, value);
}

void Window::set_focus_on_show(bool value)
{
	get_display_server().window_set_focus_on_show(m_window_id, value);
}

void Window::set_icons(int32_t width, int32_t height, uint8_t * pixels, int32_t count)
{
	get_display_server().window_set_icons(m_window_id, width, height, pixels, count);
}

void Window::set_input_mode(int32_t mode, int32_t value)
{
	get_display_server().window_set_input_mode(m_window_id, mode, value);
}

void Window::set_opacity(float_t value)
{
	get_display_server().window_set_opacity(m_window_id, value);
}

void Window::set_position(Vec2 const & value)
{
	get_display_server().window_set_position(m_window_id, value);
}

void Window::set_monitor(MonitorID monitor, Rect const & bounds)
{
	get_display_server().window_set_monitor(m_window_id, monitor, bounds);
}

void Window::set_resizable(bool value)
{
	get_display_server().window_set_resizable(m_window_id, value);
}

void Window::set_should_close(bool value)
{
	get_display_server().window_set_should_close(m_window_id, value);
}

void Window::set_size(Vec2 const & value)
{
	get_display_server().window_set_size(m_window_id, value);
}

void Window::set_title(String const & value)
{
	get_display_server().window_set_title(m_window_id, value);
}

void Window::set_user_pointer(void * value)
{
	get_display_server().window_set_user_pointer(m_window_id, value);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

WindowCharCallback Window::set_char_callback(WindowCharCallback value)
{
	return get_display_server().window_set_char_callback(m_window_id, value);
}

WindowCharModsCallback Window::set_char_mods_callback(WindowCharModsCallback value)
{
	return get_display_server().window_set_char_mods_callback(m_window_id, value);
}

WindowCloseCallback Window::set_close_callback(WindowCloseCallback value)
{
	return get_display_server().window_set_close_callback(m_window_id, value);
}

WindowContentCallback Window::set_content_scale_callback(WindowContentCallback value)
{
	return get_display_server().window_set_content_scale_callback(m_window_id, value);
}

WindowDropCallback Window::set_drop_callback(WindowDropCallback value)
{
	return get_display_server().window_set_drop_callback(m_window_id, value);
}

WindowFocusCallback Window::set_focus_callback(WindowFocusCallback value)
{
	return get_display_server().window_set_focus_callback(m_window_id, value);
}

WindowFramebufferResizeCallback Window::set_framebuffer_resize_callback(WindowFramebufferResizeCallback value)
{
	return get_display_server().window_set_framebuffer_resize_callback(m_window_id, value);
}

WindowIconifyCallback Window::set_iconify_callback(WindowIconifyCallback value)
{
	return get_display_server().window_set_iconify_callback(m_window_id, value);
}

WindowKeyCallback Window::set_key_callback(WindowKeyCallback value)
{
	return get_display_server().window_set_key_callback(m_window_id, value);
}

WindowMaximizeCallback Window::set_maximize_callback(WindowMaximizeCallback value)
{
	return get_display_server().window_set_maximize_callback(m_window_id, value);
}

WindowMouseButtonCallback Window::set_mouse_button_callback(WindowMouseButtonCallback value)
{
	return get_display_server().window_set_mouse_button_callback(m_window_id, value);
}

WindowMouseEnterCallback Window::set_mouse_enter_callback(WindowMouseEnterCallback value)
{
	return get_display_server().window_set_mouse_enter_callback(m_window_id, value);
}

WindowMousePosCallback Window::set_mouse_pos_callback(WindowMousePosCallback value)
{
	return get_display_server().window_set_mouse_pos_callback(m_window_id, value);
}

WindowMouseScrollCallback Window::set_mouse_scroll_callback(WindowMouseScrollCallback value)
{
	return get_display_server().window_set_mouse_scroll_callback(m_window_id, value);
}

WindowPositionCallback Window::set_position_callback(WindowPositionCallback value)
{
	return get_display_server().window_set_position_callback(m_window_id, value);
}

WindowRefreshCallback Window::set_refresh_callback(WindowRefreshCallback value)
{
	return get_display_server().window_set_refresh_callback(m_window_id, value);
}

WindowResizeCallback Window::set_resize_callback(WindowResizeCallback value)
{
	return get_display_server().window_set_resize_callback(m_window_id, value);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */