#include <runtime/window/display_context.hpp>

using namespace ism;

MEMBER_IMPL(DisplayContext::singleton) {};

OBJECT_IMPL(DisplayContext, t, TypeFlags_BaseType | TypeFlags_IsAbstract) {}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

Window * DisplayContext::window_new(WindowSettings const & settings)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

WindowID DisplayContext::get_current_context() const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

void DisplayContext::make_context_current(WindowID id)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

void DisplayContext::poll_events()
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

void DisplayContext::swap_buffers(WindowID id)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void DisplayContext::focus_window(WindowID id)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

void DisplayContext::hide_window(WindowID id)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

void DisplayContext::iconify_window(WindowID id)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

void DisplayContext::maximize_window(WindowID id)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

void DisplayContext::restore_window(WindowID id)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

void DisplayContext::request_window_attention(WindowID id)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

String DisplayContext::window_get_clipboard(WindowID id) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

Vec2 DisplayContext::window_get_content_scale(WindowID id) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

Vec2 DisplayContext::window_get_framebuffer_size(WindowID id) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

int32_t DisplayContext::window_get_input_mode(WindowID id, int32_t value) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

int32_t DisplayContext::window_get_key(WindowID id, int32_t value) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

int32_t DisplayContext::window_get_mouse_button(WindowID id, int32_t value) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

Vec2 DisplayContext::window_get_mouse_pos(WindowID id) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

WindowID DisplayContext::window_get_native_handle(WindowID id) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

float_t DisplayContext::window_get_opacity(WindowID id) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

Vec2 DisplayContext::window_get_position(WindowID id) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

Vec2 DisplayContext::window_get_size(WindowID id) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

void * DisplayContext::window_get_user_pointer(WindowID id) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

Rect DisplayContext::window_get_frame_size(WindowID id) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool DisplayContext::window_is_auto_iconify(WindowID id) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

bool DisplayContext::window_is_decorated(WindowID id) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

bool DisplayContext::window_is_floating(WindowID id) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

bool DisplayContext::window_is_focused(WindowID id) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

bool DisplayContext::window_is_focus_on_show(WindowID id) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

bool DisplayContext::window_is_hovered(WindowID id) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

bool DisplayContext::window_is_iconified(WindowID id) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

bool DisplayContext::window_is_maximized(WindowID id) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

bool DisplayContext::window_is_open(WindowID id) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

bool DisplayContext::window_is_resizable(WindowID id) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

bool DisplayContext::window_is_transparent(WindowID id) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

bool DisplayContext::window_is_visible(WindowID id) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void DisplayContext::window_set_auto_iconify(WindowID id, bool value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

void DisplayContext::window_set_clipboard(WindowID id, String const & value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

void DisplayContext::window_set_cursor(WindowID id, CursorID value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

void DisplayContext::window_set_cursor_mode(WindowID id, int32_t value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

void DisplayContext::window_set_mouse_pos(WindowID id, Vec2 const & value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

void DisplayContext::window_set_decorated(WindowID id, bool value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

void DisplayContext::window_set_floating(WindowID id, bool value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

void DisplayContext::window_set_focus_on_show(WindowID id, bool value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

void DisplayContext::window_set_icons(WindowID id, int32_t width, int32_t height, uint8_t * pixels, int32_t count)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

void DisplayContext::window_set_input_mode(WindowID id, int32_t mode, int32_t value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

void DisplayContext::window_set_opacity(WindowID id, float_t value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

void DisplayContext::window_set_position(WindowID id, Vec2 const & value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

void DisplayContext::window_set_monitor(WindowID id, MonitorID monitor, Rect const & bounds)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

void DisplayContext::window_set_resizable(WindowID id, bool value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

void DisplayContext::window_set_should_close(WindowID id, bool value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

void DisplayContext::window_set_size(WindowID id, Vec2 const & value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

void DisplayContext::window_set_title(WindowID id, String const & value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

void DisplayContext::window_set_user_pointer(WindowID id, void * value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION "");
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

WindowCharCallback DisplayContext::window_get_char_callback(WindowID id) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

WindowCharModsCallback DisplayContext::window_get_char_mods_callback(WindowID id) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

WindowCloseCallback DisplayContext::window_get_close_callback(WindowID id) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

WindowContentCallback DisplayContext::window_get_content_scale_callback(WindowID id) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

WindowMouseEnterCallback DisplayContext::window_get_mouse_enter_callback(WindowID id) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

WindowMousePosCallback DisplayContext::window_get_mouse_pos_callback(WindowID id) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

WindowDropCallback DisplayContext::window_get_drop_callback(WindowID id) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

WindowFocusCallback DisplayContext::window_get_focus_callback(WindowID id) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

WindowFramebufferResizeCallback DisplayContext::window_get_framebuffer_resize_callback(WindowID id) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

WindowIconifyCallback DisplayContext::window_get_iconify_callback(WindowID id) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

WindowKeyCallback DisplayContext::window_get_key_callback(WindowID id) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

WindowMaximizeCallback DisplayContext::window_get_maximize_callback(WindowID id) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

WindowMouseButtonCallback DisplayContext::window_get_mouse_button_callback(WindowID id) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

WindowPositionCallback DisplayContext::window_get_position_callback(WindowID id) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

WindowRefreshCallback DisplayContext::window_get_refresh_callback(WindowID id) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

WindowResizeCallback DisplayContext::window_get_resize_callback(WindowID id) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

WindowMouseScrollCallback DisplayContext::window_get_scroll_callback(WindowID id) const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

WindowCharCallback DisplayContext::window_set_char_callback(WindowID id, WindowCharCallback value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

WindowCharModsCallback DisplayContext::window_set_char_mods_callback(WindowID id, WindowCharModsCallback value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

WindowCloseCallback DisplayContext::window_set_close_callback(WindowID id, WindowCloseCallback value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

WindowContentCallback DisplayContext::window_set_content_scale_callback(WindowID id, WindowContentCallback value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

WindowDropCallback DisplayContext::window_set_drop_callback(WindowID id, WindowDropCallback value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

WindowFocusCallback DisplayContext::window_set_focus_callback(WindowID id, WindowFocusCallback value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

WindowFramebufferResizeCallback DisplayContext::window_set_framebuffer_resize_callback(WindowID id, WindowFramebufferResizeCallback value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

WindowIconifyCallback DisplayContext::window_set_iconify_callback(WindowID id, WindowIconifyCallback value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

WindowKeyCallback DisplayContext::window_set_key_callback(WindowID id, WindowKeyCallback value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

WindowMaximizeCallback DisplayContext::window_set_maximize_callback(WindowID id, WindowMaximizeCallback value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

WindowMouseButtonCallback DisplayContext::window_set_mouse_button_callback(WindowID id, WindowMouseButtonCallback value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

WindowMouseEnterCallback DisplayContext::window_set_mouse_enter_callback(WindowID id, WindowMouseEnterCallback value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

WindowMousePosCallback DisplayContext::window_set_mouse_pos_callback(WindowID id, WindowMousePosCallback value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

WindowMouseScrollCallback DisplayContext::window_set_mouse_scroll_callback(WindowID id, WindowMouseScrollCallback value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

WindowPositionCallback DisplayContext::window_set_position_callback(WindowID id, WindowPositionCallback value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

WindowRefreshCallback DisplayContext::window_set_refresh_callback(WindowID id, WindowRefreshCallback value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

WindowResizeCallback DisplayContext::window_set_resize_callback(WindowID id, WindowResizeCallback value)
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

VideoMode const & DisplayContext::get_desktop_video_mode() const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

Vector<VideoMode> const & DisplayContext::get_fullscreen_video_modes() const
{
	FATAL("NOT IMPLEMENTED: " PRETTY_FUNCTION ""); return {};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */