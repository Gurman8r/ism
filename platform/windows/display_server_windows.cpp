#include <platform/windows/display_server_windows.hpp>
#include <scene/main/scene_tree.hpp>

#ifdef INPUT
#undef INPUT
#endif

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

	DisplayServerWindows::DisplayServerWindows(DisplayServerSettings const & settings)
	{
		ASSERT(glfwInit() == GLFW_TRUE);

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
		glfwWindowHint(GLFW_AUTO_ICONIFY, settings.hints & WindowFlags_AutoIconify);
		glfwWindowHint(GLFW_CENTER_CURSOR, settings.hints & WindowFlags_CenterCursor);
		glfwWindowHint(GLFW_DECORATED, settings.hints & WindowFlags_Decorated);
		glfwWindowHint(GLFW_DOUBLEBUFFER, settings.hints & WindowFlags_Doublebuffer);
		glfwWindowHint(GLFW_FLOATING, settings.hints & WindowFlags_Floating);
		glfwWindowHint(GLFW_FOCUS_ON_SHOW, settings.hints & WindowFlags_FocusOnShow);
		glfwWindowHint(GLFW_FOCUSED, settings.hints & WindowFlags_Focused);
		glfwWindowHint(GLFW_MAXIMIZED, settings.hints & WindowFlags_Maximized);
		glfwWindowHint(GLFW_RESIZABLE, settings.hints & WindowFlags_Resizable);
		glfwWindowHint(GLFW_VISIBLE, settings.hints & WindowFlags_Visible);

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

		m_glfw_window = glfwCreateWindow(
			(int32_t)settings.video.size[0],
			(int32_t)settings.video.size[1],
			(cstring)settings.title.c_str(),
			(GLFWmonitor *)settings.monitor,
			(GLFWwindow *)settings.share);

		if (!m_glfw_window) {
			CRASH("FAILED CREATING GLFW WINDOW");
		}

		set_current_context((WindowID)m_glfw_window);

		if (settings.hints & WindowFlags_Maximized) { maximize_window((WindowID)m_glfw_window); }
	}

	DisplayServerWindows::~DisplayServerWindows()
	{
		glfwDestroyWindow(m_glfw_window);
		glfwTerminate();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	VideoMode const & DisplayServerWindows::get_desktop_video_mode() const
	{
		static VideoMode result{};
		if (static bool once{}; !once && (once = true))
		{
			DEVMODE dm;
			dm.dmSize = sizeof(dm);
			EnumDisplaySettings(nullptr, ENUM_CURRENT_SETTINGS, &dm);

			result = {
				{ (int32_t)dm.dmPelsWidth, (int32_t)dm.dmPelsHeight },
				util::bit_cast<Vec4b>(dm.dmBitsPerPel),
				-1
			};
		}
		return result;
	}

	List<VideoMode> const & DisplayServerWindows::get_fullscreen_video_modes() const
	{
		static List<VideoMode> result{};
		if (static bool once{}; !once && (once = true))
		{
			DEVMODE dm;
			dm.dmSize = sizeof(dm);
			for (int32_t count = 0; EnumDisplaySettings(nullptr, count, &dm); ++count)
			{
				VideoMode mode{
					{ (int32_t)dm.dmPelsWidth, (int32_t)dm.dmPelsHeight },
					util::bit_cast<Vec4b>(dm.dmBitsPerPel),
					-1
				};
				if (!result.contains(mode)) { result.push_back(mode); }
			}
		}
		return result;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	CursorID DisplayServerWindows::create_custom_cursor(int32_t w, int32_t h, byte const * p, int32_t x, int32_t y)
	{
		GLFWimage img{ w, h, (uint8_t *)p };
		return (CursorID)glfwCreateCursor(&img, x, y);
	}

	CursorID DisplayServerWindows::create_standard_cursor(CursorShape_ shape)
	{
		return (CursorID)glfwCreateStandardCursor(std::invoke([shape]() noexcept
		{
			switch (shape)
			{
			default: return 0;
			case CursorShape_Arrow: return GLFW_ARROW_CURSOR;
			case CursorShape_IBeam: return GLFW_IBEAM_CURSOR;
			case CursorShape_Crosshair: return GLFW_CROSSHAIR_CURSOR;
			case CursorShape_PointingHand: return GLFW_POINTING_HAND_CURSOR;
			case CursorShape_EW: return GLFW_RESIZE_EW_CURSOR;
			case CursorShape_NS: return GLFW_RESIZE_NS_CURSOR;
			case CursorShape_NESW: return GLFW_RESIZE_NESW_CURSOR;
			case CursorShape_NWSE: return GLFW_RESIZE_NWSE_CURSOR;
			case CursorShape_ResizeAll: return GLFW_RESIZE_ALL_CURSOR;
			case CursorShape_NotAllowed: return GLFW_NOT_ALLOWED_CURSOR;

				// glfw doesn't have these
			case CursorShape_HResize: return GLFW_HRESIZE_CURSOR;
			case CursorShape_VResize: return GLFW_VRESIZE_CURSOR;
			case CursorShape_Hand: return GLFW_HAND_CURSOR;
			}
		}));
	}

	void DisplayServerWindows::destroy_cursor(CursorID value)
	{
		glfwDestroyCursor((GLFWcursor *)value);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	WindowID DisplayServerWindows::get_current_context() const
	{
		return (WindowID)glfwGetCurrentContext();
	}

	void DisplayServerWindows::set_current_context(WindowID id)
	{
		glfwMakeContextCurrent(VALIDATE((GLFWwindow *)id));
	}

	void DisplayServerWindows::poll_events()
	{
		glfwPollEvents();
	}

	void DisplayServerWindows::swap_buffers(WindowID id)
	{
		glfwSwapBuffers(VALIDATE((GLFWwindow *)id));
	}

	void DisplayServerWindows::swap_interval(int32_t value)
	{
		glfwSwapInterval(value);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void DisplayServerWindows::focus_window(WindowID id)
	{
		glfwFocusWindow(VALIDATE((GLFWwindow *)id));
	}

	void DisplayServerWindows::hide_window(WindowID id)
	{
		glfwHideWindow(VALIDATE((GLFWwindow *)id));
	}

	void DisplayServerWindows::iconify_window(WindowID id)
	{
		glfwIconifyWindow(VALIDATE((GLFWwindow *)id));
	}

	void DisplayServerWindows::maximize_window(WindowID id)
	{
		glfwMaximizeWindow(VALIDATE((GLFWwindow *)id));
	}

	void DisplayServerWindows::restore_window(WindowID id)
	{
		glfwRestoreWindow(VALIDATE((GLFWwindow *)id));
	}

	void DisplayServerWindows::request_window_attention(WindowID id)
	{
		glfwRequestWindowAttention(VALIDATE((GLFWwindow *)id));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	String DisplayServerWindows::window_get_clipboard(WindowID id) const
	{
		return glfwGetClipboardString(VALIDATE((GLFWwindow *)id));
	}

	Vec2f DisplayServerWindows::window_get_content_scale(WindowID id) const
	{
		float_t x, y;
		glfwGetWindowContentScale(VALIDATE((GLFWwindow *)id), &x, &y);
		return { x, y };
	}

	IntRect DisplayServerWindows::window_get_frame_size(WindowID id) const
	{
		int32_t l, r, t, b;
		glfwGetWindowFrameSize(VALIDATE((GLFWwindow *)id), &l, &r, &t, &b);
		return { l, t, r, b };
	}

	Vec2i DisplayServerWindows::window_get_framebuffer_size(WindowID id) const
	{
		int32_t x, y;
		glfwGetFramebufferSize(VALIDATE((GLFWwindow *)id), &x, &y);
		return { x, y };
	}

	int32_t DisplayServerWindows::window_get_input_mode(WindowID id, InputMode_ value) const
	{
		return glfwGetInputMode(VALIDATE((GLFWwindow *)id), value);
	}

	InputAction_ DisplayServerWindows::window_get_key(WindowID id, KeyCode_ value) const
	{
		return (InputAction_)glfwGetKey(VALIDATE((GLFWwindow *)id), value);
	}

	InputAction_ DisplayServerWindows::window_get_mouse_button(WindowID id, MouseButton_ value) const
	{
		return (InputAction_)glfwGetMouseButton(VALIDATE((GLFWwindow *)id), value);
	}

	Vec2d DisplayServerWindows::window_get_mouse_position(WindowID id) const
	{
		double_t x, y;
		glfwGetCursorPos(VALIDATE((GLFWwindow *)id), &x, &y);
		return { x, y };
	}

	void * DisplayServerWindows::window_get_native_handle(WindowID id) const
	{
#if SYSTEM_WINDOWS
		return (WindowID)glfwGetWin32Window(VALIDATE((GLFWwindow *)id));
#else
		return (WindowID)VALIDATE((GLFWwindow *)id);
#endif
	}

	float_t DisplayServerWindows::window_get_opacity(WindowID id) const
	{
		return glfwGetWindowOpacity(VALIDATE((GLFWwindow *)id));
	}

	Vec2i DisplayServerWindows::window_get_position(WindowID id) const
	{
		int32_t x, y;
		glfwGetWindowPos(VALIDATE((GLFWwindow *)id), &x, &y);
		return { x, y };
	}

	Vec2i DisplayServerWindows::window_get_size(WindowID id) const
	{
		int32_t x, y;
		glfwGetWindowSize(VALIDATE((GLFWwindow *)id), &x, &y);
		return { x, y };
	}

	bool DisplayServerWindows::window_get_should_close(WindowID id) const
	{
		return glfwWindowShouldClose(VALIDATE((GLFWwindow *)id));
	}

	void * DisplayServerWindows::window_get_user_pointer(WindowID id) const
	{
		return glfwGetWindowUserPointer(VALIDATE((GLFWwindow *)id));
	}

	bool DisplayServerWindows::window_is_decorated(WindowID id) const
	{
		return glfwGetWindowAttrib(VALIDATE((GLFWwindow *)id), GLFW_DECORATED);
	}

	bool DisplayServerWindows::window_is_floating(WindowID id) const
	{
		return glfwGetWindowAttrib(VALIDATE((GLFWwindow *)id), GLFW_FLOATING);
	}

	bool DisplayServerWindows::window_is_focused(WindowID id) const
	{
		return glfwGetWindowAttrib(VALIDATE((GLFWwindow *)id), GLFW_FOCUS_ON_SHOW);
	}

	bool DisplayServerWindows::window_is_hovered(WindowID id) const
	{
		return glfwGetWindowAttrib(VALIDATE((GLFWwindow *)id), GLFW_HOVERED);
	}

	bool DisplayServerWindows::window_is_iconified(WindowID id) const
	{
		return glfwGetWindowAttrib(VALIDATE((GLFWwindow *)id), GLFW_ICONIFIED);
	}

	bool DisplayServerWindows::window_is_maximized(WindowID id) const
	{
		return glfwGetWindowAttrib(VALIDATE((GLFWwindow *)id), GLFW_MAXIMIZED);
	}

	bool DisplayServerWindows::window_is_resizable(WindowID id) const
	{
		return glfwGetWindowAttrib(VALIDATE((GLFWwindow *)id), GLFW_RESIZABLE);
	}

	bool DisplayServerWindows::window_is_transparent(WindowID id) const
	{
		return glfwGetWindowAttrib(VALIDATE((GLFWwindow *)id), GLFW_TRANSPARENT_FRAMEBUFFER);
	}

	bool DisplayServerWindows::window_is_visible(WindowID id) const
	{
		return glfwGetWindowAttrib(VALIDATE((GLFWwindow *)id), GLFW_VISIBLE);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void DisplayServerWindows::window_set_clipboard(WindowID id, String const & value)
	{
		glfwSetClipboardString(VALIDATE((GLFWwindow *)id), value.c_str());
	}

	void DisplayServerWindows::window_set_cursor(WindowID id, CursorID value)
	{
		glfwSetCursor(VALIDATE((GLFWwindow *)id), (GLFWcursor *)value);
	}

	void DisplayServerWindows::window_set_cursor_mode(WindowID id, int32_t value)
	{
		glfwSetInputMode(VALIDATE((GLFWwindow *)id), GLFW_CURSOR, std::invoke([value]() noexcept {
			switch (value) {
			case CursorMode_Normal: return GLFW_CURSOR_NORMAL;
			case CursorMode_Hidden: return GLFW_CURSOR_HIDDEN;
			case CursorMode_Disabled: return GLFW_CURSOR_DISABLED;
			}
			return 0;
		}));
	}

	void DisplayServerWindows::window_set_icons(WindowID id, int32_t width, int32_t height, uint8_t * pixels, int32_t count)
	{
		GLFWimage img{ width, height, pixels };

		glfwSetWindowIcon(VALIDATE((GLFWwindow *)id), count, &img);
	}

	void DisplayServerWindows::window_set_input_mode(WindowID id, int32_t mode, int32_t value)
	{
		glfwSetInputMode(VALIDATE((GLFWwindow *)id), mode, value);
	}

	void DisplayServerWindows::window_set_monitor(WindowID id, MonitorID monitor, IntRect const & bounds)
	{
	}

	void DisplayServerWindows::window_set_mouse_position(WindowID id, Vec2d const & value)
	{
		glfwSetCursorPos(VALIDATE((GLFWwindow *)id), value[0], value[1]);
	}

	void DisplayServerWindows::window_set_opacity(WindowID id, float_t value)
	{
		glfwSetWindowOpacity(VALIDATE((GLFWwindow *)id), value);
	}

	void DisplayServerWindows::window_set_position(WindowID id, Vec2i const & value)
	{
		glfwSetWindowPos(VALIDATE((GLFWwindow *)id), (int32_t)value[0], (int32_t)value[1]);
	}

	void DisplayServerWindows::window_set_resizable(WindowID id, bool value)
	{
		glfwSetWindowAttrib(VALIDATE((GLFWwindow *)id), GLFW_RESIZABLE, value);
	}

	void DisplayServerWindows::window_set_should_close(WindowID id, bool value)
	{
		glfwSetWindowShouldClose(VALIDATE((GLFWwindow *)id), value);
	}

	void DisplayServerWindows::window_set_size(WindowID id, Vec2i const & value)
	{
		glfwSetWindowSize(VALIDATE((GLFWwindow *)id), (int32_t)value[0], (int32_t)value[1]);
	}

	void DisplayServerWindows::window_set_title(WindowID id, String const & value)
	{
		glfwSetWindowTitle(VALIDATE((GLFWwindow *)id), value.c_str());
	}

	void DisplayServerWindows::window_set_user_pointer(WindowID id, void * value)
	{
		glfwSetWindowUserPointer(VALIDATE((GLFWwindow *)id), value);
	}

	void DisplayServerWindows::window_set_decorated(WindowID id, bool value)
	{
		glfwSetWindowAttrib(VALIDATE((GLFWwindow *)id), GLFW_DECORATED, value);
	}

	void DisplayServerWindows::window_set_floating(WindowID id, bool value)
	{
		glfwSetWindowAttrib(VALIDATE((GLFWwindow *)id), GLFW_FLOATING, value);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	WindowCharCallback DisplayServerWindows::window_set_char_callback(WindowID id, WindowCharCallback value)
	{
		return reinterpret_cast<WindowCharCallback>(glfwSetCharCallback(VALIDATE((GLFWwindow *)id), reinterpret_cast<GLFWcharfun>(value)));
	}

	WindowCharModsCallback DisplayServerWindows::window_set_char_mods_callback(WindowID id, WindowCharModsCallback value)
	{
		return reinterpret_cast<WindowCharModsCallback>(glfwSetCharModsCallback(VALIDATE((GLFWwindow *)id), reinterpret_cast<GLFWcharmodsfun>(value)));
	}

	WindowCloseCallback DisplayServerWindows::window_set_close_callback(WindowID id, WindowCloseCallback value)
	{
		return reinterpret_cast<WindowCloseCallback>(glfwSetWindowCloseCallback(VALIDATE((GLFWwindow *)id), reinterpret_cast<GLFWwindowclosefun>(value)));
	}

	WindowContentCallback DisplayServerWindows::window_set_content_scale_callback(WindowID id, WindowContentCallback value)
	{
		return reinterpret_cast<WindowContentCallback>(glfwSetWindowContentScaleCallback(VALIDATE((GLFWwindow *)id), reinterpret_cast<GLFWwindowcontentscalefun>(value)));
	}

	WindowDropCallback DisplayServerWindows::window_set_drop_callback(WindowID id, WindowDropCallback value)
	{
		return reinterpret_cast<WindowDropCallback>(glfwSetDropCallback(VALIDATE((GLFWwindow *)id), reinterpret_cast<GLFWdropfun>(value)));
	}

	WindowFocusCallback DisplayServerWindows::window_set_focus_callback(WindowID id, WindowFocusCallback value)
	{
		return reinterpret_cast<WindowFocusCallback>(glfwSetWindowFocusCallback(VALIDATE((GLFWwindow *)id), reinterpret_cast<GLFWwindowfocusfun>(value)));
	}

	WindowFramebufferResizeCallback DisplayServerWindows::window_set_framebuffer_resize_callback(WindowID id, WindowFramebufferResizeCallback value)
	{
		return reinterpret_cast<WindowFramebufferResizeCallback>(glfwSetFramebufferSizeCallback(VALIDATE((GLFWwindow *)id), reinterpret_cast<GLFWframebuffersizefun>(value)));
	}

	WindowIconifyCallback DisplayServerWindows::window_set_iconify_callback(WindowID id, WindowIconifyCallback value)
	{
		return reinterpret_cast<WindowIconifyCallback>(glfwSetWindowIconifyCallback(VALIDATE((GLFWwindow *)id), reinterpret_cast<GLFWwindowiconifyfun>(value)));
	}

	WindowKeyCallback DisplayServerWindows::window_set_key_callback(WindowID id, WindowKeyCallback value)
	{
		return reinterpret_cast<WindowKeyCallback>(glfwSetKeyCallback(VALIDATE((GLFWwindow *)id), reinterpret_cast<GLFWkeyfun>(value)));
	}

	WindowMaximizeCallback DisplayServerWindows::window_set_maximize_callback(WindowID id, WindowMaximizeCallback value)
	{
		return reinterpret_cast<WindowMaximizeCallback>(glfwSetWindowMaximizeCallback(VALIDATE((GLFWwindow *)id), reinterpret_cast<GLFWwindowmaximizefun>(value)));
	}

	WindowMouseButtonCallback DisplayServerWindows::window_set_mouse_button_callback(WindowID id, WindowMouseButtonCallback value)
	{
		return reinterpret_cast<WindowMouseButtonCallback>(glfwSetMouseButtonCallback(VALIDATE((GLFWwindow *)id), reinterpret_cast<GLFWmousebuttonfun>(value)));
	}

	WindowMouseEnterCallback DisplayServerWindows::window_set_mouse_enter_callback(WindowID id, WindowMouseEnterCallback value)
	{
		return reinterpret_cast<WindowMouseEnterCallback>(glfwSetCursorEnterCallback(VALIDATE((GLFWwindow *)id), reinterpret_cast<GLFWcursorenterfun>(value)));
	}

	WindowMousePositionCallback DisplayServerWindows::window_set_mouse_position_callback(WindowID id, WindowMousePositionCallback value)
	{
		return reinterpret_cast<WindowMousePositionCallback>(glfwSetCursorPosCallback(VALIDATE((GLFWwindow *)id), reinterpret_cast<GLFWcursorposfun>(value)));
	}

	WindowPositionCallback DisplayServerWindows::window_set_position_callback(WindowID id, WindowPositionCallback value)
	{
		return reinterpret_cast<WindowPositionCallback>(glfwSetWindowPosCallback(VALIDATE((GLFWwindow *)id), reinterpret_cast<GLFWwindowposfun>(value)));
	}

	WindowRefreshCallback DisplayServerWindows::window_set_refresh_callback(WindowID id, WindowRefreshCallback value)
	{
		return reinterpret_cast<WindowRefreshCallback>(glfwSetWindowRefreshCallback(VALIDATE((GLFWwindow *)id), reinterpret_cast<GLFWwindowrefreshfun>(value)));
	}

	WindowScrollCallback DisplayServerWindows::window_set_scroll_callback(WindowID id, WindowScrollCallback value)
	{
		return reinterpret_cast<WindowScrollCallback>(glfwSetScrollCallback(VALIDATE((GLFWwindow *)id), reinterpret_cast<GLFWscrollfun>(value)));
	}

	WindowSizeCallback DisplayServerWindows::window_set_size_callback(WindowID id, WindowSizeCallback value)
	{
		return reinterpret_cast<WindowSizeCallback>(glfwSetWindowSizeCallback(VALIDATE((GLFWwindow *)id), reinterpret_cast<GLFWwindowposfun>(value)));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

}