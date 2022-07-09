#include <platform/windows/display_server_windows.hpp>
#include <scene/main/scene_tree.hpp>

#include <glfw/glfw3.h>
#if defined(SYSTEM_WINDOWS)
#	undef APIENTRY
#	include <Windows.h>
#	define GLFW_EXPOSE_NATIVE_WIN32
#	include <glfw/glfw3native.h>
#endif

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OBJECT_EMBED(DisplayServerWindows, t) {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	DisplayServerWindows::DisplayServerWindows(String const & title, WindowMode_ mode, Vec2i const & size)
	{
		ASSERT(title);
		ASSERT(0 < size[0] && 0 < size[1]);
		ASSERT(glfwInit() == GLFW_TRUE);

#if OPENGL_ENABLED
		glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
#else
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
#endif

		switch (mode)
		{
		default: {
			CRASH("invalid window mode");
		} break;
		case WindowMode_Windowed: {
			glfwWindowHint(GLFW_AUTO_ICONIFY, true);
			glfwWindowHint(GLFW_CENTER_CURSOR, false);
			glfwWindowHint(GLFW_DECORATED, true);
			glfwWindowHint(GLFW_DOUBLEBUFFER, false);
			glfwWindowHint(GLFW_FLOATING, false);
			glfwWindowHint(GLFW_FOCUS_ON_SHOW, true);
			glfwWindowHint(GLFW_FOCUSED, true);
			glfwWindowHint(GLFW_MAXIMIZED, false);
			glfwWindowHint(GLFW_RESIZABLE, true);
			glfwWindowHint(GLFW_VISIBLE, true);
		} break;
		case WindowMode_Minimized: {
			// TODO...
		} break;
		case WindowMode_Maximized: {
			glfwWindowHint(GLFW_AUTO_ICONIFY, true);
			glfwWindowHint(GLFW_CENTER_CURSOR, false);
			glfwWindowHint(GLFW_DECORATED, true);
			glfwWindowHint(GLFW_DOUBLEBUFFER, false);
			glfwWindowHint(GLFW_FLOATING, false);
			glfwWindowHint(GLFW_FOCUS_ON_SHOW, true);
			glfwWindowHint(GLFW_FOCUSED, true);
			glfwWindowHint(GLFW_MAXIMIZED, true);
			glfwWindowHint(GLFW_RESIZABLE, true);
			glfwWindowHint(GLFW_VISIBLE, false);
		} break;
		case WindowMode_Fullscreen: {
			// TODO...
		} break;
		}

		m_window.title = title;
		m_window.window_mode = mode;
		m_window.mouse_mode = MouseMode_Normal;
		m_window.cursor_shape = CursorShape_Arrow;
		m_window.handle = VALIDATE(glfwCreateWindow(size[0], size[1], m_window.title.c_str(), nullptr, nullptr));

		glfwMakeContextCurrent(m_window.handle);
		glfwSetWindowUserPointer(m_window.handle, &m_window);

		if (mode == WindowMode_Maximized) { glfwMaximizeWindow(m_window.handle); }
	}

	DisplayServerWindows::~DisplayServerWindows()
	{
		glfwDestroyWindow(m_window.handle);
		m_window = {};
		glfwTerminate();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	DS::VideoMode const & DisplayServerWindows::get_desktop_video_mode() const
	{
		static VideoMode result{};
		static ON_SCOPE_ENTER(&)
		{
			DEVMODE dm;
			dm.dmSize = sizeof(dm);
			EnumDisplaySettings(nullptr, ENUM_CURRENT_SETTINGS, &dm);
			result.size = { (int32_t)dm.dmPelsWidth, (int32_t)dm.dmPelsHeight };
			result.bits_per_pixel = util::bit_cast<Vec4b>(dm.dmBitsPerPel);
			result.refresh_rate = -1;
		};
		return result;
	}

	List<DS::VideoMode> const & DisplayServerWindows::get_fullscreen_video_modes() const
	{
		static List<VideoMode> result{};
		static ON_SCOPE_ENTER(&)
		{
			DEVMODE dm;
			dm.dmSize = sizeof(dm);
			for (int32_t count = 0; EnumDisplaySettings(nullptr, count, &dm); ++count)
			{
				VideoMode vm;
				vm.size = { (int32_t)dm.dmPelsWidth, (int32_t)dm.dmPelsHeight };
				vm.bits_per_pixel = util::bit_cast<Vec4b>(dm.dmBitsPerPel);
				vm.refresh_rate = -1;
				if (!result.contains(vm)) { result.push_back(vm); }
			}
		};
		return result;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	DS::WindowID DisplayServerWindows::get_current_context() const
	{
		return (WindowID)glfwGetWindowUserPointer(glfwGetCurrentContext());
	}

	void DisplayServerWindows::set_current_context(WindowID window)
	{
		_Window const & w{ *VALIDATE((_Window const *)window) };
		glfwMakeContextCurrent(w.handle);
	}

	bool DisplayServerWindows::window_should_close(WindowID window) const
	{
		_Window const & w{ *VALIDATE((_Window const *)window) };
		return glfwWindowShouldClose(w.handle);
	}

	void DisplayServerWindows::window_set_should_close(WindowID window, bool value)
	{
		_Window const & w{ *VALIDATE((_Window const *)window) };
		glfwSetWindowShouldClose(w.handle, value);
	}

	String DisplayServerWindows::get_clipboard() const
	{
		return glfwGetClipboardString(m_window.handle);
	}

	void DisplayServerWindows::set_clipboard(String const & value)
	{
		glfwSetClipboardString(m_window.handle, value.c_str());
	}

	DS::MouseMode_ DisplayServerWindows::mouse_get_mode() const
	{
		return m_window.mouse_mode;
	}

	void DisplayServerWindows::mouse_set_mode(MouseMode_ value)
	{
		if (m_window.mouse_mode == value) { return; }
		m_window.mouse_mode = value;
		int32_t temp;
		switch (value) {
		default: return;
		case MouseMode_Normal: { temp = GLFW_CURSOR_NORMAL; } break;
		case MouseMode_Hidden: { temp = GLFW_CURSOR_HIDDEN; } break;
		case MouseMode_Disabled: { temp = GLFW_CURSOR_DISABLED; } break;
		}
		glfwSetInputMode(m_window.handle, GLFW_CURSOR, temp);
	}

	int32_t DisplayServerWindows::mouse_get_button(MouseButton_ value) const
	{
		return glfwGetMouseButton(m_window.handle, value);
	}

	Vec2 DisplayServerWindows::mouse_get_position() const
	{
		Vec2d v;
		glfwGetCursorPos(m_window.handle, &v[0], &v[1]);
		return { (float_t)v[0], (float_t)v[1] };
	}

	void DisplayServerWindows::mouse_set_position(Vec2 const & value)
	{
		glfwSetCursorPos(m_window.handle, value[0], value[1]);
	}

	String DisplayServerWindows::window_get_title(WindowID window) const
	{
		_Window const & w{ *VALIDATE((_Window const *)window) };
		return w.title;
	}

	void DisplayServerWindows::window_set_title(WindowID window, String const & value)
	{
		if (!value) { return; }
		_Window & w{ *VALIDATE((_Window *)window) };
		w.title = value;
		glfwSetWindowTitle(w.handle, w.title.c_str());
	}

	DS::MonitorID DisplayServerWindows::window_get_monitor(WindowID window) const
	{
		_Window const & w{ *VALIDATE((_Window const *)window) };
		return (MonitorID)glfwGetWindowMonitor(w.handle);
	}

	void DisplayServerWindows::window_set_monitor(WindowID window, MonitorID monitor)
	{
	}

	Vec2i DisplayServerWindows::window_get_position(WindowID window) const
	{
		_Window const & w{ *VALIDATE((_Window const *)window) };
		Vec2i v;
		glfwGetWindowPos(w.handle, &v[0], &v[1]);
		return v;
	}

	void DisplayServerWindows::window_set_position(WindowID window, Vec2i const & value)
	{
		_Window & w{ *VALIDATE((_Window *)window) };
		glfwSetWindowPos(w.handle, (int32_t)value[0], (int32_t)value[1]);
	}

	Vec2i DisplayServerWindows::window_get_size(WindowID window) const
	{
		_Window const & w{ *VALIDATE((_Window const *)window) };
		Vec2i v;
		glfwGetWindowSize(w.handle, &v[0], &v[1]);
		return v;
	}

	void DisplayServerWindows::window_set_size(WindowID window, Vec2i const & value)
	{
		_Window & w{ *VALIDATE((_Window *)window) };
		glfwSetWindowSize(w.handle, (int32_t)value[0], (int32_t)value[1]);
	}

	Vec2i DisplayServerWindows::window_get_real_size(WindowID window) const
	{
		_Window const & w{ *VALIDATE((_Window const *)window) };
		Vec2i v;
		glfwGetFramebufferSize(w.handle, &v[0], &v[1]);
		return v;
	}

	DS::WindowMode_ DisplayServerWindows::window_get_mode(WindowID window) const
	{
		_Window const & w{ *VALIDATE((_Window const *)window) };
		return w.window_mode;
	}

	void DisplayServerWindows::window_set_mode(WindowID window, WindowMode_ value)
	{
		_Window & w{ *VALIDATE((_Window *)window) };
		if (w.window_mode == value) { return; }
		w.window_mode = value;
		switch (value) {
		case WindowMode_Windowed: {
			glfwRestoreWindow(w.handle);
		} break;
		case WindowMode_Minimized: {
			glfwIconifyWindow(w.handle);
		} break;
		case WindowMode_Maximized: {
			glfwMaximizeWindow(w.handle);
		} break;
		case WindowMode_Fullscreen: {
			// TODO...
		} break;
		}
	}

	bool DisplayServerWindows::window_get_flag(WindowID window, int32_t flag) const
	{
		return false;
	}

	void DisplayServerWindows::window_set_flag(WindowID window, int32_t flag, bool enabled)
	{
	}

	void DisplayServerWindows::request_window_attention(WindowID window)
	{
		_Window const & w{ *VALIDATE((_Window const *)window) };
		glfwRequestWindowAttention(w.handle);
	}

	void DisplayServerWindows::move_window_to_foreground(WindowID window)
	{
	}

	void DisplayServerWindows::window_set_visible(WindowID window, bool value)
	{
		_Window & w{ *VALIDATE((_Window *)window) };
		(value ? glfwShowWindow : glfwHideWindow)(w.handle);
	}

	bool DisplayServerWindows::window_is_visible(WindowID window) const
	{
		_Window & w{ *VALIDATE((_Window *)window) };
		return glfwGetWindowAttrib(w.handle, GLFW_VISIBLE);
	}

	Vec2 DisplayServerWindows::window_get_content_scale(WindowID window) const
	{
		_Window const & w{ *VALIDATE((_Window const *)window) };
		Vec2 v;
		glfwGetWindowContentScale(w.handle, &v[0], &v[1]);
		return v;
	}

	void DisplayServerWindows::window_grab_focus(WindowID window)
	{
		_Window & w{ *VALIDATE((_Window *)window) };
		glfwFocusWindow(w.handle);
	}

	bool DisplayServerWindows::window_has_focus(WindowID window) const
	{
		_Window const & w{ *VALIDATE((_Window const *)window) };
		return glfwGetWindowAttrib(w.handle, GLFW_FOCUSED);
	}

	DS::CursorShape_ DisplayServerWindows::cursor_get_shape() const
	{
		return m_window.cursor_shape;
	}

	void DisplayServerWindows::cursor_set_shape(CursorShape_ value)
	{
		if (m_window.cursor_shape == value) { return; }
		m_window.cursor_shape = value;
	}

	void DisplayServerWindows::cursor_set_custom_image(RES const & cursor, CursorShape_ shape, Vec2 const & hotspot)
	{
	}

	void DisplayServerWindows::poll_events()
	{
		glfwPollEvents();
	}

	void DisplayServerWindows::swap_buffers()
	{
		glfwSwapBuffers(m_window.handle);
	}

	void DisplayServerWindows::set_native_icon(Path const & value)
	{
	}

	void DisplayServerWindows::set_icon(Ref<Image> const & value)
	{
		ASSERT(value && 0 < value->get_capacity());
		GLFWimage icon;
		icon.pixels = (uint8_t *)value->get_data().data();
		icon.width = value->get_width();
		icon.height = value->get_height();
		glfwSetWindowIcon(m_window.handle, 1, &icon);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	DS::ErrorCallback DisplayServerWindows::set_error_callback(ErrorCallback callback)
	{
		return reinterpret_cast<ErrorCallback>(glfwSetErrorCallback(reinterpret_cast<GLFWerrorfun>(callback)));
	}

	DS::CharCallback DisplayServerWindows::window_set_char_callback(WindowID window, CharCallback value)
	{
		_Window & w{ *VALIDATE((_Window *)window) };
		return reinterpret_cast<CharCallback>(glfwSetCharCallback(w.handle, reinterpret_cast<GLFWcharfun>(value)));
	}

	DS::CharModsCallback DisplayServerWindows::window_set_char_mods_callback(WindowID window, CharModsCallback value)
	{
		_Window & w{ *VALIDATE((_Window *)window) };
		return reinterpret_cast<CharModsCallback>(glfwSetCharModsCallback(w.handle, reinterpret_cast<GLFWcharmodsfun>(value)));
	}

	DS::CloseCallback DisplayServerWindows::window_set_close_callback(WindowID window, CloseCallback value)
	{
		_Window & w{ *VALIDATE((_Window *)window) };
		return reinterpret_cast<CloseCallback>(glfwSetWindowCloseCallback(w.handle, reinterpret_cast<GLFWwindowclosefun>(value)));
	}

	DS::ContentScaleCallback DisplayServerWindows::window_set_content_scale_callback(WindowID window, ContentScaleCallback value)
	{
		_Window & w{ *VALIDATE((_Window *)window) };
		return reinterpret_cast<ContentScaleCallback>(glfwSetWindowContentScaleCallback(w.handle, reinterpret_cast<GLFWwindowcontentscalefun>(value)));
	}

	DS::DropCallback DisplayServerWindows::window_set_drop_callback(WindowID window, DropCallback value)
	{
		_Window & w{ *VALIDATE((_Window *)window) };
		return reinterpret_cast<DropCallback>(glfwSetDropCallback(w.handle, reinterpret_cast<GLFWdropfun>(value)));
	}

	DS::FocusCallback DisplayServerWindows::window_set_focus_callback(WindowID window, FocusCallback value)
	{
		_Window & w{ *VALIDATE((_Window *)window) };
		return reinterpret_cast<FocusCallback>(glfwSetWindowFocusCallback(w.handle, reinterpret_cast<GLFWwindowfocusfun>(value)));
	}

	DS::FramebufferResizeCallback DisplayServerWindows::window_set_framebuffer_resize_callback(WindowID window, FramebufferResizeCallback value)
	{
		_Window & w{ *VALIDATE((_Window *)window) };
		return reinterpret_cast<FramebufferResizeCallback>(glfwSetFramebufferSizeCallback(w.handle, reinterpret_cast<GLFWframebuffersizefun>(value)));
	}

	DS::IconifyCallback DisplayServerWindows::window_set_iconify_callback(WindowID window, IconifyCallback value)
	{
		_Window & w{ *VALIDATE((_Window *)window) };
		return reinterpret_cast<IconifyCallback>(glfwSetWindowIconifyCallback(w.handle, reinterpret_cast<GLFWwindowiconifyfun>(value)));
	}

	DS::KeyCallback DisplayServerWindows::window_set_key_callback(WindowID window, KeyCallback value)
	{
		_Window & w{ *VALIDATE((_Window *)window) };
		return reinterpret_cast<KeyCallback>(glfwSetKeyCallback(w.handle, reinterpret_cast<GLFWkeyfun>(value)));
	}

	DS::MaximizeCallback DisplayServerWindows::window_set_maximize_callback(WindowID window, MaximizeCallback value)
	{
		_Window & w{ *VALIDATE((_Window *)window) };
		return reinterpret_cast<MaximizeCallback>(glfwSetWindowMaximizeCallback(w.handle, reinterpret_cast<GLFWwindowmaximizefun>(value)));
	}

	DS::MouseButtonCallback DisplayServerWindows::window_set_mouse_button_callback(WindowID window, MouseButtonCallback value)
	{
		_Window & w{ *VALIDATE((_Window *)window) };
		return reinterpret_cast<MouseButtonCallback>(glfwSetMouseButtonCallback(w.handle, reinterpret_cast<GLFWmousebuttonfun>(value)));
	}

	DS::MouseEnterCallback DisplayServerWindows::window_set_mouse_enter_callback(WindowID window, MouseEnterCallback value)
	{
		_Window & w{ *VALIDATE((_Window *)window) };
		return reinterpret_cast<MouseEnterCallback>(glfwSetCursorEnterCallback(w.handle, reinterpret_cast<GLFWcursorenterfun>(value)));
	}

	DS::MousePositionCallback DisplayServerWindows::window_set_mouse_position_callback(WindowID window, MousePositionCallback value)
	{
		_Window & w{ *VALIDATE((_Window *)window) };
		return reinterpret_cast<MousePositionCallback>(glfwSetCursorPosCallback(w.handle, reinterpret_cast<GLFWcursorposfun>(value)));
	}

	DS::PositionCallback DisplayServerWindows::window_set_position_callback(WindowID window, PositionCallback value)
	{
		_Window & w{ *VALIDATE((_Window *)window) };
		return reinterpret_cast<PositionCallback>(glfwSetWindowPosCallback(w.handle, reinterpret_cast<GLFWwindowposfun>(value)));
	}

	DS::RefreshCallback DisplayServerWindows::window_set_refresh_callback(WindowID window, RefreshCallback value)
	{
		_Window & w{ *VALIDATE((_Window *)window) };
		return reinterpret_cast<RefreshCallback>(glfwSetWindowRefreshCallback(w.handle, reinterpret_cast<GLFWwindowrefreshfun>(value)));
	}

	DS::ScrollCallback DisplayServerWindows::window_set_scroll_callback(WindowID window, ScrollCallback value)
	{
		_Window & w{ *VALIDATE((_Window *)window) };
		return reinterpret_cast<ScrollCallback>(glfwSetScrollCallback(w.handle, reinterpret_cast<GLFWscrollfun>(value)));
	}

	DS::SizeCallback DisplayServerWindows::window_set_size_callback(WindowID window, SizeCallback value)
	{
		_Window & w{ *VALIDATE((_Window *)window) };
		return reinterpret_cast<SizeCallback>(glfwSetWindowSizeCallback(w.handle, reinterpret_cast<GLFWwindowposfun>(value)));
	}

	DS::MonitorCallback DisplayServerWindows::set_monitor_callback(MonitorCallback callback)
	{
		return reinterpret_cast<MonitorCallback>(glfwSetMonitorCallback(reinterpret_cast<GLFWmonitorfun>(callback)));
	}

	DS::JoystickCallback DisplayServerWindows::set_joystick_callback(JoystickCallback callback)
	{
		return reinterpret_cast<JoystickCallback>(glfwSetJoystickCallback(reinterpret_cast<GLFWjoystickfun>(callback)));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

}