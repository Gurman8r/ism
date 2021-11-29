#include <scene/main/window.hpp>
#include <scene/main/scene_tree.hpp>
#include <scene/gui/imgui.hpp>

#ifdef SYSTEM_WINDOWS
#include <platform/windows/display_server_windows.hpp>
#define WINDOW_IMPL ism::DisplayServerWindows
#endif

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EMBEDED_CLASS(Window, t) {}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

Window::Window()
{
	m_window_id = SINGLETON(DisplayServer)->get_context_main();
}

Window::~Window()
{
	m_window_id = nullptr;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void Window::make_context_current()
{
	SINGLETON(DisplayServer)->make_context_current(m_window_id);
}

void Window::poll_events()
{
	SINGLETON(DisplayServer)->poll_events();
}

void Window::swap_buffers()
{
	SINGLETON(DisplayServer)->swap_buffers(m_window_id);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void Window::focus()
{
	SINGLETON(DisplayServer)->focus_window(m_window_id);
}

void Window::hide()
{
	SINGLETON(DisplayServer)->hide_window(m_window_id);
}

void Window::iconify()
{
	SINGLETON(DisplayServer)->iconify_window(m_window_id);
}

void Window::maximize()
{
	SINGLETON(DisplayServer)->maximize_window(m_window_id);
}

void Window::restore()
{
	SINGLETON(DisplayServer)->restore_window(m_window_id);
}

void Window::request_attention()
{
	SINGLETON(DisplayServer)->request_window_attention(m_window_id);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

String Window::get_clipboard() const
{
	return SINGLETON(DisplayServer)->window_get_clipboard(m_window_id);
}

Vec2 Window::get_content_scale() const
{
	return SINGLETON(DisplayServer)->window_get_content_scale(m_window_id);
}

Vec2 Window::get_framebuffer_size() const
{
	return SINGLETON(DisplayServer)->window_get_framebuffer_size(m_window_id);
}

int32_t Window::get_input_mode(int32_t value) const
{
	return SINGLETON(DisplayServer)->window_get_input_mode(m_window_id, value);
}

int32_t Window::get_key(int32_t value) const
{
	return SINGLETON(DisplayServer)->window_get_key(m_window_id, value);
}

int32_t Window::get_mouse_button(int32_t value) const
{
	return SINGLETON(DisplayServer)->window_get_mouse_button(m_window_id, value);
}

Vec2 Window::get_mouse_pos() const
{
	return SINGLETON(DisplayServer)->window_get_mouse_pos(m_window_id);
}

WindowID Window::get_native_handle() const
{
	return SINGLETON(DisplayServer)->window_get_native_handle(m_window_id);
}

float_t Window::get_opacity() const
{
	return SINGLETON(DisplayServer)->window_get_opacity(m_window_id);
}

Vec2 Window::get_position() const
{
	return SINGLETON(DisplayServer)->window_get_position(m_window_id);
}

Vec2 Window::get_size() const
{
	return SINGLETON(DisplayServer)->window_get_size(m_window_id);
}

void * Window::get_user_pointer() const
{
	return SINGLETON(DisplayServer)->window_get_user_pointer(m_window_id);
}

Rect Window::get_frame_size() const
{
	return SINGLETON(DisplayServer)->window_get_frame_size(m_window_id);
}

bool Window::get_is_auto_iconify() const
{
	return SINGLETON(DisplayServer)->window_get_is_auto_iconify(m_window_id);
}

bool Window::get_is_decorated() const
{
	return SINGLETON(DisplayServer)->window_get_is_decorated(m_window_id);
}

bool Window::get_is_floating() const
{
	return SINGLETON(DisplayServer)->window_get_is_floating(m_window_id);
}

bool Window::get_is_focused() const
{
	return SINGLETON(DisplayServer)->window_get_is_focused(m_window_id);
}

bool Window::get_is_focus_on_show() const
{
	return SINGLETON(DisplayServer)->window_get_is_focus_on_show(m_window_id);
}

bool Window::get_is_hovered() const
{
	return SINGLETON(DisplayServer)->window_get_is_hovered(m_window_id);
}

bool Window::get_is_iconified() const
{
	return SINGLETON(DisplayServer)->window_get_is_iconified(m_window_id);
}

bool Window::get_is_maximized() const
{
	return SINGLETON(DisplayServer)->window_get_is_maximized(m_window_id);
}

bool Window::get_is_resizable() const
{
	return SINGLETON(DisplayServer)->window_get_is_resizable(m_window_id);
}

bool Window::get_is_transparent() const
{
	return SINGLETON(DisplayServer)->window_get_is_transparent(m_window_id);
}

bool Window::get_is_visible() const
{
	return SINGLETON(DisplayServer)->window_get_is_visible(m_window_id);
}

bool Window::get_should_close() const
{
	return SINGLETON(DisplayServer)->window_get_should_close(m_window_id);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void Window::set_is_auto_iconify(bool value)
{
	SINGLETON(DisplayServer)->window_set_is_auto_iconify(m_window_id, value);
}

void Window::set_clipboard(String const & value)
{
	SINGLETON(DisplayServer)->window_set_clipboard(m_window_id, value);
}

void Window::set_cursor(CursorID value)
{
	SINGLETON(DisplayServer)->window_set_cursor(m_window_id, value);
}

void Window::set_cursor_mode(int32_t value)
{
	SINGLETON(DisplayServer)->window_set_cursor_mode(m_window_id, value);
}

void Window::set_is_decorated(bool value)
{
	SINGLETON(DisplayServer)->window_set_is_decorated(m_window_id, value);
}

void Window::set_is_floating(bool value)
{
	SINGLETON(DisplayServer)->window_set_is_floating(m_window_id, value);
}

void Window::set_is_focus_on_show(bool value)
{
	SINGLETON(DisplayServer)->window_set_is_focus_on_show(m_window_id, value);
}

void Window::set_is_resizable(bool value)
{
	SINGLETON(DisplayServer)->window_set_is_resizable(m_window_id, value);
}

void Window::set_mouse_pos(Vec2 const & value)
{
	SINGLETON(DisplayServer)->window_set_mouse_pos(m_window_id, value);
}

void Window::set_icons(int32_t width, int32_t height, uint8_t * pixels, int32_t count)
{
	SINGLETON(DisplayServer)->window_set_icons(m_window_id, width, height, pixels, count);
}

void Window::set_input_mode(int32_t mode, int32_t value)
{
	SINGLETON(DisplayServer)->window_set_input_mode(m_window_id, mode, value);
}

void Window::set_opacity(float_t value)
{
	SINGLETON(DisplayServer)->window_set_opacity(m_window_id, value);
}

void Window::set_position(Vec2 const & value)
{
	SINGLETON(DisplayServer)->window_set_position(m_window_id, value);
}

void Window::set_monitor(MonitorID monitor, Rect const & bounds)
{
	SINGLETON(DisplayServer)->window_set_monitor(m_window_id, monitor, bounds);
}

void Window::set_should_close(bool value)
{
	SINGLETON(DisplayServer)->window_set_should_close(m_window_id, value);
}

void Window::set_size(Vec2 const & value)
{
	SINGLETON(DisplayServer)->window_set_size(m_window_id, value);
}

void Window::set_title(String const & value)
{
	SINGLETON(DisplayServer)->window_set_title(m_window_id, value);
}

void Window::set_user_pointer(void * value)
{
	SINGLETON(DisplayServer)->window_set_user_pointer(m_window_id, value);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

WindowCharCallback Window::set_char_callback(WindowCharCallback value)
{
	return SINGLETON(DisplayServer)->window_set_char_callback(m_window_id, value);
}

WindowCharModsCallback Window::set_char_mods_callback(WindowCharModsCallback value)
{
	return SINGLETON(DisplayServer)->window_set_char_mods_callback(m_window_id, value);
}

WindowCloseCallback Window::set_close_callback(WindowCloseCallback value)
{
	return SINGLETON(DisplayServer)->window_set_close_callback(m_window_id, value);
}

WindowContentCallback Window::set_content_scale_callback(WindowContentCallback value)
{
	return SINGLETON(DisplayServer)->window_set_content_scale_callback(m_window_id, value);
}

WindowDropCallback Window::set_drop_callback(WindowDropCallback value)
{
	return SINGLETON(DisplayServer)->window_set_drop_callback(m_window_id, value);
}

WindowFocusCallback Window::set_focus_callback(WindowFocusCallback value)
{
	return SINGLETON(DisplayServer)->window_set_focus_callback(m_window_id, value);
}

WindowFramebufferResizeCallback Window::set_framebuffer_resize_callback(WindowFramebufferResizeCallback value)
{
	return SINGLETON(DisplayServer)->window_set_framebuffer_resize_callback(m_window_id, value);
}

WindowIconifyCallback Window::set_iconify_callback(WindowIconifyCallback value)
{
	return SINGLETON(DisplayServer)->window_set_iconify_callback(m_window_id, value);
}

WindowKeyCallback Window::set_key_callback(WindowKeyCallback value)
{
	return SINGLETON(DisplayServer)->window_set_key_callback(m_window_id, value);
}

WindowMaximizeCallback Window::set_maximize_callback(WindowMaximizeCallback value)
{
	return SINGLETON(DisplayServer)->window_set_maximize_callback(m_window_id, value);
}

WindowMouseButtonCallback Window::set_mouse_button_callback(WindowMouseButtonCallback value)
{
	return SINGLETON(DisplayServer)->window_set_mouse_button_callback(m_window_id, value);
}

WindowMouseEnterCallback Window::set_mouse_enter_callback(WindowMouseEnterCallback value)
{
	return SINGLETON(DisplayServer)->window_set_mouse_enter_callback(m_window_id, value);
}

WindowMousePosCallback Window::set_mouse_pos_callback(WindowMousePosCallback value)
{
	return SINGLETON(DisplayServer)->window_set_mouse_pos_callback(m_window_id, value);
}

WindowMouseScrollCallback Window::set_mouse_scroll_callback(WindowMouseScrollCallback value)
{
	return SINGLETON(DisplayServer)->window_set_mouse_scroll_callback(m_window_id, value);
}

WindowPositionCallback Window::set_position_callback(WindowPositionCallback value)
{
	return SINGLETON(DisplayServer)->window_set_position_callback(m_window_id, value);
}

WindowRefreshCallback Window::set_refresh_callback(WindowRefreshCallback value)
{
	return SINGLETON(DisplayServer)->window_set_refresh_callback(m_window_id, value);
}

WindowResizeCallback Window::set_resize_callback(WindowResizeCallback value)
{
	return SINGLETON(DisplayServer)->window_set_resize_callback(m_window_id, value);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */