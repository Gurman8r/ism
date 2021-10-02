#include <servers/display_server.hpp>

using namespace ism;

MEMBER_IMPL(DisplayServer::singleton) {};

EMBEDDED_CLASS_TYPE(DisplayServer, t, TypeFlags_IsAbstract) {}

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
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

Vec2 DisplayServer::window_get_content_scale(WindowID id) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

Vec2 DisplayServer::window_get_framebuffer_size(WindowID id) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

int32_t DisplayServer::window_get_input_mode(WindowID id, int32_t value) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

int32_t DisplayServer::window_get_key(WindowID id, int32_t value) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

int32_t DisplayServer::window_get_mouse_button(WindowID id, int32_t value) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

Vec2 DisplayServer::window_get_mouse_pos(WindowID id) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

WindowID DisplayServer::window_get_native_handle(WindowID id) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

float_t DisplayServer::window_get_opacity(WindowID id) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

Vec2 DisplayServer::window_get_position(WindowID id) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

Vec2 DisplayServer::window_get_size(WindowID id) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

void * DisplayServer::window_get_user_pointer(WindowID id) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

Rect DisplayServer::window_get_frame_size(WindowID id) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool DisplayServer::window_is_auto_iconify(WindowID id) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

bool DisplayServer::window_is_decorated(WindowID id) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

bool DisplayServer::window_is_floating(WindowID id) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

bool DisplayServer::window_is_focused(WindowID id) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

bool DisplayServer::window_is_focus_on_show(WindowID id) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

bool DisplayServer::window_is_hovered(WindowID id) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

bool DisplayServer::window_is_iconified(WindowID id) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

bool DisplayServer::window_is_maximized(WindowID id) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

bool DisplayServer::window_is_open(WindowID id) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

bool DisplayServer::window_is_resizable(WindowID id) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

bool DisplayServer::window_is_transparent(WindowID id) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

bool DisplayServer::window_is_visible(WindowID id) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void DisplayServer::window_set_auto_iconify(WindowID id, bool value)
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

void DisplayServer::window_set_mouse_pos(WindowID id, Vec2 const & value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

void DisplayServer::window_set_decorated(WindowID id, bool value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

void DisplayServer::window_set_floating(WindowID id, bool value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

void DisplayServer::window_set_focus_on_show(WindowID id, bool value)
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

void DisplayServer::window_set_opacity(WindowID id, float_t value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

void DisplayServer::window_set_position(WindowID id, Vec2 const & value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

void DisplayServer::window_set_monitor(WindowID id, MonitorID monitor, Rect const & bounds)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

void DisplayServer::window_set_resizable(WindowID id, bool value)
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
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

WindowCharModsCallback DisplayServer::window_set_char_mods_callback(WindowID id, WindowCharModsCallback value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

WindowCloseCallback DisplayServer::window_set_close_callback(WindowID id, WindowCloseCallback value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

WindowContentCallback DisplayServer::window_set_content_scale_callback(WindowID id, WindowContentCallback value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

WindowDropCallback DisplayServer::window_set_drop_callback(WindowID id, WindowDropCallback value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

WindowFocusCallback DisplayServer::window_set_focus_callback(WindowID id, WindowFocusCallback value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

WindowFramebufferResizeCallback DisplayServer::window_set_framebuffer_resize_callback(WindowID id, WindowFramebufferResizeCallback value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

WindowIconifyCallback DisplayServer::window_set_iconify_callback(WindowID id, WindowIconifyCallback value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

WindowKeyCallback DisplayServer::window_set_key_callback(WindowID id, WindowKeyCallback value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

WindowMaximizeCallback DisplayServer::window_set_maximize_callback(WindowID id, WindowMaximizeCallback value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

WindowMouseButtonCallback DisplayServer::window_set_mouse_button_callback(WindowID id, WindowMouseButtonCallback value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

WindowMouseEnterCallback DisplayServer::window_set_mouse_enter_callback(WindowID id, WindowMouseEnterCallback value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

WindowMousePosCallback DisplayServer::window_set_mouse_pos_callback(WindowID id, WindowMousePosCallback value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

WindowMouseScrollCallback DisplayServer::window_set_mouse_scroll_callback(WindowID id, WindowMouseScrollCallback value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

WindowPositionCallback DisplayServer::window_set_position_callback(WindowID id, WindowPositionCallback value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

WindowRefreshCallback DisplayServer::window_set_refresh_callback(WindowID id, WindowRefreshCallback value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

WindowResizeCallback DisplayServer::window_set_resize_callback(WindowID id, WindowResizeCallback value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */