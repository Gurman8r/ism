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
	t.tp_install = CLASS_INSTALLER(Window, t)
	{
		return t;
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

Window::Window() noexcept : m_window_id{ DISPLAY_SERVER->get_current_context() }
{
}

Window::~Window()
{
	m_window_id = nullptr;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void Window::make_current_context()
{
	DISPLAY_SERVER->set_current_context(m_window_id);
}

void Window::poll_events()
{
	DISPLAY_SERVER->poll_events();
}

void Window::swap_buffers()
{
	DISPLAY_SERVER->swap_buffers(m_window_id);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void Window::focus()
{
	DISPLAY_SERVER->focus_window(m_window_id);
}

void Window::hide()
{
	DISPLAY_SERVER->hide_window(m_window_id);
}

void Window::iconify()
{
	DISPLAY_SERVER->iconify_window(m_window_id);
}

void Window::maximize()
{
	DISPLAY_SERVER->maximize_window(m_window_id);
}

void Window::restore()
{
	DISPLAY_SERVER->restore_window(m_window_id);
}

void Window::request_attention()
{
	DISPLAY_SERVER->request_window_attention(m_window_id);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

String Window::get_clipboard() const
{
	return DISPLAY_SERVER->window_get_clipboard(m_window_id);
}

Vec2f Window::get_content_scale() const
{
	return DISPLAY_SERVER->window_get_content_scale(m_window_id);
}

IntRect Window::get_frame_size() const
{
	return DISPLAY_SERVER->window_get_frame_size(m_window_id);
}

Vec2i Window::get_framebuffer_size() const
{
	return DISPLAY_SERVER->window_get_framebuffer_size(m_window_id);
}

int32_t Window::get_input_mode(InputMode_ value) const
{
	return DISPLAY_SERVER->window_get_input_mode(m_window_id, value);
}

InputAction_ Window::get_key(KeyCode_ value) const
{
	return DISPLAY_SERVER->window_get_key(m_window_id, value);
}

InputAction_ Window::get_mouse_button(MouseButton_ value) const
{
	return DISPLAY_SERVER->window_get_mouse_button(m_window_id, value);
}

Vec2d Window::get_mouse_position() const
{
	return DISPLAY_SERVER->window_get_mouse_position(m_window_id);
}

void * Window::get_native_handle() const
{
	return DISPLAY_SERVER->window_get_native_handle(m_window_id);
}

float_t Window::get_opacity() const
{
	return DISPLAY_SERVER->window_get_opacity(m_window_id);
}

Vec2i Window::get_position() const
{
	return DISPLAY_SERVER->window_get_position(m_window_id);
}

Vec2i Window::get_size() const
{
	return DISPLAY_SERVER->window_get_size(m_window_id);
}

bool Window::get_should_close() const
{
	return DISPLAY_SERVER->window_get_should_close(m_window_id);
}

void * Window::get_user_pointer() const
{
	return DISPLAY_SERVER->window_get_user_pointer(m_window_id);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool Window::is_decorated() const
{
	return DISPLAY_SERVER->window_is_decorated(m_window_id);
}

bool Window::is_floating() const
{
	return DISPLAY_SERVER->window_is_floating(m_window_id);
}

bool Window::is_focused() const
{
	return DISPLAY_SERVER->window_is_focused(m_window_id);
}

bool Window::is_hovered() const
{
	return DISPLAY_SERVER->window_is_hovered(m_window_id);
}

bool Window::is_iconified() const
{
	return DISPLAY_SERVER->window_is_iconified(m_window_id);
}

bool Window::is_maximized() const
{
	return DISPLAY_SERVER->window_is_maximized(m_window_id);
}

bool Window::is_resizable() const
{
	return DISPLAY_SERVER->window_is_resizable(m_window_id);
}

bool Window::is_transparent() const
{
	return DISPLAY_SERVER->window_is_transparent(m_window_id);
}

bool Window::is_visible() const
{
	return DISPLAY_SERVER->window_is_visible(m_window_id);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void Window::set_clipboard(String const & value)
{
	DISPLAY_SERVER->window_set_clipboard(m_window_id, value);
}

void Window::set_cursor(CursorID value)
{
	DISPLAY_SERVER->window_set_cursor(m_window_id, value);
}

void Window::set_input_mode(InputMode_ value)
{
	DISPLAY_SERVER->window_set_cursor_mode(m_window_id, value);
}

void Window::set_icons(int32_t width, int32_t height, uint8_t * pixels, int32_t count)
{
	DISPLAY_SERVER->window_set_icons(m_window_id, width, height, pixels, count);
}

void Window::set_input_mode(int32_t mode, int32_t value)
{
	DISPLAY_SERVER->window_set_input_mode(m_window_id, mode, value);
}

void Window::set_monitor(MonitorID monitor, IntRect const & bounds)
{
	DISPLAY_SERVER->window_set_monitor(m_window_id, monitor, bounds);
}

void Window::set_mouse_position(Vec2d const & value)
{
	DISPLAY_SERVER->window_set_mouse_position(m_window_id, value);
}

void Window::set_opacity(float_t value)
{
	DISPLAY_SERVER->window_set_opacity(m_window_id, value);
}

void Window::set_position(Vec2i const & value)
{
	DISPLAY_SERVER->window_set_position(m_window_id, value);
}

void Window::set_should_close(bool value)
{
	DISPLAY_SERVER->window_set_should_close(m_window_id, value);
}

void Window::set_size(Vec2i const & value)
{
	DISPLAY_SERVER->window_set_size(m_window_id, value);
}

void Window::set_title(String const & value)
{
	DISPLAY_SERVER->window_set_title(m_window_id, value);
}

void Window::set_user_pointer(void * value)
{
	DISPLAY_SERVER->window_set_user_pointer(m_window_id, value);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void Window::set_is_decorated(bool value)
{
	DISPLAY_SERVER->window_set_is_decorated(m_window_id, value);
}

void Window::set_is_floating(bool value)
{
	DISPLAY_SERVER->window_set_is_floating(m_window_id, value);
}

void Window::set_is_resizable(bool value)
{
	DISPLAY_SERVER->window_set_is_resizable(m_window_id, value);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

WindowCharCallback Window::set_char_callback(WindowCharCallback value)
{
	return DISPLAY_SERVER->window_set_char_callback(m_window_id, value);
}

WindowCharModsCallback Window::set_char_mods_callback(WindowCharModsCallback value)
{
	return DISPLAY_SERVER->window_set_char_mods_callback(m_window_id, value);
}

WindowCloseCallback Window::set_close_callback(WindowCloseCallback value)
{
	return DISPLAY_SERVER->window_set_close_callback(m_window_id, value);
}

WindowContentCallback Window::set_content_scale_callback(WindowContentCallback value)
{
	return DISPLAY_SERVER->window_set_content_scale_callback(m_window_id, value);
}

WindowDropCallback Window::set_drop_callback(WindowDropCallback value)
{
	return DISPLAY_SERVER->window_set_drop_callback(m_window_id, value);
}

WindowFocusCallback Window::set_focus_callback(WindowFocusCallback value)
{
	return DISPLAY_SERVER->window_set_focus_callback(m_window_id, value);
}

WindowFramebufferResizeCallback Window::set_framebuffer_resize_callback(WindowFramebufferResizeCallback value)
{
	return DISPLAY_SERVER->window_set_framebuffer_resize_callback(m_window_id, value);
}

WindowIconifyCallback Window::set_iconify_callback(WindowIconifyCallback value)
{
	return DISPLAY_SERVER->window_set_iconify_callback(m_window_id, value);
}

WindowKeyCallback Window::set_key_callback(WindowKeyCallback value)
{
	return DISPLAY_SERVER->window_set_key_callback(m_window_id, value);
}

WindowMaximizeCallback Window::set_maximize_callback(WindowMaximizeCallback value)
{
	return DISPLAY_SERVER->window_set_maximize_callback(m_window_id, value);
}

WindowMouseButtonCallback Window::set_mouse_button_callback(WindowMouseButtonCallback value)
{
	return DISPLAY_SERVER->window_set_mouse_button_callback(m_window_id, value);
}

WindowMouseEnterCallback Window::set_mouse_enter_callback(WindowMouseEnterCallback value)
{
	return DISPLAY_SERVER->window_set_mouse_enter_callback(m_window_id, value);
}

WindowMousePositionCallback Window::set_mouse_position_callback(WindowMousePositionCallback value)
{
	return DISPLAY_SERVER->window_set_mouse_position_callback(m_window_id, value);
}

WindowPositionCallback Window::set_position_callback(WindowPositionCallback value)
{
	return DISPLAY_SERVER->window_set_position_callback(m_window_id, value);
}

WindowRefreshCallback Window::set_refresh_callback(WindowRefreshCallback value)
{
	return DISPLAY_SERVER->window_set_refresh_callback(m_window_id, value);
}

WindowScrollCallback Window::set_scroll_callback(WindowScrollCallback value)
{
	return DISPLAY_SERVER->window_set_scroll_callback(m_window_id, value);
}

WindowSizeCallback Window::set_size_callback(WindowSizeCallback value)
{
	return DISPLAY_SERVER->window_set_size_callback(m_window_id, value);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */