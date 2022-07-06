#ifndef _ISM_DISPLAY_SERVER_HPP_
#define _ISM_DISPLAY_SERVER_HPP_

#include <core/os/os.hpp>
#include <core/input/input.hpp>
#include <core/io/image.hpp>

// types
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	DECL_HANDLE(CursorID);
	DECL_HANDLE(MonitorID);
	DECL_HANDLE(WindowID);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ALIAS(WindowCharCallback)				void(*)(WindowID, uint32_t);
	ALIAS(WindowCharModsCallback)			void(*)(WindowID, uint32_t, int32_t);
	ALIAS(WindowCloseCallback)				void(*)(WindowID);
	ALIAS(WindowContentCallback)			void(*)(WindowID, float_t, float_t);
	ALIAS(WindowDropCallback)				void(*)(WindowID, int32_t, cstring[]);
	ALIAS(WindowFocusCallback)				void(*)(WindowID, int32_t);
	ALIAS(WindowFramebufferResizeCallback)	void(*)(WindowID, int32_t, int32_t);
	ALIAS(WindowIconifyCallback)			void(*)(WindowID, int32_t);
	ALIAS(WindowKeyCallback)				void(*)(WindowID, int32_t, int32_t, int32_t, int32_t);
	ALIAS(WindowMaximizeCallback)			void(*)(WindowID, int32_t);
	ALIAS(WindowMouseButtonCallback)		void(*)(WindowID, int32_t, int32_t, int32_t);
	ALIAS(WindowMouseEnterCallback)			void(*)(WindowID, int32_t);
	ALIAS(WindowMousePositionCallback)		void(*)(WindowID, double_t, double_t);
	ALIAS(WindowPositionCallback)			void(*)(WindowID, int32_t, int32_t);
	ALIAS(WindowRefreshCallback)			void(*)(WindowID);
	ALIAS(WindowSizeCallback)				void(*)(WindowID, int32_t, int32_t);
	ALIAS(WindowScrollCallback)				void(*)(WindowID, double_t, double_t);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// cursor mode
	ALIAS(CursorMode) int32_t;
	enum CursorMode_ : CursorMode
	{
		CursorMode_Normal		, // normal
		CursorMode_Hidden		, // hidden
		CursorMode_Disabled		, // disabled
	};

	// cursor shape
	ALIAS(CursorShape) int32_t;
	enum CursorShape_ : CursorShape
	{
		CursorShape_Arrow			, // arrow
		CursorShape_IBeam			, // ibeam
		CursorShape_Crosshair		, // crosshair
		CursorShape_PointingHand	, // pointing hand
		CursorShape_EW				, // ew
		CursorShape_NS				, // ns
		CursorShape_NESW			, // nesw
		CursorShape_NWSE			, // nwse
		CursorShape_ResizeAll		, // resize all
		CursorShape_NotAllowed		, // not allowed
		CursorShape_HResize			, // hresize
		CursorShape_VResize			, // vresize
		CursorShape_Hand			, // hand
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// window flags
	ALIAS(WindowFlags) int32_t;
	enum WindowFlags_ : WindowFlags
	{
		WindowFlags_None			= 0		, // none
		WindowFlags_AutoIconify		= 1 << 0, // auto iconify
		WindowFlags_CenterCursor	= 1 << 1, // center cursor
		WindowFlags_Decorated		= 1 << 2, // decorated
		WindowFlags_Doublebuffer	= 1 << 3, // doublebuffer
		WindowFlags_Floating		= 1 << 4, // floating
		WindowFlags_FocusOnShow		= 1 << 5, // focus on show
		WindowFlags_Focused			= 1 << 6, // focused
		WindowFlags_Maximized		= 1 << 7, // maximized
		WindowFlags_Resizable		= 1 << 8, // resizable
		WindowFlags_Visible			= 1 << 9, // visible

		WindowFlags_Default
			= WindowFlags_AutoIconify
			| WindowFlags_Decorated
			| WindowFlags_Doublebuffer
			| WindowFlags_FocusOnShow
			| WindowFlags_Focused
			| WindowFlags_Resizable
			| WindowFlags_Visible,

		WindowFlags_Default_Maximized
			= WindowFlags_AutoIconify
			| WindowFlags_Decorated
			| WindowFlags_Doublebuffer
			| WindowFlags_FocusOnShow
			| WindowFlags_Focused
			| WindowFlags_Maximized
			| WindowFlags_Resizable,
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// renderer api
	ALIAS(RendererAPI) int32_t;
	enum RendererAPI_ : RendererAPI
	{
		RendererAPI_Unknown		, // unknown
		RendererAPI_OpenGL		, // opengl
		RendererAPI_Vulkan		, // vulkan
		RendererAPI_DirectX		, // directx
	};

	// renderer profile
	ALIAS(RendererProfile) int32_t;
	enum RendererProfile_ : RendererProfile
	{
		RendererProfile_Any		, // any
		RendererProfile_Core	, // core
		RendererProfile_Compat	, // compat
		RendererProfile_Debug	, // debug
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// video mode
	struct VideoMode
	{
		Vec2i	size			{ 1280, 720 };
		Vec4b	bits_per_pixel	{ 8, 8, 8, 8 };
		int32_t	refresh_rate	{ -1 };

		int32_t compare(VideoMode const & other) const noexcept {
			if (this == std::addressof(other)) { return 0; }
			if (auto cmp{ CMP(size, other.size) }; cmp != 0) { return cmp; }
			if (auto cmp{ CMP(bits_per_pixel, other.bits_per_pixel) }; cmp != 0) { return cmp; }
			if (auto cmp{ CMP(refresh_rate, other.refresh_rate) }; cmp != 0) { return cmp; }
			return 0;
		}

		bool operator==(VideoMode const & other) const noexcept { return compare(other) == 0; }
		bool operator!=(VideoMode const & other) const noexcept { return compare(other) != 0; }
		bool operator< (VideoMode const & other) const noexcept { return compare(other) < 0; }
		bool operator> (VideoMode const & other) const noexcept { return compare(other) > 0; }
		bool operator<=(VideoMode const & other) const noexcept { return compare(other) <= 0; }
		bool operator>=(VideoMode const & other) const noexcept { return compare(other) >= 0; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// context settings
	struct ContextSettings
	{
		RendererAPI_ api{ RendererAPI_OpenGL };
		
		int32_t major{ 4 };
		
		int32_t minor{ 6 };
		
		RendererProfile_ profile{ RendererProfile_Compat };
		
		int32_t depth_bits{ 24 };
		int32_t stencil_bits{ 8 };
		bool multisample{ true };
		bool srgb_capable{ false };
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// window settings
	struct DisplayServerSettings
	{
		String			title	{ "New Window" };
		VideoMode		video	{};
		ContextSettings	context	{};
		int32_t			hints	{ WindowFlags_Default };
		MonitorID		monitor	{};
		WindowID		share	{};
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// display server
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API DisplayServer : public Object
	{
		OBJECT_COMMON(DisplayServer, Object);

		static DisplayServer * g_singleton;

	protected:
		DisplayServer();
		
	public:
		virtual ~DisplayServer() override;

		FORCE_INLINE static DisplayServer * get_singleton() noexcept { return g_singleton; }

	public:
		virtual VideoMode const & get_desktop_video_mode() const = 0;
		virtual List<VideoMode> const & get_fullscreen_video_modes() const = 0;

	public:
		virtual CursorID create_custom_cursor(int32_t w, int32_t h, byte const * p, int32_t x, int32_t y) = 0;
		virtual CursorID create_standard_cursor(CursorShape_ shape) = 0;
		virtual void destroy_cursor(CursorID value) = 0;

	public:
		virtual WindowID get_current_context() const = 0;
		virtual void set_current_context(WindowID window) = 0;
		virtual void poll_events() = 0;
		virtual void swap_buffers(WindowID window) = 0;
		virtual void swap_interval(int32_t value) = 0;

	public:
		virtual void focus_window(WindowID window) = 0;
		virtual void hide_window(WindowID window) = 0;
		virtual void iconify_window(WindowID window) = 0;
		virtual void maximize_window(WindowID window) = 0;
		virtual void restore_window(WindowID window) = 0;
		virtual void request_window_attention(WindowID window) = 0;

	public:
		inline IntRect window_get_bounds(WindowID window) const { return { window_get_position(window), window_get_size(window) }; }
		virtual String window_get_clipboard(WindowID window) const = 0;
		virtual Vec2f window_get_content_scale(WindowID window) const = 0;
		virtual IntRect window_get_frame_size(WindowID window) const = 0;
		virtual Vec2i window_get_framebuffer_size(WindowID window) const = 0;
		virtual int32_t window_get_input_mode(WindowID window, InputMode_ value) const = 0;
		virtual InputAction_ window_get_key(WindowID window, KeyCode_ value) const = 0;
		virtual InputAction_ window_get_mouse_button(WindowID window, MouseButton_ value) const = 0;
		virtual Vec2d window_get_mouse_position(WindowID window) const = 0;
		virtual void * window_get_native_handle(WindowID window) const = 0;
		virtual float_t window_get_opacity(WindowID window) const = 0;
		virtual Vec2i window_get_position(WindowID window) const = 0;
		virtual Vec2i window_get_size(WindowID window) const = 0;
		virtual bool window_get_should_close(WindowID window) const = 0;
		virtual void * window_get_user_pointer(WindowID window) const = 0;
		virtual bool window_is_decorated(WindowID window) const = 0;
		virtual bool window_is_floating(WindowID window) const = 0;
		virtual bool window_is_focused(WindowID window) const = 0;
		virtual bool window_is_hovered(WindowID window) const = 0;
		virtual bool window_is_iconified(WindowID window) const = 0;
		virtual bool window_is_maximized(WindowID window) const = 0;
		virtual bool window_is_resizable(WindowID window) const = 0;
		virtual bool window_is_transparent(WindowID window) const = 0;
		virtual bool window_is_visible(WindowID window) const = 0;

	public:
		virtual void window_set_clipboard(WindowID window, String const & value) = 0;
		virtual void window_set_cursor(WindowID window, CursorID value) = 0;
		virtual void window_set_cursor_mode(WindowID window, int32_t value) = 0;
		virtual void window_set_icons(WindowID window, int32_t width, int32_t height, uint8_t * pixels, int32_t count = 1) = 0;
		virtual void window_set_input_mode(WindowID window, int32_t mode, int32_t value) = 0;
		virtual void window_set_monitor(WindowID window, MonitorID monitor, IntRect const & bounds) = 0;
		virtual void window_set_mouse_position(WindowID window, Vec2d const & value) = 0;
		virtual void window_set_opacity(WindowID window, float_t value) = 0;
		virtual void window_set_position(WindowID window, Vec2i const & value) = 0;
		virtual void window_set_should_close(WindowID window, bool value) = 0;
		virtual void window_set_size(WindowID window, Vec2i const & value) = 0;
		virtual void window_set_title(WindowID window, String const & value) = 0;
		virtual void window_set_user_pointer(WindowID window, void * value) = 0;
		virtual void window_set_decorated(WindowID window, bool value) = 0;
		virtual void window_set_floating(WindowID window, bool value) = 0;
		virtual void window_set_resizable(WindowID window, bool value) = 0;

	public:
		virtual WindowCharCallback window_set_char_callback(WindowID window, WindowCharCallback value) = 0;
		virtual WindowCharModsCallback window_set_char_mods_callback(WindowID window, WindowCharModsCallback value) = 0;
		virtual WindowCloseCallback window_set_close_callback(WindowID window, WindowCloseCallback value) = 0;
		virtual WindowContentCallback window_set_content_scale_callback(WindowID window, WindowContentCallback value) = 0;
		virtual WindowDropCallback window_set_drop_callback(WindowID window, WindowDropCallback value) = 0;
		virtual WindowFocusCallback window_set_focus_callback(WindowID window, WindowFocusCallback value) = 0;
		virtual WindowFramebufferResizeCallback window_set_framebuffer_resize_callback(WindowID window, WindowFramebufferResizeCallback value) = 0;
		virtual WindowIconifyCallback window_set_iconify_callback(WindowID window, WindowIconifyCallback value) = 0;
		virtual WindowKeyCallback window_set_key_callback(WindowID window, WindowKeyCallback value) = 0;
		virtual WindowMaximizeCallback window_set_maximize_callback(WindowID window, WindowMaximizeCallback value) = 0;
		virtual WindowMouseButtonCallback window_set_mouse_button_callback(WindowID window, WindowMouseButtonCallback value) = 0;
		virtual WindowMouseEnterCallback window_set_mouse_enter_callback(WindowID window, WindowMouseEnterCallback value) = 0;
		virtual WindowMousePositionCallback window_set_mouse_position_callback(WindowID window, WindowMousePositionCallback value) = 0;
		virtual WindowPositionCallback window_set_position_callback(WindowID window, WindowPositionCallback value) = 0;
		virtual WindowRefreshCallback window_set_refresh_callback(WindowID window, WindowRefreshCallback value) = 0;
		virtual WindowScrollCallback window_set_scroll_callback(WindowID window, WindowScrollCallback value) = 0;
		virtual WindowSizeCallback window_set_size_callback(WindowID window, WindowSizeCallback value) = 0;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ALIAS(DS) DisplayServer;

#define DISPLAY_SERVER (ism::DisplayServer::get_singleton())

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// events
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API WindowCharEvent : public Event
	{
		EVENT_COMMON(WindowCharEvent, Event);

	public:
		WindowID window;
		uint32_t codepoint;

		WindowCharEvent(WindowID window, uint32_t codepoint) noexcept
			: window{ window }, codepoint{ codepoint } {}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API WindowCharModsEvent : public Event
	{
		EVENT_COMMON(WindowCharModsEvent, Event);

	public:
		WindowID window;
		uint32_t codepoint;
		int32_t mods;

		WindowCharModsEvent(WindowID window, uint32_t codepoint, int32_t mods) noexcept
			: window{ window }, codepoint{ codepoint }, mods{ mods } {}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API WindowCloseEvent : public Event
	{
		EVENT_COMMON(WindowCloseEvent, Event);

	public:
		WindowID window;

		WindowCloseEvent(WindowID window) noexcept
			: window{ window } {}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API WindowContentScaleEvent : public Event
	{
		EVENT_COMMON(WindowContentScaleEvent, Event);

	public:
		WindowID window;
		float_t xscale;
		float_t yscale;

		WindowContentScaleEvent(WindowID window, float_t xscale, float_t yscale) noexcept
			: window{ window }, xscale{ xscale }, yscale{ yscale } {}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API WindowDropEvent : public Event
	{
		EVENT_COMMON(WindowDropEvent, Event);

	public:
		WindowID window;
		int32_t path_count;
		cstring * paths;

		WindowDropEvent(WindowID window, int32_t path_count, cstring paths[]) noexcept
			: window{ window }, path_count{ path_count }, paths{ paths } {}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API WindowFocusEvent : public Event
	{
		EVENT_COMMON(WindowFocusEvent, Event);

	public:
		WindowID window;
		int32_t focused;

		WindowFocusEvent(WindowID window, int32_t focused) noexcept
			: window{ window }, focused{ focused } {}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API WindowFramebufferResizeEvent : public Event
	{
		EVENT_COMMON(WindowFramebufferResizeEvent, Event);

	public:
		WindowID window;
		int32_t width;
		int32_t height;

		WindowFramebufferResizeEvent(WindowID window, int32_t width, int32_t height) noexcept
			: window{ window }, width{ width }, height{ height } {}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API WindowIconifyEvent : public Event
	{
		EVENT_COMMON(WindowIconifyEvent, Event);

	public:
		WindowID window;
		int32_t iconified;

		WindowIconifyEvent(WindowID window, int32_t iconified) noexcept
			: window{ window }, iconified{ iconified } {}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API WindowKeyEvent : public Event
	{
		EVENT_COMMON(WindowKeyEvent, Event);

	public:
		WindowID window;
		KeyCode_ key;
		int32_t scancode;
		InputAction_ action;
		int32_t mods;

		WindowKeyEvent(WindowID window, int32_t key, int32_t scancode, int32_t action, int32_t mods) noexcept
			: window{ window }, key{ (KeyCode_)key }, scancode{ scancode }, action{ (InputAction_)action }, mods{ mods } {}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API WindowMaximizeEvent : public Event
	{
		EVENT_COMMON(WindowMaximizeEvent, Event);

	public:
		WindowID window;
		int32_t maximized;

		WindowMaximizeEvent(WindowID window, int32_t maximized) noexcept
			: window{ window }, maximized{ maximized } {}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API WindowMouseButtonEvent : public Event
	{
		EVENT_COMMON(WindowMouseButtonEvent, Event);

	public:
		WindowID window;
		MouseButton_ button;
		InputAction_ action;
		int32_t mods;

		WindowMouseButtonEvent(WindowID window, int32_t button, int32_t action, int32_t mods) noexcept
			: window{ window }, button{ (MouseButton_)button }, action{ (InputAction_)action }, mods{ mods } {}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API WindowMouseEnterEvent : public Event
	{
		EVENT_COMMON(WindowMouseEnterEvent, Event);

	public:
		WindowID window;
		int32_t entered;

		WindowMouseEnterEvent(WindowID window, int32_t entered) noexcept
			: window{ window }, entered{ entered } {}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API WindowMousePositionEvent : public Event
	{
		EVENT_COMMON(WindowMousePositionEvent, Event);

	public:
		WindowID window;
		double_t xpos;
		double_t ypos;

		WindowMousePositionEvent(WindowID window, double_t xpos, double_t ypos) noexcept
			: window{ window }, xpos{ xpos }, ypos{ ypos } {}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API WindowScrollEvent : public Event
	{
		EVENT_COMMON(WindowScrollEvent, Event);

	public:
		WindowID window;
		double_t xoffset;
		double_t yoffset;

		WindowScrollEvent(WindowID window, double_t xoffset, double_t yoffset) noexcept
			: window{ window }, xoffset{ xoffset }, yoffset{ yoffset } {}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API WindowPositionEvent : public Event
	{
		EVENT_COMMON(WindowPositionEvent, Event);

	public:
		WindowID window;
		int32_t xpos;
		int32_t ypos;

		WindowPositionEvent(WindowID window, int32_t xpos, int32_t ypos) noexcept
			: window{ window }, xpos{ xpos }, ypos{ ypos } {}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API WindowRefreshEvent : public Event
	{
		EVENT_COMMON(WindowRefreshEvent, Event);

	public:
		WindowID window;

		WindowRefreshEvent(WindowID window) noexcept
			: window{ window } {}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API WindowSizeEvent : public Event
	{
		EVENT_COMMON(WindowSizeEvent, Event);

	public:
		WindowID window;
		int32_t width;
		int32_t height;

		WindowSizeEvent(WindowID window, int32_t width, int32_t height) noexcept
			: window{ window }, width{ width }, height{ height } {}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_DISPLAY_SERVER_HPP_
