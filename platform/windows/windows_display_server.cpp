#include <platform/windows/windows_display_server.hpp>
#include <scene/main/scene_tree.hpp>

#include <glfw/glfw3.h>
#if defined(ISM_OS_WINDOWS)
#	undef APIENTRY
#	include <Windows.h>
#	define GLFW_EXPOSE_NATIVE_WIN32
#	include <glfw/glfw3native.h>
#endif

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Window * Windows_DisplayServer::create_window(SceneTree * tree, Node * parent, WindowSettings const & settings)
	{
		// context hints
		glfwWindowHint(GLFW_CLIENT_API, std::invoke([&]() noexcept {
			switch (settings.context.api) {
			default					: return GLFW_NO_API;
			case ContextAPI_OpenGL	: return GLFW_OPENGL_API;
			}
		}));
		glfwWindowHint(GLFW_OPENGL_PROFILE, std::invoke([&]() noexcept {
			switch (settings.context.profile) {
			default						: return GLFW_OPENGL_ANY_PROFILE;
			case ContextProfile_Core	: return GLFW_OPENGL_CORE_PROFILE;
			case ContextProfile_Compat	: return GLFW_OPENGL_COMPAT_PROFILE;
			case ContextProfile_Debug	: return GLFW_OPENGL_DEBUG_CONTEXT;
			}
		}));
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, settings.context.major);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, settings.context.minor);

		// style hints
		glfwWindowHint(GLFW_AUTO_ICONIFY,	settings.hints & WindowHints_AutoIconify);
		glfwWindowHint(GLFW_CENTER_CURSOR,	settings.hints & WindowHints_CenterCursor);
		glfwWindowHint(GLFW_DECORATED,		settings.hints & WindowHints_Decorated);
		glfwWindowHint(GLFW_DOUBLEBUFFER,	settings.hints & WindowHints_Doublebuffer);
		glfwWindowHint(GLFW_FLOATING,		settings.hints & WindowHints_Floating);
		glfwWindowHint(GLFW_FOCUS_ON_SHOW,	settings.hints & WindowHints_FocusOnShow);
		glfwWindowHint(GLFW_FOCUSED,		settings.hints & WindowHints_Focused);
		glfwWindowHint(GLFW_MAXIMIZED,		settings.hints & WindowHints_Maximized);
		glfwWindowHint(GLFW_RESIZABLE,		settings.hints & WindowHints_Resizable);
		glfwWindowHint(GLFW_VISIBLE,		settings.hints & WindowHints_Visible);

		// monitor hints
		glfwWindowHint(GLFW_REFRESH_RATE, settings.video.rate);

		// framebuffer hints
		glfwWindowHint(GLFW_RED_BITS,		(int32_t)settings.video.bpp[0]);
		glfwWindowHint(GLFW_GREEN_BITS,		(int32_t)settings.video.bpp[1]);
		glfwWindowHint(GLFW_BLUE_BITS,		(int32_t)settings.video.bpp[2]);
		glfwWindowHint(GLFW_ALPHA_BITS,		(int32_t)settings.video.bpp[3]);
		glfwWindowHint(GLFW_DEPTH_BITS,		settings.context.depth_bits);
		glfwWindowHint(GLFW_STENCIL_BITS,	settings.context.stencil_bits);
		glfwWindowHint(GLFW_SRGB_CAPABLE,	settings.context.srgb_capable);

		// create window
		return memnew(Window(tree, parent, (WindowID)glfwCreateWindow(
			(int32_t)settings.video.size[0],
			(int32_t)settings.video.size[1],
			(cstring)settings.title.c_str(),
			(GLFWmonitor *)settings.monitor,
			(GLFWwindow *)settings.share),
			settings.hints));
	}

	bool Windows_DisplayServer::initialize()
	{
		return glfwInit() == GLFW_TRUE;
	}

	void Windows_DisplayServer::finalize()
	{
		glfwTerminate();
	}

	void Windows_DisplayServer::poll_events()
	{
		glfwPollEvents();
	}

	void Windows_DisplayServer::swap_buffers(WindowID id)
	{
		glfwSwapBuffers((GLFWwindow *)id);
	}

	WindowID Windows_DisplayServer::get_current_context() const
	{
		return (WindowID)glfwGetCurrentContext();
	}

	void Windows_DisplayServer::make_context_current(WindowID id)
	{
		glfwMakeContextCurrent((GLFWwindow *)id);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Windows_DisplayServer::focus_window(WindowID id)
	{
		glfwFocusWindow((GLFWwindow *)id);
	}

	void Windows_DisplayServer::hide_window(WindowID id)
	{
		glfwHideWindow((GLFWwindow *)id);
	}

	void Windows_DisplayServer::iconify_window(WindowID id)
	{
		glfwIconifyWindow((GLFWwindow *)id);
	}

	void Windows_DisplayServer::maximize_window(WindowID id)
	{
		glfwMaximizeWindow((GLFWwindow *)id);
	}

	void Windows_DisplayServer::restore_window(WindowID id)
	{
		glfwRestoreWindow((GLFWwindow *)id);
	}

	void Windows_DisplayServer::request_window_attention(WindowID id)
	{
		glfwRequestWindowAttention((GLFWwindow *)id);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	String Windows_DisplayServer::window_get_clipboard(WindowID id) const
	{
		return glfwGetClipboardString((GLFWwindow *)id);
	}

	Vec2 Windows_DisplayServer::window_get_content_scale(WindowID id) const
	{
		float_t x, y;
		glfwGetWindowContentScale((GLFWwindow *)id, &x, &y);
		return Vec2{ x, y };
	}

	Vec2 Windows_DisplayServer::window_get_framebuffer_size(WindowID id) const
	{
		int32_t x, y;
		glfwGetFramebufferSize((GLFWwindow *)id, &x, &y);
		return Vec2{ x, y };
	}

	int32_t Windows_DisplayServer::window_get_input_mode(WindowID id, int32_t value) const
	{
		return glfwGetInputMode((GLFWwindow *)id, value);
	}

	int32_t Windows_DisplayServer::window_get_key(WindowID id, int32_t value) const
	{
		return glfwGetKey((GLFWwindow *)id, value);
	}

	int32_t Windows_DisplayServer::window_get_mouse_button(WindowID id, int32_t value) const
	{
		return glfwGetMouseButton((GLFWwindow *)id, value);
	}

	Vec2 Windows_DisplayServer::window_get_mouse_pos(WindowID id) const
	{
		double_t x, y;
		glfwGetCursorPos((GLFWwindow *)id, &x, &y);
		return Vec2{ x, y };
	}

	WindowID Windows_DisplayServer::window_get_native_handle(WindowID id) const
	{
#ifdef ISM_OS_WINDOWS
		return (WindowID)glfwGetWin32Window((GLFWwindow *)id);
#else
		return (WindowID)(GLFWwindow *)id;
#endif
	}

	float_t Windows_DisplayServer::window_get_opacity(WindowID id) const
	{
		return glfwGetWindowOpacity((GLFWwindow *)id);
	}

	Vec2 Windows_DisplayServer::window_get_position(WindowID id) const
	{
		int32_t x, y;
		glfwGetWindowPos((GLFWwindow *)id, &x, &y);
		return Vec2{ x, y };
	}

	Vec2 Windows_DisplayServer::window_get_size(WindowID id) const
	{
		int32_t x, y;
		glfwGetWindowSize((GLFWwindow *)id, &x, &y);
		return Vec2{ x, y };
	}

	void * Windows_DisplayServer::window_get_user_pointer(WindowID id) const
	{
		return glfwGetWindowUserPointer((GLFWwindow *)id);
	}

	Rect Windows_DisplayServer::window_get_frame_size(WindowID id) const
	{
		int32_t l, t, r, b;
		glfwGetWindowFrameSize((GLFWwindow *)id, &l, &r, &t, &b);
		return { l, t, r - l, b - t };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Windows_DisplayServer::window_is_auto_iconify(WindowID id) const
	{
		return id && glfwGetWindowAttrib((GLFWwindow *)id, GLFW_AUTO_ICONIFY);
	}

	bool Windows_DisplayServer::window_is_decorated(WindowID id) const
	{
		return id && glfwGetWindowAttrib((GLFWwindow *)id, GLFW_DECORATED);
	}

	bool Windows_DisplayServer::window_is_floating(WindowID id) const
	{
		return id && glfwGetWindowAttrib((GLFWwindow *)id, GLFW_FLOATING);
	}

	bool Windows_DisplayServer::window_is_focused(WindowID id) const
	{
		return id && glfwGetWindowAttrib((GLFWwindow *)id, GLFW_FOCUS_ON_SHOW);
	}

	bool Windows_DisplayServer::window_is_focus_on_show(WindowID id) const
	{
		return id && glfwGetWindowAttrib((GLFWwindow *)id, GLFW_FOCUSED);
	}

	bool Windows_DisplayServer::window_is_hovered(WindowID id) const
	{
		return id && glfwGetWindowAttrib((GLFWwindow *)id, GLFW_HOVERED);
	}

	bool Windows_DisplayServer::window_is_iconified(WindowID id) const
	{
		return id && glfwGetWindowAttrib((GLFWwindow *)id, GLFW_ICONIFIED);
	}

	bool Windows_DisplayServer::window_is_maximized(WindowID id) const
	{
		return id && glfwGetWindowAttrib((GLFWwindow *)id, GLFW_MAXIMIZED);
	}

	bool Windows_DisplayServer::window_is_open(WindowID id) const
	{
		return id && !glfwWindowShouldClose((GLFWwindow *)id);
	}

	bool Windows_DisplayServer::window_is_resizable(WindowID id) const
	{
		return id && glfwGetWindowAttrib((GLFWwindow *)id, GLFW_RESIZABLE);
	}

	bool Windows_DisplayServer::window_is_transparent(WindowID id) const
	{
		return id && glfwGetWindowAttrib((GLFWwindow *)id, GLFW_TRANSPARENT_FRAMEBUFFER);
	}

	bool Windows_DisplayServer::window_is_visible(WindowID id) const
	{
		return id && glfwGetWindowAttrib((GLFWwindow *)id, GLFW_VISIBLE);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Windows_DisplayServer::window_set_auto_iconify(WindowID id, bool value)
	{
		glfwSetWindowAttrib((GLFWwindow *)id, GLFW_AUTO_ICONIFY, value);
	}

	void Windows_DisplayServer::window_set_clipboard(WindowID id, String const & value)
	{
		glfwSetClipboardString((GLFWwindow *)id, value.c_str());
	}

	void Windows_DisplayServer::window_set_cursor(WindowID id, CursorID value)
	{
		glfwSetCursor((GLFWwindow *)id, (GLFWcursor *)value);
	}

	void Windows_DisplayServer::window_set_cursor_mode(WindowID id, int32_t value)
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

	void Windows_DisplayServer::window_set_mouse_pos(WindowID id, Vec2 const & value)
	{
		glfwSetCursorPos((GLFWwindow *)id, value[0], value[1]);
	}

	void Windows_DisplayServer::window_set_decorated(WindowID id, bool value)
	{
		glfwSetWindowAttrib((GLFWwindow *)id, GLFW_DECORATED, value);
	}

	void Windows_DisplayServer::window_set_floating(WindowID id, bool value)
	{
		glfwSetWindowAttrib((GLFWwindow *)id, GLFW_FLOATING, value);
	}

	void Windows_DisplayServer::window_set_focus_on_show(WindowID id, bool value)
	{
		glfwSetWindowAttrib((GLFWwindow *)id, GLFW_FOCUS_ON_SHOW, value);
	}

	void Windows_DisplayServer::window_set_icons(WindowID id, int32_t width, int32_t height, uint8_t * pixels, int32_t count)
	{
		GLFWimage img{ width, height, pixels };

		glfwSetWindowIcon((GLFWwindow *)id, count, &img);
	}

	void Windows_DisplayServer::window_set_input_mode(WindowID id, int32_t mode, int32_t value)
	{
		glfwSetInputMode((GLFWwindow *)id, mode, value);
	}

	void Windows_DisplayServer::window_set_opacity(WindowID id, float_t value)
	{
		glfwSetWindowOpacity((GLFWwindow *)id, value);
	}

	void Windows_DisplayServer::window_set_position(WindowID id, Vec2 const & value)
	{
		glfwSetWindowPos((GLFWwindow *)id, (int32_t)value[0], (int32_t)value[1]);
	}

	void Windows_DisplayServer::window_set_monitor(WindowID id, MonitorID monitor, Rect const & bounds)
	{
	}

	void Windows_DisplayServer::window_set_resizable(WindowID id, bool value)
	{
		glfwSetWindowAttrib((GLFWwindow *)id, GLFW_RESIZABLE, value);
	}

	void Windows_DisplayServer::window_set_should_close(WindowID id, bool value)
	{
		glfwSetWindowShouldClose((GLFWwindow *)id, value);
	}

	void Windows_DisplayServer::window_set_size(WindowID id, Vec2 const & value)
	{
		glfwSetWindowSize((GLFWwindow *)id, (int32_t)value[0], (int32_t)value[1]);
	}

	void Windows_DisplayServer::window_set_title(WindowID id, String const & value)
	{
		glfwSetWindowTitle((GLFWwindow *)id, value.c_str());
	}

	void Windows_DisplayServer::window_set_user_pointer(WindowID id, void * value)
	{
		glfwSetWindowUserPointer((GLFWwindow *)id, value);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	window_char_callback Windows_DisplayServer::window_get_char_callback(WindowID id) const
	{
		return window_char_callback{};
	}

	window_char_mods_callback Windows_DisplayServer::window_get_char_mods_callback(WindowID id) const
	{
		return window_char_mods_callback{};
	}

	window_close_callback Windows_DisplayServer::window_get_close_callback(WindowID id) const
	{
		return window_close_callback{};
	}

	window_content_scale_callback Windows_DisplayServer::window_get_content_scale_callback(WindowID id) const
	{
		return window_content_scale_callback{};
	}

	window_mouse_enter_callback Windows_DisplayServer::window_get_mouse_enter_callback(WindowID id) const
	{
		return window_mouse_enter_callback{};
	}

	window_mouse_pos_callback Windows_DisplayServer::window_get_mouse_pos_callback(WindowID id) const
	{
		return window_mouse_pos_callback{};
	}

	window_drop_callback Windows_DisplayServer::window_get_drop_callback(WindowID id) const
	{
		return window_drop_callback{};
	}

	window_focus_callback Windows_DisplayServer::window_get_focus_callback(WindowID id) const
	{
		return window_focus_callback{};
	}

	window_framebuffer_resize_callback Windows_DisplayServer::window_get_framebuffer_resize_callback(WindowID id) const
	{
		return window_framebuffer_resize_callback{};
	}

	window_iconify_callback Windows_DisplayServer::window_get_iconify_callback(WindowID id) const
	{
		return window_iconify_callback{};
	}

	window_key_callback Windows_DisplayServer::window_get_key_callback(WindowID id) const
	{
		return window_key_callback{};
	}

	window_maximize_callback Windows_DisplayServer::window_get_maximize_callback(WindowID id) const
	{
		return window_maximize_callback{};
	}

	window_mouse_button_callback Windows_DisplayServer::window_get_mouse_button_callback(WindowID id) const
	{
		return window_mouse_button_callback{};
	}

	window_position_callback Windows_DisplayServer::window_get_position_callback(WindowID id) const
	{
		return window_position_callback{};
	}

	window_refresh_callback Windows_DisplayServer::window_get_refresh_callback(WindowID id) const
	{
		return window_refresh_callback{};
	}

	window_resize_callback Windows_DisplayServer::window_get_resize_callback(WindowID id) const
	{
		return window_resize_callback{};
	}

	window_scroll_callback Windows_DisplayServer::window_get_scroll_callback(WindowID id) const
	{
		return window_scroll_callback{};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	window_char_callback Windows_DisplayServer::window_set_char_callback(WindowID id, window_char_callback value)
	{
		return reinterpret_cast<window_char_callback>(
			glfwSetCharCallback((GLFWwindow *)id,
				reinterpret_cast<GLFWcharfun>(value)));
	}

	window_char_mods_callback Windows_DisplayServer::window_set_char_mods_callback(WindowID id, window_char_mods_callback value)
	{
		return reinterpret_cast<window_char_mods_callback>(
			glfwSetCharModsCallback((GLFWwindow *)id,
				reinterpret_cast<GLFWcharmodsfun>(value)));
	}

	window_close_callback Windows_DisplayServer::window_set_close_callback(WindowID id, window_close_callback value)
	{
		return reinterpret_cast<window_close_callback>(
			glfwSetWindowCloseCallback((GLFWwindow *)id,
				reinterpret_cast<GLFWwindowclosefun>(value)));
	}

	window_content_scale_callback Windows_DisplayServer::window_set_content_scale_callback(WindowID id, window_content_scale_callback value)
	{
		return reinterpret_cast<window_content_scale_callback>(
			glfwSetWindowContentScaleCallback((GLFWwindow *)id,
				reinterpret_cast<GLFWwindowcontentscalefun>(value)));
	}

	window_drop_callback Windows_DisplayServer::window_set_drop_callback(WindowID id, window_drop_callback value)
	{
		return reinterpret_cast<window_drop_callback>(
			glfwSetDropCallback((GLFWwindow *)id,
				reinterpret_cast<GLFWdropfun>(value)));
	}

	window_focus_callback Windows_DisplayServer::window_set_focus_callback(WindowID id, window_focus_callback value)
	{
		return reinterpret_cast<window_focus_callback>(
			glfwSetWindowFocusCallback((GLFWwindow *)id,
				reinterpret_cast<GLFWwindowfocusfun>(value)));
	}

	window_framebuffer_resize_callback Windows_DisplayServer::window_set_framebuffer_resize_callback(WindowID id, window_framebuffer_resize_callback value)
	{
		return reinterpret_cast<window_framebuffer_resize_callback>(
			glfwSetFramebufferSizeCallback((GLFWwindow *)id,
				reinterpret_cast<GLFWframebuffersizefun>(value)));
	}

	window_iconify_callback Windows_DisplayServer::window_set_iconify_callback(WindowID id, window_iconify_callback value)
	{
		return reinterpret_cast<window_iconify_callback>(
			glfwSetWindowIconifyCallback((GLFWwindow *)id,
				reinterpret_cast<GLFWwindowiconifyfun>(value)));
	}

	window_key_callback Windows_DisplayServer::window_set_key_callback(WindowID id, window_key_callback value)
	{
		return reinterpret_cast<window_key_callback>(
			glfwSetKeyCallback((GLFWwindow *)id,
				reinterpret_cast<GLFWkeyfun>(value)));
	}

	window_maximize_callback Windows_DisplayServer::window_set_maximize_callback(WindowID id, window_maximize_callback value)
	{
		return reinterpret_cast<window_maximize_callback>(
			glfwSetWindowMaximizeCallback((GLFWwindow *)id,
				reinterpret_cast<GLFWwindowmaximizefun>(value)));
	}

	window_mouse_button_callback Windows_DisplayServer::window_set_mouse_button_callback(WindowID id, window_mouse_button_callback value)
	{
		return reinterpret_cast<window_mouse_button_callback>(
			glfwSetMouseButtonCallback((GLFWwindow *)id,
				reinterpret_cast<GLFWmousebuttonfun>(value)));
	}

	window_mouse_enter_callback Windows_DisplayServer::window_set_mouse_enter_callback(WindowID id, window_mouse_enter_callback value)
	{
		return reinterpret_cast<window_mouse_enter_callback>(
			glfwSetCursorEnterCallback((GLFWwindow *)id,
				reinterpret_cast<GLFWcursorenterfun>(value)));
	}

	window_mouse_pos_callback Windows_DisplayServer::window_set_mouse_pos_callback(WindowID id, window_mouse_pos_callback value)
	{
		return reinterpret_cast<window_mouse_pos_callback>(
			glfwSetCursorPosCallback((GLFWwindow *)id,
				reinterpret_cast<GLFWcursorposfun>(value)));
	}

	window_position_callback Windows_DisplayServer::window_set_position_callback(WindowID id, window_position_callback value)
	{
		return reinterpret_cast<window_position_callback>(
			glfwSetWindowPosCallback((GLFWwindow *)id,
				reinterpret_cast<GLFWwindowposfun>(value)));
	}

	window_refresh_callback Windows_DisplayServer::window_set_refresh_callback(WindowID id, window_refresh_callback value)
	{
		return reinterpret_cast<window_refresh_callback>(
			glfwSetWindowRefreshCallback((GLFWwindow *)id,
				reinterpret_cast<GLFWwindowrefreshfun>(value)));
	}

	window_resize_callback Windows_DisplayServer::window_set_resize_callback(WindowID id, window_resize_callback value)
	{
		return reinterpret_cast<window_resize_callback>(
			glfwSetWindowSizeCallback((GLFWwindow *)id,
				reinterpret_cast<GLFWwindowposfun>(value)));
	}

	window_scroll_callback Windows_DisplayServer::window_set_scroll_callback(WindowID id, window_scroll_callback value)
	{
		return reinterpret_cast<window_scroll_callback>(
			glfwSetScrollCallback((GLFWwindow *)id,
				reinterpret_cast<GLFWscrollfun>(value)));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}