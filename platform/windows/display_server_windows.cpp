#include <platform/windows/display_server_windows.hpp>
#include <scene/main/window.hpp>

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
		ASSERT(glfwInit() == GLFW_TRUE);

		glfwSetMonitorCallback([](GLFWmonitor * monitor, i32 connected)
		{
			// monitor connected/disconnected
		});

		glfwSetJoystickCallback([](i32 device, i32 connected)
		{
			// joystick connected/disconnected
			Input::get_singleton()->joy_connection_changed(device, (connected == GLFW_CONNECTED), glfwGetJoystickGUID(device));
		});

		ASSERT(title);
		ASSERT(0 < size[0]);
		ASSERT(0 < size[1]);
		ASSERT((unsigned)mode < (unsigned)WindowMode_MAX);

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

		glfwWindowHint(GLFW_RED_BITS, 8);
		glfwWindowHint(GLFW_GREEN_BITS, 8);
		glfwWindowHint(GLFW_BLUE_BITS, 8);
		glfwWindowHint(GLFW_ALPHA_BITS, 8);
		glfwWindowHint(GLFW_DEPTH_BITS, 24);
		glfwWindowHint(GLFW_STENCIL_BITS, 8);
		glfwWindowHint(GLFW_DOUBLEBUFFER, false);
		glfwWindowHint(GLFW_REFRESH_RATE, GLFW_DONT_CARE);

		_Window & w{ m_windows[MAIN_WINDOW_ID] = {} };
		w.title = title;
		w.window_mode = mode;
		w.handle = VALIDATE(glfwCreateWindow(size[0], size[1], w.title.c_str(), nullptr, nullptr));
		glfwSetWindowUserPointer(w.handle, &w);

		glfwSetCharCallback(w.handle, [](auto, auto c)
		{
		});
		glfwSetKeyCallback(w.handle, [](auto, auto key, auto, auto action, auto)
		{
			Input::get_singleton()->set_key(key, action);
		});
		glfwSetMouseButtonCallback(w.handle, [](auto, auto button, auto action, auto)
		{
			Input::get_singleton()->set_mouse_button(button, action);
		});
		glfwSetCursorPosCallback(w.handle, [](auto, auto x, auto y)
		{
			Input::get_singleton()->set_mouse_position({ (f32)x, (f32)y });
		});
		glfwSetScrollCallback(w.handle, [](auto, auto x, auto y)
		{
			Input::get_singleton()->set_mouse_wheel({ (f32)x, (f32)y });
		});
		glfwSetCursorEnterCallback(w.handle, [](auto, auto entered)
		{
			if (auto const tree{ SceneTree::get_singleton() }) {
				tree->get_root()->propagate_notification(entered ? Node::Notification_WM_MouseEnter : Node::Notification_WM_MouseExit);
			}
		});
		glfwSetWindowCloseCallback(w.handle, [](auto)
		{
			if (auto const tree{ SceneTree::get_singleton() }) {
				tree->get_root()->propagate_notification(Node::Notification_WM_CloseRequest);
				tree->quit();
			}
		});
		glfwSetWindowFocusCallback(w.handle, [](auto, auto focused)
		{
			if (auto const tree{ SceneTree::get_singleton() }) {
				tree->get_root()->propagate_notification(focused ? Node::Notification_WM_FocusIn : Node::Notification_WM_FocusOut);
			}
		});
		glfwSetWindowContentScaleCallback(w.handle, [](auto, auto x, auto y) 
		{
			if (auto const tree{ SceneTree::get_singleton() }) {
				tree->get_root()->propagate_notification(Node::Notification_WM_ScaleChanged);
			}
		});
		glfwSetWindowSizeCallback(w.handle, [](auto, auto w, auto h)
		{
			if (auto const tree{ SceneTree::get_singleton() }) {
				tree->get_root()->propagate_notification(Node::Notification_WM_SizeChanged);
			}
		});

		switch (mode) {
		case WindowMode_Windowed: {} break;
		case WindowMode_Minimized: { glfwIconifyWindow(w.handle); } break;
		case WindowMode_Maximized: { glfwMaximizeWindow(w.handle); } break;
		case WindowMode_Fullscreen: {} break;
		}

		glfwMakeContextCurrent(w.handle);
		m_main_window = &w;
	}

	DisplayServerWindows::~DisplayServerWindows()
	{
		for (auto const & [k, v] : m_windows)
		{
			glfwDestroyWindow(v.handle);
		}

		glfwTerminate();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void DisplayServerWindows::get_desktop_video_mode(VideoMode & out) const
	{
		DEVMODE dm; dm.dmSize = sizeof(dm);
		EnumDisplaySettings(nullptr, ENUM_CURRENT_SETTINGS, &dm);
		out.size = { (i32)dm.dmPelsWidth, (i32)dm.dmPelsHeight };
		out.bits_per_pixel = util::bit_cast<Vec4b>(dm.dmBitsPerPel);
		out.refresh_rate = dm.dmDisplayFrequency;
	}

	void DisplayServerWindows::get_fullscreen_video_modes(Vector<VideoMode> & out) const
	{
		DEVMODE dm; dm.dmSize = sizeof(dm);
		for (i32 i = 0; EnumDisplaySettings(nullptr, i, &dm); ++i)
		{
			VideoMode vm;
			vm.size = { (i32)dm.dmPelsWidth, (i32)dm.dmPelsHeight };
			vm.bits_per_pixel = util::bit_cast<Vec4b>(dm.dmBitsPerPel);
			vm.refresh_rate = dm.dmDisplayFrequency;

			if (auto const it{ std::equal_range(out.begin(), out.end(), vm, [
			](VideoMode const & a, VideoMode const & b) {
				return (a.size < b.size || a.refresh_rate < b.refresh_rate || a.bits_per_pixel < b.bits_per_pixel);
			}) }
			; it.first == it.second) {
				out.emplace(it.second, std::move(vm));
			}
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// TODO: global menu stuff goes here

	void DisplayServerWindows::alert(String const & message, String const & title)
	{
	}

	String DisplayServerWindows::get_clipboard() const
	{
		ASSERT(m_main_window);
		return glfwGetClipboardString(m_main_window->handle);
	}

	void DisplayServerWindows::set_clipboard(String const & text)
	{
		ASSERT(m_main_window);
		glfwSetClipboardString(m_main_window->handle, text.c_str());
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Input::Cursor_ DisplayServerWindows::cursor_get_shape() const
	{
		return {};
	}

	void DisplayServerWindows::cursor_set_shape(Input::Cursor_ shape)
	{
	}

	void DisplayServerWindows::cursor_set_custom_image(RES const & cursor, Input::Cursor_ shape, Vec2 const & hotspot)
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Input::MouseMode_ DisplayServerWindows::mouse_get_mode() const
	{
		ASSERT(m_main_window);
		switch (glfwGetInputMode(m_main_window->handle, GLFW_CURSOR))
		{
		case GLFW_CURSOR_NORMAL: return Input::MouseMode_Normal;
		}
		return {};
	}

	void DisplayServerWindows::mouse_set_mode(Input::MouseMode_ mode)
	{
		ASSERT(m_main_window);
		glfwSetInputMode(m_main_window->handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	i32 DisplayServerWindows::mouse_get_button(Input::MouseButton_ button) const
	{
		ASSERT(m_main_window);
		return glfwGetMouseButton(m_main_window->handle, (i32)button);
	}

	Vec2 DisplayServerWindows::mouse_get_position() const
	{
		ASSERT(m_main_window);
		Vec2d v; glfwGetCursorPos(m_main_window->handle, &v[0], &v[1]);
		return { (f32)v[0], (f32)v[1] };
	}

	void DisplayServerWindows::mouse_set_position(Vec2 const & position)
	{
		ASSERT(m_main_window);
		glfwSetCursorPos(m_main_window->handle, position[0], position[1]);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool DisplayServerWindows::joystick_is_present(i32 device) const
	{
		return glfwJoystickPresent(device);
	}

	String DisplayServerWindows::joystick_get_name(i32 device) const
	{
		return glfwGetJoystickName(device);
	}

	String DisplayServerWindows::joystick_get_guid(i32 device) const
	{
		return glfwGetJoystickGUID(device);
	}

	View<f32 const> DisplayServerWindows::joystick_get_axes(i32 device) const
	{
		i32 count;
		if (auto const data{ glfwGetJoystickAxes(device, &count) }
		; 0 < count) { return { data, (size_t)count }; }
		return nullptr;
	}

	View<byte const> DisplayServerWindows::joystick_get_buttons(i32 device) const
	{
		i32 count;
		if (auto const data{ glfwGetJoystickButtons(device, &count) }
		; 0 < count) { return { data, (size_t)count }; }
		return nullptr;
	}

	View<byte const> DisplayServerWindows::joystick_get_hats(i32 device) const
	{
		i32 count;
		if (auto const data{ glfwGetJoystickHats(device, &count) }
		; 0 < count) { return { data, (size_t)count }; }
		return nullptr;
	}

	bool DisplayServerWindows::joystick_is_gamepad(i32 device) const
	{
		return glfwJoystickIsGamepad(device);
	}

	String DisplayServerWindows::gamepad_get_name(i32 device) const
	{
		return glfwGetGamepadName(device);
	}

	bool DisplayServerWindows::gamepad_update_mappings(String const & mappings)
	{
		return mappings && glfwUpdateGamepadMappings(mappings.c_str());
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	i32 DisplayServerWindows::get_screen_count() const
	{
		i32 monitor_count;
		UNUSED(glfwGetMonitors(&monitor_count));
		return monitor_count;
	}

	String DisplayServerWindows::screen_get_name(i32 screen) const
	{
		i32 monitor_count;
		GLFWmonitor ** monitors{ glfwGetMonitors(&monitor_count) };
		screen = (screen == SCREEN_OF_MAIN_WINDOW) ? window_get_current_screen() : screen;
		ASSERT((unsigned)screen < (unsigned)monitor_count);
		return glfwGetMonitorName(monitors[screen]);
	}

	Vec2i DisplayServerWindows::screen_get_physical_size(i32 screen) const
	{
		i32 monitor_count;
		GLFWmonitor ** monitors{ glfwGetMonitors(&monitor_count) };
		screen = (screen == SCREEN_OF_MAIN_WINDOW) ? window_get_current_screen() : screen;
		ASSERT((unsigned)screen < (unsigned)monitor_count);
		Vec2i size; glfwGetMonitorPhysicalSize(monitors[screen], &size[0], &size[1]);
		return size;
	}

	IntRect DisplayServerWindows::screen_get_workarea(i32 screen) const
	{
		i32 monitor_count;
		GLFWmonitor ** monitors{ glfwGetMonitors(&monitor_count) };
		screen = (screen == SCREEN_OF_MAIN_WINDOW) ? window_get_current_screen() : screen;
		ASSERT((unsigned)screen < (unsigned)monitor_count);
		IntRect rect; glfwGetMonitorWorkarea(monitors[screen], &rect[0], &rect[1], &rect[2], &rect[3]);
		return rect;
	}

	Vec2i DisplayServerWindows::screen_get_position(i32 screen) const
	{
		i32 monitor_count;
		GLFWmonitor ** monitors{ glfwGetMonitors(&monitor_count) };
		screen = (screen == SCREEN_OF_MAIN_WINDOW) ? window_get_current_screen() : screen;
		ASSERT((unsigned)screen < (unsigned)monitor_count);
		Vec2i pos; glfwGetMonitorPos(monitors[screen], &pos[0], &pos[1]);
		return pos;
	}

	Vec2i DisplayServerWindows::screen_get_size(i32 screen) const
	{
		i32 monitor_count;
		GLFWmonitor ** monitors{ glfwGetMonitors(&monitor_count) };
		screen = (screen == SCREEN_OF_MAIN_WINDOW) ? window_get_current_screen() : screen;
		ASSERT((unsigned)screen < (unsigned)monitor_count);
		GLFWvidmode const * vm{ glfwGetVideoMode(monitors[screen]) };
		return { vm->width, vm->height };
	}

	Vec2 DisplayServerWindows::screen_get_scale(i32 screen) const
	{
		i32 monitor_count;
		GLFWmonitor ** monitors{ glfwGetMonitors(&monitor_count) };
		screen = (screen == SCREEN_OF_MAIN_WINDOW) ? window_get_current_screen() : screen;
		ASSERT((unsigned)screen < (unsigned)monitor_count);
		Vec2 scale; glfwGetMonitorContentScale(monitors[screen], &scale[0], &scale[1]);
		return scale;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Vector<DS::WindowID> DisplayServerWindows::get_window_list() const
	{
		Vector<WindowID> temp{};
		temp.reserve(m_windows.size());
		for (auto const & [k, v] : m_windows) { temp.push_back(k); }
		return temp;
	}

	String DisplayServerWindows::window_get_title(WindowID window) const
	{
		ASSERT(INVALID_WINDOW_ID < window);
		_Window const & w{ *VALIDATE(util::getptr(m_windows, window)) };
		return w.title;
	}

	void DisplayServerWindows::window_set_title(String const & title, WindowID window)
	{
		ASSERT(INVALID_WINDOW_ID < window);
		_Window & w{ *VALIDATE(util::getptr(m_windows, window)) };
		if (!title) { return; }
		w.title = title;
		glfwSetWindowTitle(w.handle, w.title.c_str());
	}

	i32 DisplayServerWindows::window_get_current_screen(WindowID window) const
	{
		ASSERT(INVALID_WINDOW_ID < window);
		_Window const & w{ *VALIDATE(util::getptr(m_windows, window)) };
		return w.current_screen;
	}

	void DisplayServerWindows::window_set_current_screen(i32 screen, WindowID window)
	{
		ASSERT(INVALID_WINDOW_ID < window);
		_Window & w{ *VALIDATE(util::getptr(m_windows, window)) };
		if (w.current_screen == screen) { return; }
		w.current_screen = screen;
	}

	Vec2i DisplayServerWindows::window_get_position(WindowID window) const
	{
		ASSERT(INVALID_WINDOW_ID < window);
		_Window const & w{ *VALIDATE(util::getptr(m_windows, window)) };
		Vec2i pos; glfwGetWindowPos(w.handle, &pos[0], &pos[1]);
		return pos;
	}

	void DisplayServerWindows::window_set_position(Vec2i const & position, WindowID window)
	{
		ASSERT(INVALID_WINDOW_ID < window);
		_Window & w{ *VALIDATE(util::getptr(m_windows, window)) };
		glfwSetWindowPos(w.handle, (i32)position[0], (i32)position[1]);
	}

	Vec2i DisplayServerWindows::window_get_size(WindowID window) const
	{
		ASSERT(INVALID_WINDOW_ID < window);
		_Window const & w{ *VALIDATE(util::getptr(m_windows, window)) };
		Vec2i size; glfwGetWindowSize(w.handle, &size[0], &size[1]);
		return size;
	}

	void DisplayServerWindows::window_set_size(Vec2i const & size, WindowID window)
	{
		ASSERT(INVALID_WINDOW_ID < window);
		_Window & w{ *VALIDATE(util::getptr(m_windows, window)) };
		glfwSetWindowSize(w.handle, (i32)size[0], (i32)size[1]);
	}

	Vec2i DisplayServerWindows::window_get_real_size(WindowID window) const
	{
		_Window const & w{ *VALIDATE(util::getptr(m_windows, window)) };
		Vec2i size; glfwGetFramebufferSize(w.handle, &size[0], &size[1]);
		return size;
	}

	DS::WindowMode_ DisplayServerWindows::window_get_mode(WindowID window) const
	{
		ASSERT(INVALID_WINDOW_ID < window);
		_Window const & w{ *VALIDATE(util::getptr(m_windows, window)) };
		return w.window_mode;
	}

	void DisplayServerWindows::window_set_mode(WindowMode_ mode, WindowID window)
	{
		ASSERT(INVALID_WINDOW_ID < window);
		_Window & w{ *VALIDATE(util::getptr(m_windows, window)) };
		if (w.window_mode == mode) { return; }
		w.window_mode = mode;
		switch (mode) {
		case WindowMode_Windowed: {
			//glfwRestoreWindow(w.handle);
		} break;
		case WindowMode_Minimized: {
			//glfwIconifyWindow(w.handle);
		} break;
		case WindowMode_Maximized: {
			//glfwMaximizeWindow(w.handle);
		} break;
		case WindowMode_Fullscreen: {
		} break;
		}
	}

	bool DisplayServerWindows::window_get_flag(i32 flag, WindowID window) const
	{
		return false;
	}

	void DisplayServerWindows::window_set_flag(i32 flag, bool enabled, WindowID window)
	{
	}

	void DisplayServerWindows::request_window_attention(WindowID window)
	{
		ASSERT(INVALID_WINDOW_ID < window);
		_Window const & w{ *VALIDATE(util::getptr(m_windows, window)) };
		glfwRequestWindowAttention(w.handle);
	}

	void DisplayServerWindows::move_window_to_foreground(WindowID window)
	{
	}

	void DisplayServerWindows::window_set_visible(bool enabled, WindowID window)
	{
		ASSERT(INVALID_WINDOW_ID < window);
		_Window & w{ *VALIDATE(util::getptr(m_windows, window)) };
		enabled ? glfwShowWindow(w.handle) : glfwHideWindow(w.handle);
	}

	bool DisplayServerWindows::window_is_visible(WindowID window) const
	{
		ASSERT(INVALID_WINDOW_ID < window);
		_Window const & w{ *VALIDATE(util::getptr(m_windows, window)) };
		return glfwGetWindowAttrib(w.handle, GLFW_VISIBLE);
	}

	Vec2 DisplayServerWindows::window_get_scale(WindowID window) const
	{
		ASSERT(INVALID_WINDOW_ID < window);
		_Window const & w{ *VALIDATE(util::getptr(m_windows, window)) };
		Vec2 scale; glfwGetWindowContentScale(w.handle, &scale[0], &scale[1]);
		return scale;
	}

	void DisplayServerWindows::window_grab_focus(WindowID window)
	{
		ASSERT(INVALID_WINDOW_ID < window);
		_Window & w{ *VALIDATE(util::getptr(m_windows, window)) };
		glfwFocusWindow(w.handle);
	}

	bool DisplayServerWindows::window_has_focus(WindowID window) const
	{
		ASSERT(INVALID_WINDOW_ID < window);
		_Window const & w{ *VALIDATE(util::getptr(m_windows, window)) };
		return glfwGetWindowAttrib(w.handle, GLFW_FOCUSED);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void DisplayServerWindows::poll_events()
	{
		glfwPollEvents();
	}

	void DisplayServerWindows::swap_buffers()
	{
		ASSERT(m_main_window);
		glfwMakeContextCurrent(m_main_window->handle);
		glfwSwapBuffers(m_main_window->handle);
	}

	void DisplayServerWindows::set_native_icon(Path const & value)
	{
	}

	void DisplayServerWindows::set_icon(byte const * data, i32 width, i32 height)
	{
		ASSERT(m_main_window);
		ASSERT(data);
		ASSERT(0 < width);
		ASSERT(0 < height);
		GLFWimage icon;
		icon.pixels = (byte *)data;
		icon.width = width;
		icon.height = height;
		glfwSetWindowIcon(m_main_window->handle, 1, &icon);
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