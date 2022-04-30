#include <servers/display_server.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJECT_EMBED(WindowCharEvent, t) {}

OBJECT_EMBED(WindowCharModsEvent, t) {}

OBJECT_EMBED(WindowCloseEvent, t) {}

OBJECT_EMBED(WindowContentScaleEvent, t) {}

OBJECT_EMBED(WindowDropEvent, t) {}

OBJECT_EMBED(WindowFocusEvent, t) {}

OBJECT_EMBED(WindowFramebufferResizeEvent, t) {}

OBJECT_EMBED(WindowIconifyEvent, t) {}

OBJECT_EMBED(WindowKeyEvent, t) {}

OBJECT_EMBED(WindowMaximizeEvent, t) {}

OBJECT_EMBED(WindowMouseButtonEvent, t) {}

OBJECT_EMBED(WindowMouseEnterEvent, t) {}

OBJECT_EMBED(WindowMousePositionEvent, t) {}

OBJECT_EMBED(WindowScrollEvent, t) {}

OBJECT_EMBED(WindowPositionEvent, t) {}

OBJECT_EMBED(WindowRefreshEvent, t) {}

OBJECT_EMBED(WindowSizeEvent, t) {}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

MEMBER_IMPL(DisplayServer::singleton) {};

OBJECT_EMBED(DisplayServer, t, TypeFlags_IsAbstract) {}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

WindowID DisplayServer::get_context_current() const
{
	CRASH("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

void DisplayServer::make_context_current(WindowID id)
{
	CRASH("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

void DisplayServer::poll_events()
{
	CRASH("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

void DisplayServer::swap_buffers(WindowID id)
{
	CRASH("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

void DisplayServer::swap_interval(int32_t value)
{
	CRASH("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

CursorID DisplayServer::create_custom_cursor(int32_t w, int32_t h, byte const * p, int32_t x, int32_t y)
{
	CRASH("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

CursorID DisplayServer::create_standard_cursor(CursorShape shape)
{
	CRASH("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

void DisplayServer::destroy_cursor(CursorID value)
{
	CRASH("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void DisplayServer::focus_window(WindowID id)
{
	CRASH("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

void DisplayServer::hide_window(WindowID id)
{
	CRASH("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

void DisplayServer::iconify_window(WindowID id)
{
	CRASH("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

void DisplayServer::maximize_window(WindowID id)
{
	CRASH("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

void DisplayServer::restore_window(WindowID id)
{
	CRASH("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

void DisplayServer::request_window_attention(WindowID id)
{
	CRASH("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

String DisplayServer::window_get_clipboard(WindowID id) const
{
	CRASH("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

Vec2f DisplayServer::window_get_content_scale(WindowID id) const
{
	CRASH("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

IntRect DisplayServer::window_get_frame_size(WindowID id) const
{
	CRASH("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

Vec2i DisplayServer::window_get_framebuffer_size(WindowID id) const
{
	CRASH("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

int32_t DisplayServer::window_get_input_mode(WindowID id, InputMode value) const
{
	CRASH("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

InputAction DisplayServer::window_get_key(WindowID id, KeyCode value) const
{
	CRASH("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

InputAction DisplayServer::window_get_mouse_button(WindowID id, MouseButton value) const
{
	CRASH("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

Vec2d DisplayServer::window_get_mouse_position(WindowID id) const
{
	CRASH("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

void * DisplayServer::window_get_native_handle(WindowID id) const
{
	CRASH("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

float_t DisplayServer::window_get_opacity(WindowID id) const
{
	CRASH("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

Vec2i DisplayServer::window_get_position(WindowID id) const
{
	CRASH("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

Vec2i DisplayServer::window_get_size(WindowID id) const
{
	CRASH("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

bool DisplayServer::window_get_should_close(WindowID id) const
{
	CRASH("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

void * DisplayServer::window_get_user_pointer(WindowID id) const
{
	CRASH("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool DisplayServer::window_is_decorated(WindowID id) const
{
	CRASH("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

bool DisplayServer::window_is_floating(WindowID id) const
{
	CRASH("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

bool DisplayServer::window_is_focused(WindowID id) const
{
	CRASH("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

bool DisplayServer::window_is_hovered(WindowID id) const
{
	CRASH("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

bool DisplayServer::window_is_iconified(WindowID id) const
{
	CRASH("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

bool DisplayServer::window_is_maximized(WindowID id) const
{
	CRASH("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

bool DisplayServer::window_is_resizable(WindowID id) const
{
	CRASH("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

bool DisplayServer::window_is_transparent(WindowID id) const
{
	CRASH("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

bool DisplayServer::window_is_visible(WindowID id) const
{
	CRASH("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void DisplayServer::window_set_clipboard(WindowID id, String const & value)
{
	CRASH("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

void DisplayServer::window_set_cursor(WindowID id, CursorID value)
{
	CRASH("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

void DisplayServer::window_set_cursor_mode(WindowID id, int32_t value)
{
	CRASH("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

void DisplayServer::window_set_icons(WindowID id, int32_t width, int32_t height, uint8_t * pixels, int32_t count)
{
	CRASH("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

void DisplayServer::window_set_input_mode(WindowID id, int32_t mode, int32_t value)
{
	CRASH("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

void DisplayServer::window_set_monitor(WindowID id, MonitorID monitor, IntRect const & bounds)
{
	CRASH("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

void DisplayServer::window_set_mouse_position(WindowID id, Vec2d const & value)
{
	CRASH("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

void DisplayServer::window_set_opacity(WindowID id, float_t value)
{
	CRASH("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

void DisplayServer::window_set_position(WindowID id, Vec2i const & value)
{
	CRASH("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

void DisplayServer::window_set_should_close(WindowID id, bool value)
{
	CRASH("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

void DisplayServer::window_set_size(WindowID id, Vec2i const & value)
{
	CRASH("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

void DisplayServer::window_set_title(WindowID id, String const & value)
{
	CRASH("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

void DisplayServer::window_set_user_pointer(WindowID id, void * value)
{
	CRASH("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void DisplayServer::window_set_is_decorated(WindowID id, bool value)
{
	CRASH("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

void DisplayServer::window_set_is_floating(WindowID id, bool value)
{
	CRASH("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

void DisplayServer::window_set_is_resizable(WindowID id, bool value)
{
	CRASH("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

WindowCharCallback DisplayServer::window_set_char_callback(WindowID id, WindowCharCallback value)
{
	CRASH("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

WindowCharModsCallback DisplayServer::window_set_char_mods_callback(WindowID id, WindowCharModsCallback value)
{
	CRASH("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

WindowCloseCallback DisplayServer::window_set_close_callback(WindowID id, WindowCloseCallback value)
{
	CRASH("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

WindowContentCallback DisplayServer::window_set_content_scale_callback(WindowID id, WindowContentCallback value)
{
	CRASH("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

WindowDropCallback DisplayServer::window_set_drop_callback(WindowID id, WindowDropCallback value)
{
	CRASH("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

WindowFocusCallback DisplayServer::window_set_focus_callback(WindowID id, WindowFocusCallback value)
{
	CRASH("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

WindowFramebufferResizeCallback DisplayServer::window_set_framebuffer_resize_callback(WindowID id, WindowFramebufferResizeCallback value)
{
	CRASH("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

WindowIconifyCallback DisplayServer::window_set_iconify_callback(WindowID id, WindowIconifyCallback value)
{
	CRASH("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

WindowKeyCallback DisplayServer::window_set_key_callback(WindowID id, WindowKeyCallback value)
{
	CRASH("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

WindowMaximizeCallback DisplayServer::window_set_maximize_callback(WindowID id, WindowMaximizeCallback value)
{
	CRASH("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

WindowMouseButtonCallback DisplayServer::window_set_mouse_button_callback(WindowID id, WindowMouseButtonCallback value)
{
	CRASH("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

WindowMouseEnterCallback DisplayServer::window_set_mouse_enter_callback(WindowID id, WindowMouseEnterCallback value)
{
	CRASH("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

WindowMousePositionCallback DisplayServer::window_set_mouse_position_callback(WindowID id, WindowMousePositionCallback value)
{
	CRASH("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

WindowPositionCallback DisplayServer::window_set_position_callback(WindowID id, WindowPositionCallback value)
{
	CRASH("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

WindowRefreshCallback DisplayServer::window_set_refresh_callback(WindowID id, WindowRefreshCallback value)
{
	CRASH("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

WindowScrollCallback DisplayServer::window_set_scroll_callback(WindowID id, WindowScrollCallback value)
{
	CRASH("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

WindowSizeCallback DisplayServer::window_set_size_callback(WindowID id, WindowSizeCallback value)
{
	CRASH("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
	return {};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */