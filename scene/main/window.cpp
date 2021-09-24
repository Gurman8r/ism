#include <scene/main/window.hpp>

#ifdef ISM_OS_WINDOWS
#include <platform/windows/display_server_windows.hpp>
#define WINDOW_IMPL ism::DisplayServerWindows
#endif

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJ_IMPL(Window, t, TypeFlags_BaseType) {}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

Window * Window::new_(SceneTree * tree, Node * parent, WindowSettings const & settings)
{
	Window * w{ get_display_server().create_window(tree, parent, settings) };

	w->make_context_current();

	w->set_user_pointer(w);

	w->set_position((Vec2(1920, 1080) - settings.video.size) / 2);

	if (w->has_hints(WindowHints_Maximized)) { w->maximize(); }

	return w;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void Window::poll_events()
{
	get_display_server().poll_events();
}

void Window::swap_buffers()
{
	get_display_server().swap_buffers(m_window);
}

void Window::make_context_current()
{
	get_display_server().make_context_current(m_window);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void Window::focus()
{
	get_display_server().focus_window(m_window);
}

void Window::hide()
{
	get_display_server().hide_window(m_window);
}

void Window::iconify()
{
	get_display_server().iconify_window(m_window);
}

void Window::maximize()
{
	get_display_server().maximize_window(m_window);
}

void Window::restore()
{
	get_display_server().restore_window(m_window);
}

void Window::request_attention()
{
	get_display_server().request_window_attention(m_window);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

String Window::get_clipboard() const
{
	return get_display_server().window_get_clipboard(m_window);
}

Vec2 Window::get_content_scale() const
{
	return get_display_server().window_get_content_scale(m_window);
}

Vec2 Window::get_framebuffer_size() const
{
	return get_display_server().window_get_framebuffer_size(m_window);
}

int32_t Window::get_input_mode(int32_t value) const
{
	return get_display_server().window_get_input_mode(m_window, value);
}

int32_t Window::get_key(int32_t value) const
{
	return get_display_server().window_get_key(m_window, value);
}

int32_t Window::get_mouse_button(int32_t value) const
{
	return get_display_server().window_get_mouse_button(m_window, value);
}

Vec2 Window::get_mouse_pos() const
{
	return get_display_server().window_get_mouse_pos(m_window);
}

WindowID Window::get_native_handle() const
{
	return get_display_server().window_get_native_handle(m_window);
}

float_t Window::get_opacity() const
{
	return get_display_server().window_get_opacity(m_window);
}

Vec2 Window::get_position() const
{
	return get_display_server().window_get_position(m_window);
}

Vec2 Window::get_size() const
{
	return get_display_server().window_get_size(m_window);
}

void * Window::get_user_pointer() const
{
	return get_display_server().window_get_user_pointer(m_window);
}

Rect Window::get_frame_size() const
{
	return get_display_server().window_get_frame_size(m_window);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool Window::is_auto_iconify() const
{
	return get_display_server().window_is_auto_iconify(m_window);
}

bool Window::is_decorated() const
{
	return get_display_server().window_is_decorated(m_window);
}

bool Window::is_floating() const
{
	return get_display_server().window_is_floating(m_window);
}

bool Window::is_focused() const
{
	return get_display_server().window_is_focused(m_window);
}

bool Window::is_focus_on_show() const
{
	return get_display_server().window_is_focus_on_show(m_window);
}

bool Window::is_hovered() const
{
	return get_display_server().window_is_hovered(m_window);
}

bool Window::is_iconified() const
{
	return get_display_server().window_is_iconified(m_window);
}

bool Window::is_maximized() const
{
	return get_display_server().window_is_maximized(m_window);
}

bool Window::is_open() const
{
	return get_display_server().window_is_open(m_window);
}

bool Window::is_resizable() const
{
	return get_display_server().window_is_resizable(m_window);
}

bool Window::is_transparent() const
{
	return get_display_server().window_is_transparent(m_window);
}

bool Window::is_visible() const
{
	return get_display_server().window_is_visible(m_window);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void Window::set_auto_iconify(bool value)
{
	get_display_server().window_set_auto_iconify(m_window, value);
}

void Window::set_clipboard(String const & value)
{
	get_display_server().window_set_clipboard(m_window, value);
}

void Window::set_cursor(CursorID value)
{
	get_display_server().window_set_cursor(m_window, value);
}

void Window::set_cursor_mode(int32_t value)
{
	get_display_server().window_set_cursor_mode(m_window, value);
}

void Window::set_mouse_pos(Vec2 const & value)
{
	get_display_server().window_set_mouse_pos(m_window, value);
}

void Window::set_decorated(bool value)
{
	get_display_server().window_set_decorated(m_window, value);
}

void Window::set_floating(bool value)
{
	get_display_server().window_set_floating(m_window, value);
}

void Window::set_focus_on_show(bool value)
{
	get_display_server().window_set_focus_on_show(m_window, value);
}

void Window::set_icons(int32_t width, int32_t height, uint8_t * pixels, int32_t count)
{
	get_display_server().window_set_icons(m_window, width, height, pixels, count);
}

void Window::set_input_mode(int32_t mode, int32_t value)
{
	get_display_server().window_set_input_mode(m_window, mode, value);
}

void Window::set_opacity(float_t value)
{
	get_display_server().window_set_opacity(m_window, value);
}

void Window::set_position(Vec2 const & value)
{
	get_display_server().window_set_position(m_window, value);
}

void Window::set_monitor(MonitorID monitor, Rect const & bounds)
{
	get_display_server().window_set_monitor(m_window, monitor, bounds);
}

void Window::set_resizable(bool value)
{
	get_display_server().window_set_resizable(m_window, value);
}

void Window::set_should_close(bool value)
{
	get_display_server().window_set_should_close(m_window, value);
}

void Window::set_size(Vec2 const & value)
{
	get_display_server().window_set_size(m_window, value);
}

void Window::set_title(String const & value)
{
	get_display_server().window_set_title(m_window, value);
}

void Window::set_user_pointer(void * value)
{
	get_display_server().window_set_user_pointer(m_window, value);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

window_char_callback Window::get_char_callback() const
{
	return get_display_server().window_get_char_callback(m_window);
}

window_char_mods_callback Window::get_char_mods_callback() const
{
	return get_display_server().window_get_char_mods_callback(m_window);
}

window_close_callback Window::get_close_callback() const
{
	return get_display_server().window_get_close_callback(m_window);
}

window_content_scale_callback Window::get_content_scale_callback() const
{
	return get_display_server().window_get_content_scale_callback(m_window);
}

window_mouse_enter_callback Window::get_mouse_enter_callback() const
{
	return get_display_server().window_get_mouse_enter_callback(m_window);
}

window_mouse_pos_callback Window::get_mouse_pos_callback() const
{
	return get_display_server().window_get_mouse_pos_callback(m_window);
}

window_drop_callback Window::get_drop_callback() const
{
	return get_display_server().window_get_drop_callback(m_window);
}

window_focus_callback Window::get_focus_callback() const
{
	return get_display_server().window_get_focus_callback(m_window);
}

window_framebuffer_resize_callback Window::get_framebuffer_resize_callback() const
{
	return get_display_server().window_get_framebuffer_resize_callback(m_window);
}

window_iconify_callback Window::get_iconify_callback() const
{
	return get_display_server().window_get_iconify_callback(m_window);
}

window_key_callback Window::get_key_callback() const
{
	return get_display_server().window_get_key_callback(m_window);
}

window_maximize_callback Window::get_maximize_callback() const
{
	return get_display_server().window_get_maximize_callback(m_window);
}

window_mouse_button_callback Window::get_mouse_button_callback() const
{
	return get_display_server().window_get_mouse_button_callback(m_window);
}

window_position_callback Window::get_position_callback() const
{
	return get_display_server().window_get_position_callback(m_window);
}

window_refresh_callback Window::get_refresh_callback() const
{
	return get_display_server().window_get_refresh_callback(m_window);
}

window_resize_callback Window::get_resize_callback() const
{
	return get_display_server().window_get_resize_callback(m_window);
}

window_scroll_callback Window::get_scroll_callback() const
{
	return get_display_server().window_get_scroll_callback(m_window);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

window_char_callback Window::set_char_callback(window_char_callback value)
{
	return get_display_server().window_set_char_callback(m_window, value);
}

window_char_mods_callback Window::set_char_mods_callback(window_char_mods_callback value)
{
	return get_display_server().window_set_char_mods_callback(m_window, value);
}

window_close_callback Window::set_close_callback(window_close_callback value)
{
	return get_display_server().window_set_close_callback(m_window, value);
}

window_content_scale_callback Window::set_content_scale_callback(window_content_scale_callback value)
{
	return get_display_server().window_set_content_scale_callback(m_window, value);
}

window_mouse_enter_callback Window::set_mouse_enter_callback(window_mouse_enter_callback value)
{
	return get_display_server().window_set_mouse_enter_callback(m_window, value);
}

window_mouse_pos_callback Window::set_mouse_pos_callback(window_mouse_pos_callback value)
{
	return get_display_server().window_set_mouse_pos_callback(m_window, value);
}

window_drop_callback Window::set_drop_callback(window_drop_callback value)
{
	return get_display_server().window_set_drop_callback(m_window, value);
}

window_focus_callback Window::set_focus_callback(window_focus_callback value)
{
	return get_display_server().window_set_focus_callback(m_window, value);
}

window_framebuffer_resize_callback Window::set_framebuffer_resize_callback(window_framebuffer_resize_callback value)
{
	return get_display_server().window_set_framebuffer_resize_callback(m_window, value);
}

window_iconify_callback Window::set_iconify_callback(window_iconify_callback value)
{
	return get_display_server().window_set_iconify_callback(m_window, value);
}

window_key_callback Window::set_key_callback(window_key_callback value)
{
	return get_display_server().window_set_key_callback(m_window, value);
}

window_maximize_callback Window::set_maximize_callback(window_maximize_callback value)
{
	return get_display_server().window_set_maximize_callback(m_window, value);
}

window_mouse_button_callback Window::set_mouse_button_callback(window_mouse_button_callback value)
{
	return get_display_server().window_set_mouse_button_callback(m_window, value);
}

window_position_callback Window::set_position_callback(window_position_callback value)
{
	return get_display_server().window_set_position_callback(m_window, value);
}

window_refresh_callback Window::set_refresh_callback(window_refresh_callback value)
{
	return get_display_server().window_set_refresh_callback(m_window, value);
}

window_resize_callback Window::set_resize_callback(window_resize_callback value)
{
	return get_display_server().window_set_resize_callback(m_window, value);
}

window_scroll_callback Window::set_scroll_callback(window_scroll_callback value)
{
	return get_display_server().window_set_scroll_callback(m_window, value);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */