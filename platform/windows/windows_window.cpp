#include <platform/windows/windows_window.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Windows_Window::~Windows_Window() {}

	Windows_Window * Windows_Window::new_(WindowSettings const & settings)
	{
		VERIFY(glfwInit() == GLFW_TRUE);

		Windows_Window * window{ memnew(Windows_Window(settings.tree, settings.parent)) };

		if ((window->m_title = settings.title).empty()) { window->m_title = "New Window"; }

		window->m_hints = settings.hints;

		window->m_monitor = (GLFWmonitor *)settings.monitor;

		// apply context hints
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

		// apply window hints
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

		// apply monitor hints
		glfwWindowHint(GLFW_REFRESH_RATE, settings.video.rate);

		// apply framebuffer hints
		glfwWindowHint(GLFW_RED_BITS,		(int32_t)settings.video.bpp[0]);
		glfwWindowHint(GLFW_GREEN_BITS,		(int32_t)settings.video.bpp[1]);
		glfwWindowHint(GLFW_BLUE_BITS,		(int32_t)settings.video.bpp[2]);
		glfwWindowHint(GLFW_ALPHA_BITS,		(int32_t)settings.video.bpp[3]);
		glfwWindowHint(GLFW_DEPTH_BITS,		settings.context.depth_bits);
		glfwWindowHint(GLFW_STENCIL_BITS,	settings.context.stencil_bits);
		glfwWindowHint(GLFW_SRGB_CAPABLE,	settings.context.srgb_capable);

		// create window backend
		window->m_window = glfwCreateWindow(
			(int32_t)settings.video.size[0],
			(int32_t)settings.video.size[1],
			(cstring)window->m_title.c_str(),
			(GLFWmonitor *)window->m_monitor,
			(GLFWwindow *)settings.share);

		return window;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Windows_Window::focus()
	{
		glfwFocusWindow(m_window);
	}

	void Windows_Window::hide()
	{
		glfwHideWindow(m_window);
	}

	void Windows_Window::iconify()
	{
		glfwIconifyWindow(m_window);
	}

	void Windows_Window::maximize()
	{
		glfwMaximizeWindow(m_window);
	}

	void Windows_Window::restore()
	{
		glfwRestoreWindow(m_window);
	}

	void Windows_Window::request_attention()
	{
		glfwRequestWindowAttention(m_window);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	String Windows_Window::get_clipboard() const
	{
		return glfwGetClipboardString(m_window);
	}

	Vec2 Windows_Window::get_content_scale() const
	{
		float_t x{}, y{};
		glfwGetWindowContentScale(m_window, &x, &y);
		return Vec2{ x, y };
	}

	Vec2 Windows_Window::get_framebuffer_size() const
	{
		int32_t x{}, y{};
		glfwGetFramebufferSize(m_window, &x, &y);
		return Vec2{ x, y };
	}

	WindowID Windows_Window::get_handle() const
	{
		return (WindowID)m_window;
	}

	WindowHints Windows_Window::get_hints() const
	{
		return m_hints;
	}

	int32_t Windows_Window::get_input_mode(int32_t mode) const
	{
		return glfwGetInputMode(m_window, mode);
	}

	int32_t Windows_Window::get_key(int32_t key) const
	{
		return glfwGetKey(m_window, key);
	}

	int32_t Windows_Window::get_mouse_button(int32_t button) const
	{
		return glfwGetMouseButton(m_window, button);
	}

	Vec2 Windows_Window::get_mouse_pos() const
	{
		double_t x{}, y{};
		glfwGetCursorPos(m_window, &x, &y);
		return Vec2{ x, y };
	}

	WindowID Windows_Window::get_native_handle() const
	{
#ifdef ISM_OS_WINDOWS
		return (WindowID)glfwGetWin32Window(m_window);
#else
		return (WindowID)m_window;
#endif
	}

	float_t Windows_Window::get_opacity() const
	{
		return glfwGetWindowOpacity(m_window);
	}

	Vec2 Windows_Window::get_position() const
	{
		int32_t x{}, y{};
		glfwGetWindowPos(m_window, &x, &y);
		return Vec2{ x, y };
	}

	Vec2 Windows_Window::get_size() const
	{
		int32_t x{}, y{};
		glfwGetWindowSize(m_window, &x, &y);
		return Vec2{ x, y };
	}

	String const & Windows_Window::get_title() const
	{
		return m_title;
	}

	void * Windows_Window::get_user_pointer() const
	{
		return glfwGetWindowUserPointer(m_window);
	}

	Rect Windows_Window::get_window_frame_size() const
	{
		int32_t l, t, r, b;
		glfwGetWindowFrameSize(m_window, &l, &r, &t, &b);
		return { l, t, r - l, b - t };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Windows_Window::is_auto_iconify() const
	{
		return m_window && glfwGetWindowAttrib(m_window, GLFW_AUTO_ICONIFY);
	}

	bool Windows_Window::is_decorated() const
	{
		return m_window && glfwGetWindowAttrib(m_window, GLFW_DECORATED);
	}

	bool Windows_Window::is_floating() const
	{
		return m_window && glfwGetWindowAttrib(m_window, GLFW_FLOATING);
	}

	bool Windows_Window::is_focus_on_show() const
	{
		return m_window && glfwGetWindowAttrib(m_window, GLFW_FOCUS_ON_SHOW);
	}

	bool Windows_Window::is_focused() const
	{
		return m_window && glfwGetWindowAttrib(m_window, GLFW_FOCUSED);
	}

	bool Windows_Window::is_hovered() const
	{
		return m_window && glfwGetWindowAttrib(m_window, GLFW_HOVERED);
	}

	bool Windows_Window::is_iconified() const
	{
		return m_window && glfwGetWindowAttrib(m_window, GLFW_ICONIFIED);
	}

	bool Windows_Window::is_maximized() const
	{
		return m_window && glfwGetWindowAttrib(m_window, GLFW_MAXIMIZED);
	}

	bool Windows_Window::is_open() const
	{
		return m_window && !glfwWindowShouldClose(m_window);
	}

	bool Windows_Window::is_resizable() const
	{
		return m_window && glfwGetWindowAttrib(m_window, GLFW_RESIZABLE);
	}

	bool Windows_Window::is_transparent() const
	{
		return m_window && glfwGetWindowAttrib(m_window, GLFW_TRANSPARENT_FRAMEBUFFER);
	}

	bool Windows_Window::is_visible() const
	{
		return m_window && glfwGetWindowAttrib(m_window, GLFW_VISIBLE);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Windows_Window::set_auto_iconify(bool value)
	{
		glfwSetWindowAttrib(m_window, GLFW_AUTO_ICONIFY, value);
	}

	void Windows_Window::set_clipboard(String const & value)
	{
		glfwSetClipboardString(m_window, value.c_str());
	}

	void Windows_Window::set_cursor(CursorID value)
	{
		glfwSetCursor(m_window, (GLFWcursor *)value);
	}

	void Windows_Window::set_cursor_mode(int32_t value)
	{
		set_input_mode(GLFW_CURSOR, std::invoke([value]() noexcept
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

	void Windows_Window::set_mouse_pos(Vec2 const & value)
	{
		glfwSetCursorPos(m_window, value[0], value[1]);
	}

	void Windows_Window::set_decorated(bool value)
	{
		glfwSetWindowAttrib(m_window, GLFW_DECORATED, value);
	}

	void Windows_Window::set_floating(bool value)
	{
		glfwSetWindowAttrib(m_window, GLFW_FLOATING, value);
	}

	void Windows_Window::set_focus_on_show(bool value)
	{
		glfwSetWindowAttrib(m_window, GLFW_FOCUS_ON_SHOW, value);
	}

	void Windows_Window::set_icons(int32_t n, size_t w, size_t h, byte const * p)
	{
		GLFWimage temp{ (int32_t)w, (int32_t)h, (uint8_t *)p };

		glfwSetWindowIcon(m_window, n, &temp);
	}

	void Windows_Window::set_input_mode(int32_t mode, int32_t value)
	{
		glfwSetInputMode(m_window, mode, value);
	}

	void Windows_Window::set_opacity(float_t value)
	{
		glfwSetWindowOpacity(m_window, value);
	}

	void Windows_Window::set_position(Vec2 const & value)
	{
		glfwSetWindowPos(m_window, (int32_t)value[0], (int32_t)value[1]);
	}

	void Windows_Window::set_monitor(MonitorID value, Rect const & bounds)
	{
		// WIP
		m_monitor = (GLFWmonitor *)value;

		if (m_monitor)
		{
			GLFWvidmode const * vm{ glfwGetVideoMode(m_monitor) };

			glfwSetWindowMonitor(m_window, m_monitor,
				0,
				0,
				vm->width,
				vm->height,
				vm->refreshRate);
		}
		else
		{
			glfwSetWindowMonitor(m_window, m_monitor,
				(int32_t)(bounds[0]),
				(int32_t)(bounds[1]),
				(int32_t)(bounds[2] - bounds[0]),
				(int32_t)(bounds[3] - bounds[1]),
				GLFW_DONT_CARE);
		}
	}

	void Windows_Window::set_resizable(bool value)
	{
		glfwSetWindowAttrib(m_window, GLFW_RESIZABLE, value);
	}

	void Windows_Window::set_should_close(bool value)
	{
		glfwSetWindowShouldClose(m_window, value);
	}

	void Windows_Window::set_size(Vec2 const & value)
	{
		glfwSetWindowSize(m_window, (int32_t)value[0], (int32_t)value[1]);
	}

	void Windows_Window::set_title(String const & value)
	{
		glfwSetWindowTitle(m_window, (m_title = value).c_str());
	}

	void Windows_Window::set_user_pointer(void * value)
	{
		glfwSetWindowUserPointer(m_window, value);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	window_char_callback Windows_Window::get_char_callback() const
	{
		return m_callbacks.on_char;
	}

	window_char_mods_callback Windows_Window::get_char_mods_callback() const
	{
		return m_callbacks.on_char_mods;
	}

	window_close_callback Windows_Window::get_close_callback() const
	{
		return m_callbacks.on_close;
	}

	window_content_scale_callback Windows_Window::get_content_scale_callback() const
	{
		return m_callbacks.on_content_scale;
	}

	window_mouse_enter_callback Windows_Window::get_mouse_enter_callback() const
	{
		return m_callbacks.on_mouse_enter;
	}

	window_mouse_pos_callback Windows_Window::get_mouse_pos_callback() const
	{
		return m_callbacks.on_mouse_pos;
	}

	window_drop_callback Windows_Window::get_drop_callback() const
	{
		return m_callbacks.on_drop;
	}

	window_focus_callback Windows_Window::get_focus_callback() const
	{
		return m_callbacks.on_focus;
	}

	window_framebuffer_resize_callback Windows_Window::get_framebuffer_resize_callback() const
	{
		return m_callbacks.on_framebuffer_resize;
	}

	window_iconify_callback Windows_Window::get_iconify_callback() const
	{
		return m_callbacks.on_iconify;
	}

	window_key_callback Windows_Window::get_key_callback() const
	{
		return m_callbacks.on_key;
	}

	window_maximize_callback Windows_Window::get_maximize_callback() const
	{
		return m_callbacks.on_maximize;
	}

	window_mouse_button_callback Windows_Window::get_mouse_button_callback() const
	{
		return m_callbacks.on_mouse_button;
	}

	window_position_callback Windows_Window::get_position_callback() const
	{
		return m_callbacks.on_position;
	}

	window_refresh_callback Windows_Window::get_refresh_callback() const
	{
		return m_callbacks.on_refresh;
	}

	window_resize_callback Windows_Window::get_resize_callback() const
	{
		return m_callbacks.on_resize;
	}

	window_scroll_callback Windows_Window::get_scroll_callback() const
	{
		return m_callbacks.on_mouse_wheel;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	window_char_callback Windows_Window::set_char_callback(window_char_callback value)
	{
		return reinterpret_cast<window_char_callback>(
			glfwSetCharCallback(m_window,
				reinterpret_cast<GLFWcharfun>(m_callbacks.on_char = value)));
	}

	window_char_mods_callback Windows_Window::set_char_mods_callback(window_char_mods_callback value)
	{
		return reinterpret_cast<window_char_mods_callback>(
			glfwSetCharModsCallback(m_window,
				reinterpret_cast<GLFWcharmodsfun>(m_callbacks.on_char_mods = value)));
	}

	window_close_callback Windows_Window::set_close_callback(window_close_callback value)
	{
		return reinterpret_cast<window_close_callback>(
			glfwSetWindowCloseCallback(m_window,
				reinterpret_cast<GLFWwindowclosefun>(m_callbacks.on_close = value)));
	}

	window_content_scale_callback Windows_Window::set_content_scale_callback(window_content_scale_callback value)
	{
		return reinterpret_cast<window_content_scale_callback>(
			glfwSetWindowContentScaleCallback(m_window,
				reinterpret_cast<GLFWwindowcontentscalefun>(m_callbacks.on_content_scale = value)));
	}

	window_drop_callback Windows_Window::set_drop_callback(window_drop_callback value)
	{
		return reinterpret_cast<window_drop_callback>(
			glfwSetDropCallback(m_window,
				reinterpret_cast<GLFWdropfun>(m_callbacks.on_drop = value)));
	}

	window_focus_callback Windows_Window::set_focus_callback(window_focus_callback value)
	{
		return reinterpret_cast<window_focus_callback>(
			glfwSetWindowFocusCallback(m_window,
				reinterpret_cast<GLFWwindowfocusfun>(m_callbacks.on_focus = value)));
	}

	window_framebuffer_resize_callback Windows_Window::set_framebuffer_resize_callback(window_framebuffer_resize_callback value)
	{
		return reinterpret_cast<window_framebuffer_resize_callback>(
			glfwSetFramebufferSizeCallback(m_window,
				reinterpret_cast<GLFWframebuffersizefun>(m_callbacks.on_framebuffer_resize = value)));
	}

	window_iconify_callback Windows_Window::set_iconify_callback(window_iconify_callback value)
	{
		return reinterpret_cast<window_iconify_callback>(
			glfwSetWindowIconifyCallback(m_window,
				reinterpret_cast<GLFWwindowiconifyfun>(m_callbacks.on_iconify = value)));
	}

	window_key_callback Windows_Window::set_key_callback(window_key_callback value)
	{
		return reinterpret_cast<window_key_callback>(
			glfwSetKeyCallback(m_window,
				reinterpret_cast<GLFWkeyfun>(m_callbacks.on_key = value)));
	}

	window_maximize_callback Windows_Window::set_maximize_callback(window_maximize_callback value)
	{
		return reinterpret_cast<window_maximize_callback>(
			glfwSetWindowMaximizeCallback(m_window,
				reinterpret_cast<GLFWwindowmaximizefun>(m_callbacks.on_maximize = value)));
	}

	window_mouse_button_callback Windows_Window::set_mouse_button_callback(window_mouse_button_callback value)
	{
		return reinterpret_cast<window_mouse_button_callback>(
			glfwSetMouseButtonCallback(m_window,
				reinterpret_cast<GLFWmousebuttonfun>(m_callbacks.on_mouse_button = value)));
	}

	window_mouse_enter_callback Windows_Window::set_mouse_enter_callback(window_mouse_enter_callback value)
	{
		return reinterpret_cast<window_mouse_enter_callback>(
			glfwSetCursorEnterCallback(m_window,
				reinterpret_cast<GLFWcursorenterfun>(m_callbacks.on_mouse_enter = value)));
	}

	window_mouse_pos_callback Windows_Window::set_mouse_pos_callback(window_mouse_pos_callback value)
	{
		return reinterpret_cast<window_mouse_pos_callback>(
			glfwSetCursorPosCallback(m_window,
				reinterpret_cast<GLFWcursorposfun>(m_callbacks.on_mouse_pos = value)));
	}

	window_position_callback Windows_Window::set_position_callback(window_position_callback value)
	{
		return reinterpret_cast<window_position_callback>(
			glfwSetWindowPosCallback(m_window,
				reinterpret_cast<GLFWwindowposfun>(m_callbacks.on_position = value)));
	}

	window_refresh_callback Windows_Window::set_refresh_callback(window_refresh_callback value)
	{
		return reinterpret_cast<window_refresh_callback>(
			glfwSetWindowRefreshCallback(m_window,
				reinterpret_cast<GLFWwindowrefreshfun>(m_callbacks.on_refresh = value)));
	}

	window_resize_callback Windows_Window::set_resize_callback(window_resize_callback value)
	{
		return reinterpret_cast<window_resize_callback>(
			glfwSetWindowSizeCallback(m_window,
				reinterpret_cast<GLFWwindowposfun>(m_callbacks.on_resize = value)));
	}

	window_scroll_callback Windows_Window::set_scroll_callback(window_scroll_callback value)
	{
		return reinterpret_cast<window_scroll_callback>(
			glfwSetScrollCallback(m_window,
				reinterpret_cast<GLFWscrollfun>(m_callbacks.on_mouse_wheel = value)));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}
