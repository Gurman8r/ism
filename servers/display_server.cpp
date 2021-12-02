#include <servers/display_server.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EMBEDED_CLASS(WindowCharEvent, t) {}

EMBEDED_CLASS(WindowCharModsEvent, t) {}

EMBEDED_CLASS(WindowCloseEvent, t) {}

EMBEDED_CLASS(WindowContentScaleEvent, t) {}

EMBEDED_CLASS(WindowDropEvent, t) {}

EMBEDED_CLASS(WindowFocusEvent, t) {}

EMBEDED_CLASS(WindowFramebufferResizeEvent, t) {}

EMBEDED_CLASS(WindowIconifyEvent, t) {}

EMBEDED_CLASS(WindowKeyEvent, t) {}

EMBEDED_CLASS(WindowMaximizeEvent, t) {}

EMBEDED_CLASS(WindowMouseButtonEvent, t) {}

EMBEDED_CLASS(WindowMouseEnterEvent, t) {}

EMBEDED_CLASS(WindowMousePositionEvent, t) {}

EMBEDED_CLASS(WindowScrollEvent, t) {}

EMBEDED_CLASS(WindowPositionEvent, t) {}

EMBEDED_CLASS(WindowRefreshEvent, t) {}

EMBEDED_CLASS(WindowSizeEvent, t) {}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

MEMBER_IMPL(DisplayServer::singleton) {};

EMBEDED_CLASS(DisplayServer, t, TypeFlags_IsAbstract) {}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

WindowID DisplayServer::get_context_current() const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

void DisplayServer::make_context_current(WindowID id)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

void DisplayServer::poll_events()
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

void DisplayServer::swap_buffers(WindowID id)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

void DisplayServer::swap_interval(int32_t value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

CursorID DisplayServer::create_custom_cursor(int32_t w, int32_t h, byte const * p, int32_t x, int32_t y)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

CursorID DisplayServer::create_standard_cursor(CursorShape shape)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

void DisplayServer::destroy_cursor(CursorID value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void DisplayServer::focus_window(WindowID id)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

void DisplayServer::hide_window(WindowID id)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

void DisplayServer::iconify_window(WindowID id)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

void DisplayServer::maximize_window(WindowID id)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

void DisplayServer::restore_window(WindowID id)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

void DisplayServer::request_window_attention(WindowID id)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

String DisplayServer::window_get_clipboard(WindowID id) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

Vec2 DisplayServer::window_get_content_scale(WindowID id) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

Rect DisplayServer::window_get_frame_size(WindowID id) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

Vec2 DisplayServer::window_get_framebuffer_size(WindowID id) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

int32_t DisplayServer::window_get_input_mode(WindowID id, InputMode value) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

InputAction DisplayServer::window_get_key(WindowID id, KeyCode value) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

InputAction DisplayServer::window_get_mouse_button(WindowID id, MouseButton value) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

Vec2 DisplayServer::window_get_mouse_position(WindowID id) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

void * DisplayServer::window_get_native_handle(WindowID id) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

float_t DisplayServer::window_get_opacity(WindowID id) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

Vec2 DisplayServer::window_get_position(WindowID id) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

Vec2 DisplayServer::window_get_size(WindowID id) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

bool DisplayServer::window_get_is_auto_iconify(WindowID id) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

bool DisplayServer::window_get_is_decorated(WindowID id) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

bool DisplayServer::window_get_is_floating(WindowID id) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

bool DisplayServer::window_get_is_focused(WindowID id) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

bool DisplayServer::window_get_is_focus_on_show(WindowID id) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

bool DisplayServer::window_get_is_hovered(WindowID id) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

bool DisplayServer::window_get_is_iconified(WindowID id) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

bool DisplayServer::window_get_is_maximized(WindowID id) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

bool DisplayServer::window_get_is_resizable(WindowID id) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

bool DisplayServer::window_get_is_transparent(WindowID id) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

bool DisplayServer::window_get_is_visible(WindowID id) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

bool DisplayServer::window_get_should_close(WindowID id) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

void * DisplayServer::window_get_user_pointer(WindowID id) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void DisplayServer::window_set_is_auto_iconify(WindowID id, bool value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

void DisplayServer::window_set_clipboard(WindowID id, String const & value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

void DisplayServer::window_set_cursor(WindowID id, CursorID value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

void DisplayServer::window_set_cursor_mode(WindowID id, int32_t value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

void DisplayServer::window_set_is_decorated(WindowID id, bool value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

void DisplayServer::window_set_is_floating(WindowID id, bool value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

void DisplayServer::window_set_is_focus_on_show(WindowID id, bool value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

void DisplayServer::window_set_icons(WindowID id, int32_t width, int32_t height, uint8_t * pixels, int32_t count)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

void DisplayServer::window_set_input_mode(WindowID id, int32_t mode, int32_t value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

void DisplayServer::window_set_monitor(WindowID id, MonitorID monitor, Rect const & bounds)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

void DisplayServer::window_set_mouse_position(WindowID id, Vec2 const & value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

void DisplayServer::window_set_opacity(WindowID id, float_t value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

void DisplayServer::window_set_position(WindowID id, Vec2 const & value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

void DisplayServer::window_set_is_resizable(WindowID id, bool value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

void DisplayServer::window_set_should_close(WindowID id, bool value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

void DisplayServer::window_set_size(WindowID id, Vec2 const & value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

void DisplayServer::window_set_title(WindowID id, String const & value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

void DisplayServer::window_set_user_pointer(WindowID id, void * value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

WindowCharCallback DisplayServer::window_set_char_callback(WindowID id, WindowCharCallback value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

WindowCharModsCallback DisplayServer::window_set_char_mods_callback(WindowID id, WindowCharModsCallback value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

WindowCloseCallback DisplayServer::window_set_close_callback(WindowID id, WindowCloseCallback value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

WindowContentCallback DisplayServer::window_set_content_scale_callback(WindowID id, WindowContentCallback value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

WindowDropCallback DisplayServer::window_set_drop_callback(WindowID id, WindowDropCallback value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

WindowFocusCallback DisplayServer::window_set_focus_callback(WindowID id, WindowFocusCallback value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

WindowFramebufferResizeCallback DisplayServer::window_set_framebuffer_resize_callback(WindowID id, WindowFramebufferResizeCallback value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

WindowIconifyCallback DisplayServer::window_set_iconify_callback(WindowID id, WindowIconifyCallback value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

WindowKeyCallback DisplayServer::window_set_key_callback(WindowID id, WindowKeyCallback value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

WindowMaximizeCallback DisplayServer::window_set_maximize_callback(WindowID id, WindowMaximizeCallback value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

WindowMouseButtonCallback DisplayServer::window_set_mouse_button_callback(WindowID id, WindowMouseButtonCallback value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

WindowMouseEnterCallback DisplayServer::window_set_mouse_enter_callback(WindowID id, WindowMouseEnterCallback value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

WindowMousePositionCallback DisplayServer::window_set_mouse_position_callback(WindowID id, WindowMousePositionCallback value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

WindowPositionCallback DisplayServer::window_set_position_callback(WindowID id, WindowPositionCallback value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

WindowRefreshCallback DisplayServer::window_set_refresh_callback(WindowID id, WindowRefreshCallback value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

WindowScrollCallback DisplayServer::window_set_scroll_callback(WindowID id, WindowScrollCallback value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

WindowSizeCallback DisplayServer::window_set_size_callback(WindowID id, WindowSizeCallback value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */