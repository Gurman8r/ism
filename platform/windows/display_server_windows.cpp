#include <platform/windows/display_server_windows.hpp>
#include <scene/main/scene_tree.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <glfw/glfw3.h>
#if defined(SYSTEM_WINDOWS)
#	undef APIENTRY
#	include <Windows.h>
#	define GLFW_EXPOSE_NATIVE_WIN32
#	include <glfw/glfw3native.h>
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

using namespace ism;

OBJECT_EMBED(DisplayServerWindows, t) {}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

DisplayServerWindows::DisplayServerWindows()
{
	VERIFY(glfwInit() == GLFW_TRUE);
}

DisplayServerWindows::~DisplayServerWindows()
{
	glfwTerminate();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

Window * DisplayServerWindows::window_new(WindowSettings const & settings)
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
	Window * w{ memnew(Window((WindowID)glfwCreateWindow(
		(int32_t)settings.video.size[0],
		(int32_t)settings.video.size[1],
		(cstring)settings.title.c_str(),
		(GLFWmonitor *)settings.monitor,
		(GLFWwindow *)settings.share))) };

	w->make_context_current();
	w->set_user_pointer(w);
	w->set_position((Vec2(1920, 1080) - settings.video.size) / 2);
	if (flag_read(settings.hints, WindowHints_Maximized)) { w->maximize(); }

	return w;
}

WindowID DisplayServerWindows::get_current_context() const
{
	return (WindowID)glfwGetCurrentContext();
}

void DisplayServerWindows::make_context_current(WindowID id)
{
	glfwMakeContextCurrent((GLFWwindow *)id);
}

void DisplayServerWindows::poll_events()
{
	glfwPollEvents();
}

void DisplayServerWindows::swap_buffers(WindowID id)
{
	glfwSwapBuffers((GLFWwindow *)id);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void DisplayServerWindows::focus_window(WindowID id)
{
	glfwFocusWindow((GLFWwindow *)id);
}

void DisplayServerWindows::hide_window(WindowID id)
{
	glfwHideWindow((GLFWwindow *)id);
}

void DisplayServerWindows::iconify_window(WindowID id)
{
	glfwIconifyWindow((GLFWwindow *)id);
}

void DisplayServerWindows::maximize_window(WindowID id)
{
	glfwMaximizeWindow((GLFWwindow *)id);
}

void DisplayServerWindows::restore_window(WindowID id)
{
	glfwRestoreWindow((GLFWwindow *)id);
}

void DisplayServerWindows::request_window_attention(WindowID id)
{
	glfwRequestWindowAttention((GLFWwindow *)id);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

String DisplayServerWindows::window_get_clipboard(WindowID id) const
{
	return glfwGetClipboardString((GLFWwindow *)id);
}

Vec2 DisplayServerWindows::window_get_content_scale(WindowID id) const
{
	float_t x, y;
	glfwGetWindowContentScale((GLFWwindow *)id, &x, &y);
	return Vec2{ x, y };
}

Vec2 DisplayServerWindows::window_get_framebuffer_size(WindowID id) const
{
	int32_t x, y;
	glfwGetFramebufferSize((GLFWwindow *)id, &x, &y);
	return Vec2{ x, y };
}

int32_t DisplayServerWindows::window_get_input_mode(WindowID id, int32_t value) const
{
	return glfwGetInputMode((GLFWwindow *)id, value);
}

int32_t DisplayServerWindows::window_get_key(WindowID id, int32_t value) const
{
	return glfwGetKey((GLFWwindow *)id, value);
}

int32_t DisplayServerWindows::window_get_mouse_button(WindowID id, int32_t value) const
{
	return glfwGetMouseButton((GLFWwindow *)id, value);
}

Vec2 DisplayServerWindows::window_get_mouse_pos(WindowID id) const
{
	double_t x, y;
	glfwGetCursorPos((GLFWwindow *)id, &x, &y);
	return Vec2{ x, y };
}

WindowID DisplayServerWindows::window_get_native_handle(WindowID id) const
{
#ifdef SYSTEM_WINDOWS
	return (WindowID)glfwGetWin32Window((GLFWwindow *)id);
#else
	return (WindowID)(GLFWwindow *)id;
#endif
}

float_t DisplayServerWindows::window_get_opacity(WindowID id) const
{
	return glfwGetWindowOpacity((GLFWwindow *)id);
}

Vec2 DisplayServerWindows::window_get_position(WindowID id) const
{
	int32_t x, y;
	glfwGetWindowPos((GLFWwindow *)id, &x, &y);
	return Vec2{ x, y };
}

Vec2 DisplayServerWindows::window_get_size(WindowID id) const
{
	int32_t x, y;
	glfwGetWindowSize((GLFWwindow *)id, &x, &y);
	return Vec2{ x, y };
}

void * DisplayServerWindows::window_get_user_pointer(WindowID id) const
{
	return glfwGetWindowUserPointer((GLFWwindow *)id);
}

Rect DisplayServerWindows::window_get_frame_size(WindowID id) const
{
	int32_t l, t, r, b;
	glfwGetWindowFrameSize((GLFWwindow *)id, &l, &r, &t, &b);
	return { l, t, r - l, b - t };
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool DisplayServerWindows::window_is_auto_iconify(WindowID id) const
{
	return id && glfwGetWindowAttrib((GLFWwindow *)id, GLFW_AUTO_ICONIFY);
}

bool DisplayServerWindows::window_is_decorated(WindowID id) const
{
	return id && glfwGetWindowAttrib((GLFWwindow *)id, GLFW_DECORATED);
}

bool DisplayServerWindows::window_is_floating(WindowID id) const
{
	return id && glfwGetWindowAttrib((GLFWwindow *)id, GLFW_FLOATING);
}

bool DisplayServerWindows::window_is_focused(WindowID id) const
{
	return id && glfwGetWindowAttrib((GLFWwindow *)id, GLFW_FOCUS_ON_SHOW);
}

bool DisplayServerWindows::window_is_focus_on_show(WindowID id) const
{
	return id && glfwGetWindowAttrib((GLFWwindow *)id, GLFW_FOCUSED);
}

bool DisplayServerWindows::window_is_hovered(WindowID id) const
{
	return id && glfwGetWindowAttrib((GLFWwindow *)id, GLFW_HOVERED);
}

bool DisplayServerWindows::window_is_iconified(WindowID id) const
{
	return id && glfwGetWindowAttrib((GLFWwindow *)id, GLFW_ICONIFIED);
}

bool DisplayServerWindows::window_is_maximized(WindowID id) const
{
	return id && glfwGetWindowAttrib((GLFWwindow *)id, GLFW_MAXIMIZED);
}

bool DisplayServerWindows::window_is_open(WindowID id) const
{
	return id && !glfwWindowShouldClose((GLFWwindow *)id);
}

bool DisplayServerWindows::window_is_resizable(WindowID id) const
{
	return id && glfwGetWindowAttrib((GLFWwindow *)id, GLFW_RESIZABLE);
}

bool DisplayServerWindows::window_is_transparent(WindowID id) const
{
	return id && glfwGetWindowAttrib((GLFWwindow *)id, GLFW_TRANSPARENT_FRAMEBUFFER);
}

bool DisplayServerWindows::window_is_visible(WindowID id) const
{
	return id && glfwGetWindowAttrib((GLFWwindow *)id, GLFW_VISIBLE);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void DisplayServerWindows::window_set_auto_iconify(WindowID id, bool value)
{
	glfwSetWindowAttrib((GLFWwindow *)id, GLFW_AUTO_ICONIFY, value);
}

void DisplayServerWindows::window_set_clipboard(WindowID id, String const & value)
{
	glfwSetClipboardString((GLFWwindow *)id, value.c_str());
}

void DisplayServerWindows::window_set_cursor(WindowID id, CursorID value)
{
	glfwSetCursor((GLFWwindow *)id, (GLFWcursor *)value);
}

void DisplayServerWindows::window_set_cursor_mode(WindowID id, int32_t value)
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

void DisplayServerWindows::window_set_mouse_pos(WindowID id, Vec2 const & value)
{
	glfwSetCursorPos((GLFWwindow *)id, value[0], value[1]);
}

void DisplayServerWindows::window_set_decorated(WindowID id, bool value)
{
	glfwSetWindowAttrib((GLFWwindow *)id, GLFW_DECORATED, value);
}

void DisplayServerWindows::window_set_floating(WindowID id, bool value)
{
	glfwSetWindowAttrib((GLFWwindow *)id, GLFW_FLOATING, value);
}

void DisplayServerWindows::window_set_focus_on_show(WindowID id, bool value)
{
	glfwSetWindowAttrib((GLFWwindow *)id, GLFW_FOCUS_ON_SHOW, value);
}

void DisplayServerWindows::window_set_icons(WindowID id, int32_t width, int32_t height, uint8_t * pixels, int32_t count)
{
	GLFWimage img{ width, height, pixels };

	glfwSetWindowIcon((GLFWwindow *)id, count, &img);
}

void DisplayServerWindows::window_set_input_mode(WindowID id, int32_t mode, int32_t value)
{
	glfwSetInputMode((GLFWwindow *)id, mode, value);
}

void DisplayServerWindows::window_set_opacity(WindowID id, float_t value)
{
	glfwSetWindowOpacity((GLFWwindow *)id, value);
}

void DisplayServerWindows::window_set_position(WindowID id, Vec2 const & value)
{
	glfwSetWindowPos((GLFWwindow *)id, (int32_t)value[0], (int32_t)value[1]);
}

void DisplayServerWindows::window_set_monitor(WindowID id, MonitorID monitor, Rect const & bounds)
{
}

void DisplayServerWindows::window_set_resizable(WindowID id, bool value)
{
	glfwSetWindowAttrib((GLFWwindow *)id, GLFW_RESIZABLE, value);
}

void DisplayServerWindows::window_set_should_close(WindowID id, bool value)
{
	glfwSetWindowShouldClose((GLFWwindow *)id, value);
}

void DisplayServerWindows::window_set_size(WindowID id, Vec2 const & value)
{
	glfwSetWindowSize((GLFWwindow *)id, (int32_t)value[0], (int32_t)value[1]);
}

void DisplayServerWindows::window_set_title(WindowID id, String const & value)
{
	glfwSetWindowTitle((GLFWwindow *)id, value.c_str());
}

void DisplayServerWindows::window_set_user_pointer(WindowID id, void * value)
{
	glfwSetWindowUserPointer((GLFWwindow *)id, value);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

WindowCharCallback DisplayServerWindows::window_set_char_callback(WindowID id, WindowCharCallback value)
{
	return reinterpret_cast<WindowCharCallback>(
		glfwSetCharCallback((GLFWwindow *)id,
			reinterpret_cast<GLFWcharfun>(value)));
}

WindowCharModsCallback DisplayServerWindows::window_set_char_mods_callback(WindowID id, WindowCharModsCallback value)
{
	return reinterpret_cast<WindowCharModsCallback>(
		glfwSetCharModsCallback((GLFWwindow *)id,
			reinterpret_cast<GLFWcharmodsfun>(value)));
}

WindowCloseCallback DisplayServerWindows::window_set_close_callback(WindowID id, WindowCloseCallback value)
{
	return reinterpret_cast<WindowCloseCallback>(
		glfwSetWindowCloseCallback((GLFWwindow *)id,
			reinterpret_cast<GLFWwindowclosefun>(value)));
}

WindowContentCallback DisplayServerWindows::window_set_content_scale_callback(WindowID id, WindowContentCallback value)
{
	return reinterpret_cast<WindowContentCallback>(
		glfwSetWindowContentScaleCallback((GLFWwindow *)id,
			reinterpret_cast<GLFWwindowcontentscalefun>(value)));
}

WindowDropCallback DisplayServerWindows::window_set_drop_callback(WindowID id, WindowDropCallback value)
{
	return reinterpret_cast<WindowDropCallback>(
		glfwSetDropCallback((GLFWwindow *)id,
			reinterpret_cast<GLFWdropfun>(value)));
}

WindowFocusCallback DisplayServerWindows::window_set_focus_callback(WindowID id, WindowFocusCallback value)
{
	return reinterpret_cast<WindowFocusCallback>(
		glfwSetWindowFocusCallback((GLFWwindow *)id,
			reinterpret_cast<GLFWwindowfocusfun>(value)));
}

WindowFramebufferResizeCallback DisplayServerWindows::window_set_framebuffer_resize_callback(WindowID id, WindowFramebufferResizeCallback value)
{
	return reinterpret_cast<WindowFramebufferResizeCallback>(
		glfwSetFramebufferSizeCallback((GLFWwindow *)id,
			reinterpret_cast<GLFWframebuffersizefun>(value)));
}

WindowIconifyCallback DisplayServerWindows::window_set_iconify_callback(WindowID id, WindowIconifyCallback value)
{
	return reinterpret_cast<WindowIconifyCallback>(
		glfwSetWindowIconifyCallback((GLFWwindow *)id,
			reinterpret_cast<GLFWwindowiconifyfun>(value)));
}

WindowKeyCallback DisplayServerWindows::window_set_key_callback(WindowID id, WindowKeyCallback value)
{
	return reinterpret_cast<WindowKeyCallback>(
		glfwSetKeyCallback((GLFWwindow *)id,
			reinterpret_cast<GLFWkeyfun>(value)));
}

WindowMaximizeCallback DisplayServerWindows::window_set_maximize_callback(WindowID id, WindowMaximizeCallback value)
{
	return reinterpret_cast<WindowMaximizeCallback>(
		glfwSetWindowMaximizeCallback((GLFWwindow *)id,
			reinterpret_cast<GLFWwindowmaximizefun>(value)));
}

WindowMouseButtonCallback DisplayServerWindows::window_set_mouse_button_callback(WindowID id, WindowMouseButtonCallback value)
{
	return reinterpret_cast<WindowMouseButtonCallback>(
		glfwSetMouseButtonCallback((GLFWwindow *)id,
			reinterpret_cast<GLFWmousebuttonfun>(value)));
}

WindowMouseEnterCallback DisplayServerWindows::window_set_mouse_enter_callback(WindowID id, WindowMouseEnterCallback value)
{
	return reinterpret_cast<WindowMouseEnterCallback>(
		glfwSetCursorEnterCallback((GLFWwindow *)id,
			reinterpret_cast<GLFWcursorenterfun>(value)));
}

WindowMousePosCallback DisplayServerWindows::window_set_mouse_pos_callback(WindowID id, WindowMousePosCallback value)
{
	return reinterpret_cast<WindowMousePosCallback>(
		glfwSetCursorPosCallback((GLFWwindow *)id,
			reinterpret_cast<GLFWcursorposfun>(value)));
}

WindowMouseScrollCallback DisplayServerWindows::window_set_mouse_scroll_callback(WindowID id, WindowMouseScrollCallback value)
{
	return reinterpret_cast<WindowMouseScrollCallback>(
		glfwSetScrollCallback((GLFWwindow *)id,
			reinterpret_cast<GLFWscrollfun>(value)));
}

WindowPositionCallback DisplayServerWindows::window_set_position_callback(WindowID id, WindowPositionCallback value)
{
	return reinterpret_cast<WindowPositionCallback>(
		glfwSetWindowPosCallback((GLFWwindow *)id,
			reinterpret_cast<GLFWwindowposfun>(value)));
}

WindowRefreshCallback DisplayServerWindows::window_set_refresh_callback(WindowID id, WindowRefreshCallback value)
{
	return reinterpret_cast<WindowRefreshCallback>(
		glfwSetWindowRefreshCallback((GLFWwindow *)id,
			reinterpret_cast<GLFWwindowrefreshfun>(value)));
}

WindowResizeCallback DisplayServerWindows::window_set_resize_callback(WindowID id, WindowResizeCallback value)
{
	return reinterpret_cast<WindowResizeCallback>(
		glfwSetWindowSizeCallback((GLFWwindow *)id,
			reinterpret_cast<GLFWwindowposfun>(value)));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

VideoMode const & DisplayServerWindows::get_desktop_video_mode() const
{
	RETURN_STATIC(VideoMode{});
}

Vector<VideoMode> const & DisplayServerWindows::get_fullscreen_video_modes() const
{
	RETURN_STATIC(Vector<VideoMode>{});
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */