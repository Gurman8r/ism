#include <platform/windows/display_context_windows.hpp>
#include <runtime/scene/scene_tree.hpp>

#include <glfw/glfw3.h>
#if defined(SYSTEM_WINDOWS)
#	undef APIENTRY
#	include <Windows.h>
#	define GLFW_EXPOSE_NATIVE_WIN32
#	include <glfw/glfw3native.h>
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

using namespace ism;

OBJECT_IMPL(DisplayContextWindows, t) {}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

DisplayContextWindows::DisplayContextWindows()
{
	VERIFY(glfwInit() == GLFW_TRUE);
}

DisplayContextWindows::~DisplayContextWindows()
{
	glfwTerminate();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

Window * DisplayContextWindows::new_window(WindowSettings const & settings)
{
	// context hints
	glfwWindowHint(GLFW_CLIENT_API, std::invoke([&]() noexcept {
		switch (settings.context.api) {
		default: return GLFW_NO_API;
		case RendererAPI_OpenGL: return GLFW_OPENGL_API;
		}
	}));
	glfwWindowHint(GLFW_OPENGL_PROFILE, std::invoke([&]() noexcept {
		switch (settings.context.profile) {
		default: return GLFW_OPENGL_ANY_PROFILE;
		case RendererProfile_Core: return GLFW_OPENGL_CORE_PROFILE;
		case RendererProfile_Compat: return GLFW_OPENGL_COMPAT_PROFILE;
		case RendererProfile_Debug: return GLFW_OPENGL_DEBUG_CONTEXT;
		}
	}));
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, settings.context.major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, settings.context.minor);

	// style hints
	glfwWindowHint(GLFW_AUTO_ICONIFY, settings.hints & WindowHints_AutoIconify);
	glfwWindowHint(GLFW_CENTER_CURSOR, settings.hints & WindowHints_CenterCursor);
	glfwWindowHint(GLFW_DECORATED, settings.hints & WindowHints_Decorated);
	glfwWindowHint(GLFW_DOUBLEBUFFER, settings.hints & WindowHints_Doublebuffer);
	glfwWindowHint(GLFW_FLOATING, settings.hints & WindowHints_Floating);
	glfwWindowHint(GLFW_FOCUS_ON_SHOW, settings.hints & WindowHints_FocusOnShow);
	glfwWindowHint(GLFW_FOCUSED, settings.hints & WindowHints_Focused);
	glfwWindowHint(GLFW_MAXIMIZED, settings.hints & WindowHints_Maximized);
	glfwWindowHint(GLFW_RESIZABLE, settings.hints & WindowHints_Resizable);
	glfwWindowHint(GLFW_VISIBLE, settings.hints & WindowHints_Visible);

	// monitor hints
	glfwWindowHint(GLFW_REFRESH_RATE, settings.video.refresh_rate);

	// framebuffer hints
	glfwWindowHint(GLFW_RED_BITS, (int32_t)settings.video.bits_per_pixel[0]);
	glfwWindowHint(GLFW_GREEN_BITS, (int32_t)settings.video.bits_per_pixel[1]);
	glfwWindowHint(GLFW_BLUE_BITS, (int32_t)settings.video.bits_per_pixel[2]);
	glfwWindowHint(GLFW_ALPHA_BITS, (int32_t)settings.video.bits_per_pixel[3]);
	glfwWindowHint(GLFW_DEPTH_BITS, settings.context.depth_bits);
	glfwWindowHint(GLFW_STENCIL_BITS, settings.context.stencil_bits);
	glfwWindowHint(GLFW_SRGB_CAPABLE, settings.context.srgb_capable);

	// create window
	return memnew(Window((WindowID)glfwCreateWindow(
		(int32_t)settings.video.size[0],
		(int32_t)settings.video.size[1],
		(cstring)settings.title.c_str(),
		(GLFWmonitor *)settings.monitor,
		(GLFWwindow *)settings.share),
		settings.hints));
}

void DisplayContextWindows::poll_events()
{
	glfwPollEvents();
}

void DisplayContextWindows::swap_buffers(WindowID id)
{
	glfwSwapBuffers((GLFWwindow *)id);
}

WindowID DisplayContextWindows::get_current_context() const
{
	return (WindowID)glfwGetCurrentContext();
}

void DisplayContextWindows::make_context_current(WindowID id)
{
	glfwMakeContextCurrent((GLFWwindow *)id);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void DisplayContextWindows::focus_window(WindowID id)
{
	glfwFocusWindow((GLFWwindow *)id);
}

void DisplayContextWindows::hide_window(WindowID id)
{
	glfwHideWindow((GLFWwindow *)id);
}

void DisplayContextWindows::iconify_window(WindowID id)
{
	glfwIconifyWindow((GLFWwindow *)id);
}

void DisplayContextWindows::maximize_window(WindowID id)
{
	glfwMaximizeWindow((GLFWwindow *)id);
}

void DisplayContextWindows::restore_window(WindowID id)
{
	glfwRestoreWindow((GLFWwindow *)id);
}

void DisplayContextWindows::request_window_attention(WindowID id)
{
	glfwRequestWindowAttention((GLFWwindow *)id);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

String DisplayContextWindows::window_get_clipboard(WindowID id) const
{
	return glfwGetClipboardString((GLFWwindow *)id);
}

Vec2 DisplayContextWindows::window_get_content_scale(WindowID id) const
{
	float_t x, y;
	glfwGetWindowContentScale((GLFWwindow *)id, &x, &y);
	return Vec2{ x, y };
}

Vec2 DisplayContextWindows::window_get_framebuffer_size(WindowID id) const
{
	int32_t x, y;
	glfwGetFramebufferSize((GLFWwindow *)id, &x, &y);
	return Vec2{ x, y };
}

int32_t DisplayContextWindows::window_get_input_mode(WindowID id, int32_t value) const
{
	return glfwGetInputMode((GLFWwindow *)id, value);
}

int32_t DisplayContextWindows::window_get_key(WindowID id, int32_t value) const
{
	return glfwGetKey((GLFWwindow *)id, value);
}

int32_t DisplayContextWindows::window_get_mouse_button(WindowID id, int32_t value) const
{
	return glfwGetMouseButton((GLFWwindow *)id, value);
}

Vec2 DisplayContextWindows::window_get_mouse_pos(WindowID id) const
{
	double_t x, y;
	glfwGetCursorPos((GLFWwindow *)id, &x, &y);
	return Vec2{ x, y };
}

WindowID DisplayContextWindows::window_get_native_handle(WindowID id) const
{
#ifdef SYSTEM_WINDOWS
	return (WindowID)glfwGetWin32Window((GLFWwindow *)id);
#else
	return (WindowID)(GLFWwindow *)id;
#endif
}

float_t DisplayContextWindows::window_get_opacity(WindowID id) const
{
	return glfwGetWindowOpacity((GLFWwindow *)id);
}

Vec2 DisplayContextWindows::window_get_position(WindowID id) const
{
	int32_t x, y;
	glfwGetWindowPos((GLFWwindow *)id, &x, &y);
	return Vec2{ x, y };
}

Vec2 DisplayContextWindows::window_get_size(WindowID id) const
{
	int32_t x, y;
	glfwGetWindowSize((GLFWwindow *)id, &x, &y);
	return Vec2{ x, y };
}

void * DisplayContextWindows::window_get_user_pointer(WindowID id) const
{
	return glfwGetWindowUserPointer((GLFWwindow *)id);
}

Rect DisplayContextWindows::window_get_frame_size(WindowID id) const
{
	int32_t l, t, r, b;
	glfwGetWindowFrameSize((GLFWwindow *)id, &l, &r, &t, &b);
	return { l, t, r - l, b - t };
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool DisplayContextWindows::window_is_auto_iconify(WindowID id) const
{
	return id && glfwGetWindowAttrib((GLFWwindow *)id, GLFW_AUTO_ICONIFY);
}

bool DisplayContextWindows::window_is_decorated(WindowID id) const
{
	return id && glfwGetWindowAttrib((GLFWwindow *)id, GLFW_DECORATED);
}

bool DisplayContextWindows::window_is_floating(WindowID id) const
{
	return id && glfwGetWindowAttrib((GLFWwindow *)id, GLFW_FLOATING);
}

bool DisplayContextWindows::window_is_focused(WindowID id) const
{
	return id && glfwGetWindowAttrib((GLFWwindow *)id, GLFW_FOCUS_ON_SHOW);
}

bool DisplayContextWindows::window_is_focus_on_show(WindowID id) const
{
	return id && glfwGetWindowAttrib((GLFWwindow *)id, GLFW_FOCUSED);
}

bool DisplayContextWindows::window_is_hovered(WindowID id) const
{
	return id && glfwGetWindowAttrib((GLFWwindow *)id, GLFW_HOVERED);
}

bool DisplayContextWindows::window_is_iconified(WindowID id) const
{
	return id && glfwGetWindowAttrib((GLFWwindow *)id, GLFW_ICONIFIED);
}

bool DisplayContextWindows::window_is_maximized(WindowID id) const
{
	return id && glfwGetWindowAttrib((GLFWwindow *)id, GLFW_MAXIMIZED);
}

bool DisplayContextWindows::window_is_open(WindowID id) const
{
	return id && !glfwWindowShouldClose((GLFWwindow *)id);
}

bool DisplayContextWindows::window_is_resizable(WindowID id) const
{
	return id && glfwGetWindowAttrib((GLFWwindow *)id, GLFW_RESIZABLE);
}

bool DisplayContextWindows::window_is_transparent(WindowID id) const
{
	return id && glfwGetWindowAttrib((GLFWwindow *)id, GLFW_TRANSPARENT_FRAMEBUFFER);
}

bool DisplayContextWindows::window_is_visible(WindowID id) const
{
	return id && glfwGetWindowAttrib((GLFWwindow *)id, GLFW_VISIBLE);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void DisplayContextWindows::window_set_auto_iconify(WindowID id, bool value)
{
	glfwSetWindowAttrib((GLFWwindow *)id, GLFW_AUTO_ICONIFY, value);
}

void DisplayContextWindows::window_set_clipboard(WindowID id, String const & value)
{
	glfwSetClipboardString((GLFWwindow *)id, value.c_str());
}

void DisplayContextWindows::window_set_cursor(WindowID id, CursorID value)
{
	glfwSetCursor((GLFWwindow *)id, (GLFWcursor *)value);
}

void DisplayContextWindows::window_set_cursor_mode(WindowID id, int32_t value)
{
	glfwSetInputMode((GLFWwindow *)id, GLFW_CURSOR, std::invoke([value]() noexcept
	{
		switch (value)
		{
		default: return 0;
		case CursorMode_Normal: return GLFW_CURSOR_NORMAL;
		case CursorMode_Hidden: return GLFW_CURSOR_HIDDEN;
		case CursorMode_Disabled: return GLFW_CURSOR_DISABLED;
		}
	}));
}

void DisplayContextWindows::window_set_mouse_pos(WindowID id, Vec2 const & value)
{
	glfwSetCursorPos((GLFWwindow *)id, value[0], value[1]);
}

void DisplayContextWindows::window_set_decorated(WindowID id, bool value)
{
	glfwSetWindowAttrib((GLFWwindow *)id, GLFW_DECORATED, value);
}

void DisplayContextWindows::window_set_floating(WindowID id, bool value)
{
	glfwSetWindowAttrib((GLFWwindow *)id, GLFW_FLOATING, value);
}

void DisplayContextWindows::window_set_focus_on_show(WindowID id, bool value)
{
	glfwSetWindowAttrib((GLFWwindow *)id, GLFW_FOCUS_ON_SHOW, value);
}

void DisplayContextWindows::window_set_icons(WindowID id, int32_t width, int32_t height, uint8_t * pixels, int32_t count)
{
	GLFWimage img{ width, height, pixels };

	glfwSetWindowIcon((GLFWwindow *)id, count, &img);
}

void DisplayContextWindows::window_set_input_mode(WindowID id, int32_t mode, int32_t value)
{
	glfwSetInputMode((GLFWwindow *)id, mode, value);
}

void DisplayContextWindows::window_set_opacity(WindowID id, float_t value)
{
	glfwSetWindowOpacity((GLFWwindow *)id, value);
}

void DisplayContextWindows::window_set_position(WindowID id, Vec2 const & value)
{
	glfwSetWindowPos((GLFWwindow *)id, (int32_t)value[0], (int32_t)value[1]);
}

void DisplayContextWindows::window_set_monitor(WindowID id, MonitorID monitor, Rect const & bounds)
{
}

void DisplayContextWindows::window_set_resizable(WindowID id, bool value)
{
	glfwSetWindowAttrib((GLFWwindow *)id, GLFW_RESIZABLE, value);
}

void DisplayContextWindows::window_set_should_close(WindowID id, bool value)
{
	glfwSetWindowShouldClose((GLFWwindow *)id, value);
}

void DisplayContextWindows::window_set_size(WindowID id, Vec2 const & value)
{
	glfwSetWindowSize((GLFWwindow *)id, (int32_t)value[0], (int32_t)value[1]);
}

void DisplayContextWindows::window_set_title(WindowID id, String const & value)
{
	glfwSetWindowTitle((GLFWwindow *)id, value.c_str());
}

void DisplayContextWindows::window_set_user_pointer(WindowID id, void * value)
{
	glfwSetWindowUserPointer((GLFWwindow *)id, value);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

window_char_callback DisplayContextWindows::window_get_char_callback(WindowID id) const
{
	return window_char_callback{};
}

window_char_mods_callback DisplayContextWindows::window_get_char_mods_callback(WindowID id) const
{
	return window_char_mods_callback{};
}

window_close_callback DisplayContextWindows::window_get_close_callback(WindowID id) const
{
	return window_close_callback{};
}

window_content_scale_callback DisplayContextWindows::window_get_content_scale_callback(WindowID id) const
{
	return window_content_scale_callback{};
}

window_mouse_enter_callback DisplayContextWindows::window_get_mouse_enter_callback(WindowID id) const
{
	return window_mouse_enter_callback{};
}

window_mouse_pos_callback DisplayContextWindows::window_get_mouse_pos_callback(WindowID id) const
{
	return window_mouse_pos_callback{};
}

window_drop_callback DisplayContextWindows::window_get_drop_callback(WindowID id) const
{
	return window_drop_callback{};
}

window_focus_callback DisplayContextWindows::window_get_focus_callback(WindowID id) const
{
	return window_focus_callback{};
}

window_framebuffer_resize_callback DisplayContextWindows::window_get_framebuffer_resize_callback(WindowID id) const
{
	return window_framebuffer_resize_callback{};
}

window_iconify_callback DisplayContextWindows::window_get_iconify_callback(WindowID id) const
{
	return window_iconify_callback{};
}

window_key_callback DisplayContextWindows::window_get_key_callback(WindowID id) const
{
	return window_key_callback{};
}

window_maximize_callback DisplayContextWindows::window_get_maximize_callback(WindowID id) const
{
	return window_maximize_callback{};
}

window_mouse_button_callback DisplayContextWindows::window_get_mouse_button_callback(WindowID id) const
{
	return window_mouse_button_callback{};
}

window_position_callback DisplayContextWindows::window_get_position_callback(WindowID id) const
{
	return window_position_callback{};
}

window_refresh_callback DisplayContextWindows::window_get_refresh_callback(WindowID id) const
{
	return window_refresh_callback{};
}

window_resize_callback DisplayContextWindows::window_get_resize_callback(WindowID id) const
{
	return window_resize_callback{};
}

window_scroll_callback DisplayContextWindows::window_get_scroll_callback(WindowID id) const
{
	return window_scroll_callback{};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

window_char_callback DisplayContextWindows::window_set_char_callback(WindowID id, window_char_callback value)
{
	return reinterpret_cast<window_char_callback>(
		glfwSetCharCallback((GLFWwindow *)id,
			reinterpret_cast<GLFWcharfun>(value)));
}

window_char_mods_callback DisplayContextWindows::window_set_char_mods_callback(WindowID id, window_char_mods_callback value)
{
	return reinterpret_cast<window_char_mods_callback>(
		glfwSetCharModsCallback((GLFWwindow *)id,
			reinterpret_cast<GLFWcharmodsfun>(value)));
}

window_close_callback DisplayContextWindows::window_set_close_callback(WindowID id, window_close_callback value)
{
	return reinterpret_cast<window_close_callback>(
		glfwSetWindowCloseCallback((GLFWwindow *)id,
			reinterpret_cast<GLFWwindowclosefun>(value)));
}

window_content_scale_callback DisplayContextWindows::window_set_content_scale_callback(WindowID id, window_content_scale_callback value)
{
	return reinterpret_cast<window_content_scale_callback>(
		glfwSetWindowContentScaleCallback((GLFWwindow *)id,
			reinterpret_cast<GLFWwindowcontentscalefun>(value)));
}

window_drop_callback DisplayContextWindows::window_set_drop_callback(WindowID id, window_drop_callback value)
{
	return reinterpret_cast<window_drop_callback>(
		glfwSetDropCallback((GLFWwindow *)id,
			reinterpret_cast<GLFWdropfun>(value)));
}

window_focus_callback DisplayContextWindows::window_set_focus_callback(WindowID id, window_focus_callback value)
{
	return reinterpret_cast<window_focus_callback>(
		glfwSetWindowFocusCallback((GLFWwindow *)id,
			reinterpret_cast<GLFWwindowfocusfun>(value)));
}

window_framebuffer_resize_callback DisplayContextWindows::window_set_framebuffer_resize_callback(WindowID id, window_framebuffer_resize_callback value)
{
	return reinterpret_cast<window_framebuffer_resize_callback>(
		glfwSetFramebufferSizeCallback((GLFWwindow *)id,
			reinterpret_cast<GLFWframebuffersizefun>(value)));
}

window_iconify_callback DisplayContextWindows::window_set_iconify_callback(WindowID id, window_iconify_callback value)
{
	return reinterpret_cast<window_iconify_callback>(
		glfwSetWindowIconifyCallback((GLFWwindow *)id,
			reinterpret_cast<GLFWwindowiconifyfun>(value)));
}

window_key_callback DisplayContextWindows::window_set_key_callback(WindowID id, window_key_callback value)
{
	return reinterpret_cast<window_key_callback>(
		glfwSetKeyCallback((GLFWwindow *)id,
			reinterpret_cast<GLFWkeyfun>(value)));
}

window_maximize_callback DisplayContextWindows::window_set_maximize_callback(WindowID id, window_maximize_callback value)
{
	return reinterpret_cast<window_maximize_callback>(
		glfwSetWindowMaximizeCallback((GLFWwindow *)id,
			reinterpret_cast<GLFWwindowmaximizefun>(value)));
}

window_mouse_button_callback DisplayContextWindows::window_set_mouse_button_callback(WindowID id, window_mouse_button_callback value)
{
	return reinterpret_cast<window_mouse_button_callback>(
		glfwSetMouseButtonCallback((GLFWwindow *)id,
			reinterpret_cast<GLFWmousebuttonfun>(value)));
}

window_mouse_enter_callback DisplayContextWindows::window_set_mouse_enter_callback(WindowID id, window_mouse_enter_callback value)
{
	return reinterpret_cast<window_mouse_enter_callback>(
		glfwSetCursorEnterCallback((GLFWwindow *)id,
			reinterpret_cast<GLFWcursorenterfun>(value)));
}

window_mouse_pos_callback DisplayContextWindows::window_set_mouse_pos_callback(WindowID id, window_mouse_pos_callback value)
{
	return reinterpret_cast<window_mouse_pos_callback>(
		glfwSetCursorPosCallback((GLFWwindow *)id,
			reinterpret_cast<GLFWcursorposfun>(value)));
}

window_position_callback DisplayContextWindows::window_set_position_callback(WindowID id, window_position_callback value)
{
	return reinterpret_cast<window_position_callback>(
		glfwSetWindowPosCallback((GLFWwindow *)id,
			reinterpret_cast<GLFWwindowposfun>(value)));
}

window_refresh_callback DisplayContextWindows::window_set_refresh_callback(WindowID id, window_refresh_callback value)
{
	return reinterpret_cast<window_refresh_callback>(
		glfwSetWindowRefreshCallback((GLFWwindow *)id,
			reinterpret_cast<GLFWwindowrefreshfun>(value)));
}

window_resize_callback DisplayContextWindows::window_set_resize_callback(WindowID id, window_resize_callback value)
{
	return reinterpret_cast<window_resize_callback>(
		glfwSetWindowSizeCallback((GLFWwindow *)id,
			reinterpret_cast<GLFWwindowposfun>(value)));
}

window_scroll_callback DisplayContextWindows::window_set_scroll_callback(WindowID id, window_scroll_callback value)
{
	return reinterpret_cast<window_scroll_callback>(
		glfwSetScrollCallback((GLFWwindow *)id,
			reinterpret_cast<GLFWscrollfun>(value)));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

VideoMode const & DisplayContextWindows::get_desktop_video_mode() const
{
	RETURN_STATIC(VideoMode{});
}

Vector<VideoMode> const & DisplayContextWindows::get_fullscreen_video_modes() const
{
	RETURN_STATIC(Vector<VideoMode>{});
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */