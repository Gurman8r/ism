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

	DisplayServerWindows::DisplayServerWindows(String const & title, WindowMode_ mode, VideoMode const & video)
	{
		ASSERT(title);
		ASSERT(0 < video.size[0]);
		ASSERT(0 < video.size[1]);
		ASSERT((unsigned)mode < (unsigned)WindowMode_MAX);
		ASSERT(glfwInit() == GLFW_TRUE);

#if OPENGL_ENABLED
		glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
		glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_NATIVE_CONTEXT_API);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
#else
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
#endif

		glfwWindowHint(GLFW_RESIZABLE, true);
		glfwWindowHint(GLFW_VISIBLE, mode != WindowMode_Maximized);
		glfwWindowHint(GLFW_DECORATED, true);
		glfwWindowHint(GLFW_FOCUSED, true);
		glfwWindowHint(GLFW_AUTO_ICONIFY, true);
		glfwWindowHint(GLFW_CENTER_CURSOR, true);
		glfwWindowHint(GLFW_FOCUS_ON_SHOW, true);
		glfwWindowHint(GLFW_FLOATING, false);
		glfwWindowHint(GLFW_MAXIMIZED, mode == WindowMode_Maximized);

		glfwWindowHint(GLFW_RED_BITS, COLOR32_R(video.bits_per_pixel));
		glfwWindowHint(GLFW_GREEN_BITS, COLOR32_G(video.bits_per_pixel));
		glfwWindowHint(GLFW_BLUE_BITS, COLOR32_B(video.bits_per_pixel));
		glfwWindowHint(GLFW_ALPHA_BITS, COLOR32_A(video.bits_per_pixel));
		glfwWindowHint(GLFW_DEPTH_BITS, 24);
		glfwWindowHint(GLFW_STENCIL_BITS, 8);
		glfwWindowHint(GLFW_DOUBLEBUFFER, false);
		glfwWindowHint(GLFW_REFRESH_RATE, video.refresh_rate);
		
		_Window & w{ m_windows[MAIN_WINDOW_ID] = {} };
		w.title = title;
		w.current_screen = 0;
		w.window_mode = mode;
		w.handle = VALIDATE(glfwCreateWindow(video.size[0], video.size[1], w.title.c_str(), 0, 0));
		glfwSetWindowUserPointer(w.handle, &w);

		if (mode == WindowMode_Maximized) { glfwMaximizeWindow(w.handle); }

		glfwMakeContextCurrent(w.handle);

		m_main_window = &m_windows[MAIN_WINDOW_ID];
	}

	DisplayServerWindows::~DisplayServerWindows()
	{
		for (auto const & [id, w] : m_windows)
		{
			glfwDestroyWindow(w.handle);
		}

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
			result.bits_per_pixel = dm.dmBitsPerPel;
			result.refresh_rate = dm.dmDisplayFrequency;
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
				vm.bits_per_pixel = dm.dmBitsPerPel;
				vm.refresh_rate = dm.dmDisplayFrequency;

				if (auto const it{ std::equal_range(result.begin(), result.end(), vm, [
				](VideoMode const & a, VideoMode const & b) {
					return (a.size < b.size || a.bits_per_pixel < b.bits_per_pixel || a.refresh_rate < b.refresh_rate);
				}) }
				; it.first == it.second) {
					result.emplace(it.second, std::move(vm));
				}
			}
		};
		return result;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// TODO: global menu stuff goes here

	void DisplayServerWindows::alert(String const & message, String const & title)
	{
	}

	String DisplayServerWindows::get_clipboard() const
	{
		return glfwGetClipboardString(VALIDATE(m_main_window)->handle);
	}

	void DisplayServerWindows::set_clipboard(String const & text)
	{
		glfwSetClipboardString(VALIDATE(m_main_window)->handle, text.c_str());
	}

	DS::CursorShape_ DisplayServerWindows::cursor_get_shape() const
	{
		return {};
	}

	void DisplayServerWindows::cursor_set_shape(CursorShape_ shape)
	{
	}

	void DisplayServerWindows::cursor_set_custom_image(RES const & cursor, CursorShape_ shape, Vec2 const & hotspot)
	{
	}

	DS::MouseMode_ DisplayServerWindows::mouse_get_mode() const
	{
		return {};
	}

	void DisplayServerWindows::mouse_set_mode(MouseMode_ mode)
	{
		glfwSetInputMode(VALIDATE(m_main_window)->handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	int32_t DisplayServerWindows::mouse_get_button(MouseButton_ button) const
	{
		return glfwGetMouseButton(VALIDATE(m_main_window)->handle, (int32_t)button);
	}

	Vec2 DisplayServerWindows::mouse_get_position() const
	{
		Vec2d v;
		glfwGetCursorPos(VALIDATE(m_main_window)->handle, &v[0], &v[1]);
		return { (float_t)v[0], (float_t)v[1] };
	}

	void DisplayServerWindows::mouse_set_position(Vec2 const & position)
	{
		glfwSetCursorPos(VALIDATE(m_main_window)->handle, position[0], position[1]);
	}

	int32_t DisplayServerWindows::get_screen_count() const
	{
		int32_t monitor_count;
		UNUSED(glfwGetMonitors(&monitor_count));
		return monitor_count;
	}

	String DisplayServerWindows::screen_get_name(int32_t screen) const
	{
		int32_t monitor_count;
		GLFWmonitor ** monitors{ glfwGetMonitors(&monitor_count) };
		screen = (screen == SCREEN_OF_MAIN_WINDOW) ? window_get_current_screen() : screen;
		ASSERT((unsigned)screen < (unsigned)monitor_count);

		return glfwGetMonitorName(monitors[screen]);
	}

	IntRect DisplayServerWindows::screen_get_rect(int32_t screen) const
	{
		int32_t monitor_count;
		GLFWmonitor ** monitors{ glfwGetMonitors(&monitor_count) };
		screen = (screen == SCREEN_OF_MAIN_WINDOW) ? window_get_current_screen() : screen;
		ASSERT((unsigned)screen < (unsigned)monitor_count);

		IntRect rect;
		glfwGetMonitorWorkarea(monitors[screen], &rect[0], &rect[1], &rect[2], &rect[3]);
		return rect;
	}

	Vec2i DisplayServerWindows::screen_get_position(int32_t screen) const
	{
		int32_t monitor_count;
		GLFWmonitor ** monitors{ glfwGetMonitors(&monitor_count) };
		screen = (screen == SCREEN_OF_MAIN_WINDOW) ? window_get_current_screen() : screen;
		ASSERT((unsigned)screen < (unsigned)monitor_count);

		Vec2i pos;
		glfwGetMonitorWorkarea(monitors[screen], &pos[0], &pos[1], nullptr, nullptr);
		return pos;
	}

	Vec2i DisplayServerWindows::screen_get_size(int32_t screen) const
	{
		int32_t monitor_count;
		GLFWmonitor ** monitors{ glfwGetMonitors(&monitor_count) };
		screen = (screen == SCREEN_OF_MAIN_WINDOW) ? window_get_current_screen() : screen;
		ASSERT((unsigned)screen < (unsigned)monitor_count);

		Vec2i size;
		glfwGetMonitorWorkarea(monitors[screen], nullptr, nullptr, &size[0], &size[1]);
		return size;
	}

	Vec2 DisplayServerWindows::screen_get_scale(int32_t screen) const
	{
		int32_t monitor_count;
		GLFWmonitor ** monitors{ glfwGetMonitors(&monitor_count) };
		screen = (screen == SCREEN_OF_MAIN_WINDOW) ? window_get_current_screen() : screen;
		ASSERT((unsigned)screen < (unsigned)monitor_count);

		Vec2 scale;
		glfwGetMonitorContentScale(monitors[screen], &scale[0], &scale[1]);
		return scale;
	}

	List<DS::WindowID> DisplayServerWindows::get_window_list() const
	{
		return {};
	}

	String DisplayServerWindows::window_get_title(WindowID window) const
	{
		_Window const & w{ *VALIDATE(util::getptr(m_windows, window)) };
		return w.title;
	}

	void DisplayServerWindows::window_set_title(String const & title, WindowID window)
	{
		_Window & w{ *VALIDATE(util::getptr(m_windows, window)) };
		if (!title) { return; }
		w.title = title;
		glfwSetWindowTitle(w.handle, w.title.c_str());
	}

	int32_t DisplayServerWindows::window_get_current_screen(WindowID window) const
	{
		_Window const & w{ *VALIDATE(util::getptr(m_windows, window)) };

		return w.current_screen;
	}

	void DisplayServerWindows::window_set_current_screen(int32_t screen, WindowID window)
	{
		_Window & w{ *VALIDATE(util::getptr(m_windows, window)) };
		if (w.current_screen == screen) { return; }
		w.current_screen = screen;
	}

	Vec2i DisplayServerWindows::window_get_position(WindowID window) const
	{
		_Window const & w{ *VALIDATE(util::getptr(m_windows, window)) };

		Vec2i pos;
		glfwGetWindowPos(w.handle, &pos[0], &pos[1]);
		return pos;
	}

	void DisplayServerWindows::window_set_position(Vec2i const & position, WindowID window)
	{
		_Window & w{ *VALIDATE(util::getptr(m_windows, window)) };

		glfwSetWindowPos(w.handle, (int32_t)position[0], (int32_t)position[1]);
	}

	Vec2i DisplayServerWindows::window_get_size(WindowID window) const
	{
		_Window const & w{ *VALIDATE(util::getptr(m_windows, window)) };

		Vec2i size;
		glfwGetWindowSize(w.handle, &size[0], &size[1]);
		return size;
	}

	void DisplayServerWindows::window_set_size(Vec2i const & size, WindowID window)
	{
		_Window & w{ *VALIDATE(util::getptr(m_windows, window)) };

		glfwSetWindowSize(w.handle, (int32_t)size[0], (int32_t)size[1]);
	}

	Vec2i DisplayServerWindows::window_get_real_size(WindowID window) const
	{
		_Window const & w{ *VALIDATE(util::getptr(m_windows, window)) };

		Vec2i size;
		glfwGetFramebufferSize(w.handle, &size[0], &size[1]);
		return size;
	}

	DS::WindowMode_ DisplayServerWindows::window_get_mode(WindowID window) const
	{
		_Window const & w{ *VALIDATE(util::getptr(m_windows, window)) };

		return w.window_mode;
	}

	void DisplayServerWindows::window_set_mode(WindowMode_ mode, WindowID window)
	{
		_Window & w{ *VALIDATE(util::getptr(m_windows, window)) };
		if (w.window_mode == mode) { return; }
		w.window_mode = mode;
		switch (mode) {
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

	bool DisplayServerWindows::window_get_flag(int32_t flag, WindowID window) const
	{
		return false;
	}

	void DisplayServerWindows::window_set_flag(int32_t flag, bool enabled, WindowID window)
	{
	}

	void DisplayServerWindows::request_window_attention(WindowID window)
	{
		_Window const & w{ *VALIDATE(util::getptr(m_windows, window)) };

		glfwRequestWindowAttention(w.handle);
	}

	void DisplayServerWindows::move_window_to_foreground(WindowID window)
	{
		_Window const & w{ *VALIDATE(util::getptr(m_windows, window)) };
	}

	void DisplayServerWindows::window_set_visible(bool enabled, WindowID window)
	{
		_Window & w{ *VALIDATE(util::getptr(m_windows, window)) };

		(enabled ? glfwShowWindow : glfwHideWindow)(w.handle);
	}

	bool DisplayServerWindows::window_is_visible(WindowID window) const
	{
		_Window const & w{ *VALIDATE(util::getptr(m_windows, window)) };

		return glfwGetWindowAttrib(w.handle, GLFW_VISIBLE);
	}

	Vec2 DisplayServerWindows::window_get_scale(WindowID window) const
	{
		_Window const & w{ *VALIDATE(util::getptr(m_windows, window)) };

		Vec2 scale;
		glfwGetWindowContentScale(w.handle, &scale[0], &scale[1]);
		return scale;
	}

	void DisplayServerWindows::window_grab_focus(WindowID window)
	{
		_Window & w{ *VALIDATE(util::getptr(m_windows, window)) };

		glfwFocusWindow(w.handle);
	}

	bool DisplayServerWindows::window_has_focus(WindowID window) const
	{
		_Window const & w{ *VALIDATE(util::getptr(m_windows, window)) };

		return glfwGetWindowAttrib(w.handle, GLFW_FOCUSED);
	}

	void * DisplayServerWindows::window_get_native_handle(WindowID window) const
	{
		_Window const & w{ *VALIDATE(util::getptr(m_windows, window)) };

		return glfwGetWin32Window(w.handle);
	}

	DS::WindowID DisplayServerWindows::get_current_context() const
	{
		return (WindowID)glfwGetCurrentContext();
	}

	void DisplayServerWindows::make_context_current(WindowID window)
	{
		glfwMakeContextCurrent((GLFWwindow *)window);
	}

	void DisplayServerWindows::poll_events()
	{
		glfwPollEvents();
	}

	void DisplayServerWindows::swap_buffers()
	{
		glfwSwapBuffers(VALIDATE(m_main_window)->handle);
	}

	void DisplayServerWindows::set_native_icon(Path const & value)
	{
	}

	void DisplayServerWindows::set_icon(uint8_t const * data, int32_t width, int32_t height)
	{
		ASSERT(data);
		ASSERT(0 < width);
		ASSERT(0 < height);
		GLFWimage icon;
		icon.pixels = (uint8_t *)data;
		icon.width = width;
		icon.height = height;
		glfwSetWindowIcon(VALIDATE(m_main_window)->handle, 1, &icon);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	DS::CharCallback DisplayServerWindows::window_set_char_callback(CharCallback callback, WindowID window)
	{
		_Window & w{ *VALIDATE(util::getptr(m_windows, window)) };

		return reinterpret_cast<CharCallback>(glfwSetCharCallback(w.handle, reinterpret_cast<GLFWcharfun>(callback)));
	}

	DS::CharModsCallback DisplayServerWindows::window_set_char_mods_callback(CharModsCallback callback, WindowID window)
	{
		_Window & w{ *VALIDATE(util::getptr(m_windows, window)) };

		return reinterpret_cast<CharModsCallback>(glfwSetCharModsCallback(w.handle, reinterpret_cast<GLFWcharmodsfun>(callback)));
	}

	DS::CloseCallback DisplayServerWindows::window_set_close_callback(CloseCallback callback, WindowID window)
	{
		_Window & w{ *VALIDATE(util::getptr(m_windows, window)) };

		return reinterpret_cast<CloseCallback>(glfwSetWindowCloseCallback(w.handle, reinterpret_cast<GLFWwindowclosefun>(callback)));
	}

	DS::DropCallback DisplayServerWindows::window_set_drop_callback(DropCallback callback, WindowID window)
	{
		_Window & w{ *VALIDATE(util::getptr(m_windows, window)) };

		return reinterpret_cast<DropCallback>(glfwSetDropCallback(w.handle, reinterpret_cast<GLFWdropfun>(callback)));
	}

	DS::FocusCallback DisplayServerWindows::window_set_focus_callback(FocusCallback callback, WindowID window)
	{
		_Window & w{ *VALIDATE(util::getptr(m_windows, window)) };

		return reinterpret_cast<FocusCallback>(glfwSetWindowFocusCallback(w.handle, reinterpret_cast<GLFWwindowfocusfun>(callback)));
	}

	DS::FramebufferResizeCallback DisplayServerWindows::window_set_framebuffer_resize_callback(FramebufferResizeCallback callback, WindowID window)
	{
		_Window & w{ *VALIDATE(util::getptr(m_windows, window)) };

		return reinterpret_cast<FramebufferResizeCallback>(glfwSetFramebufferSizeCallback(w.handle, reinterpret_cast<GLFWframebuffersizefun>(callback)));
	}

	DS::IconifyCallback DisplayServerWindows::window_set_iconify_callback(IconifyCallback callback, WindowID window)
	{
		_Window & w{ *VALIDATE(util::getptr(m_windows, window)) };

		return reinterpret_cast<IconifyCallback>(glfwSetWindowIconifyCallback(w.handle, reinterpret_cast<GLFWwindowiconifyfun>(callback)));
	}

	DS::KeyCallback DisplayServerWindows::window_set_key_callback(KeyCallback callback, WindowID window)
	{
		_Window & w{ *VALIDATE(util::getptr(m_windows, window)) };

		return reinterpret_cast<KeyCallback>(glfwSetKeyCallback(w.handle, reinterpret_cast<GLFWkeyfun>(callback)));
	}

	DS::MaximizeCallback DisplayServerWindows::window_set_maximize_callback(MaximizeCallback callback, WindowID window)
	{
		_Window & w{ *VALIDATE(util::getptr(m_windows, window)) };

		return reinterpret_cast<MaximizeCallback>(glfwSetWindowMaximizeCallback(w.handle, reinterpret_cast<GLFWwindowmaximizefun>(callback)));
	}

	DS::MouseButtonCallback DisplayServerWindows::window_set_mouse_button_callback(MouseButtonCallback callback, WindowID window)
	{
		_Window & w{ *VALIDATE(util::getptr(m_windows, window)) };

		return reinterpret_cast<MouseButtonCallback>(glfwSetMouseButtonCallback(w.handle, reinterpret_cast<GLFWmousebuttonfun>(callback)));
	}

	DS::MouseEnterCallback DisplayServerWindows::window_set_mouse_enter_callback(MouseEnterCallback callback, WindowID window)
	{
		_Window & w{ *VALIDATE(util::getptr(m_windows, window)) };

		return reinterpret_cast<MouseEnterCallback>(glfwSetCursorEnterCallback(w.handle, reinterpret_cast<GLFWcursorenterfun>(callback)));
	}

	DS::MousePositionCallback DisplayServerWindows::window_set_mouse_position_callback(MousePositionCallback callback, WindowID window)
	{
		_Window & w{ *VALIDATE(util::getptr(m_windows, window)) };

		return reinterpret_cast<MousePositionCallback>(glfwSetCursorPosCallback(w.handle, reinterpret_cast<GLFWcursorposfun>(callback)));
	}

	DS::PositionCallback DisplayServerWindows::window_set_position_callback(PositionCallback callback, WindowID window)
	{
		_Window & w{ *VALIDATE(util::getptr(m_windows, window)) };

		return reinterpret_cast<PositionCallback>(glfwSetWindowPosCallback(w.handle, reinterpret_cast<GLFWwindowposfun>(callback)));
	}

	DS::RefreshCallback DisplayServerWindows::window_set_refresh_callback(RefreshCallback callback, WindowID window)
	{
		_Window & w{ *VALIDATE(util::getptr(m_windows, window)) };

		return reinterpret_cast<RefreshCallback>(glfwSetWindowRefreshCallback(w.handle, reinterpret_cast<GLFWwindowrefreshfun>(callback)));
	}

	DS::ScaleCallback DisplayServerWindows::window_set_scale_callback(ScaleCallback callback, WindowID window)
	{
		_Window & w{ *VALIDATE(util::getptr(m_windows, window)) };

		return reinterpret_cast<ScaleCallback>(glfwSetWindowContentScaleCallback(w.handle, reinterpret_cast<GLFWwindowcontentscalefun>(callback)));
	}

	DS::ScrollCallback DisplayServerWindows::window_set_scroll_callback(ScrollCallback callback, WindowID window)
	{
		_Window & w{ *VALIDATE(util::getptr(m_windows, window)) };

		return reinterpret_cast<ScrollCallback>(glfwSetScrollCallback(w.handle, reinterpret_cast<GLFWscrollfun>(callback)));
	}

	DS::SizeCallback DisplayServerWindows::window_set_size_callback(SizeCallback callback, WindowID window)
	{
		_Window & w{ *VALIDATE(util::getptr(m_windows, window)) };

		return reinterpret_cast<SizeCallback>(glfwSetWindowSizeCallback(w.handle, reinterpret_cast<GLFWwindowposfun>(callback)));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

}