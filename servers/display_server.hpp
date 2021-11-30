#ifndef _ISM_DISPLAY_SERVER_HPP_
#define _ISM_DISPLAY_SERVER_HPP_

#include <core/os/os.hpp>
#include <core/input/input.hpp>

// display api
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class Viewport;
	class SubViewport;
	class Window;

	OPAQUE_TYPE(MonitorID);
	OPAQUE_TYPE(WindowID);

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

	// window hints
	ENUM_INT(WindowHints)
	{
		WindowHints_None			= 0		, // none
		WindowHints_AutoIconify		= 1 << 0, // auto iconify
		WindowHints_CenterCursor	= 1 << 1, // center cursor
		WindowHints_Decorated		= 1 << 2, // decorated
		WindowHints_Doublebuffer	= 1 << 3, // doublebuffer
		WindowHints_Floating		= 1 << 4, // floating
		WindowHints_FocusOnShow		= 1 << 5, // focus on show
		WindowHints_Focused			= 1 << 6, // focused
		WindowHints_Maximized		= 1 << 7, // maximized
		WindowHints_Resizable		= 1 << 8, // resizable
		WindowHints_Visible			= 1 << 9, // visible

		WindowHints_Default
			= WindowHints_AutoIconify
			| WindowHints_Decorated
			| WindowHints_Doublebuffer
			| WindowHints_FocusOnShow
			| WindowHints_Focused
			| WindowHints_Resizable
			| WindowHints_Visible,

		WindowHints_Default_Maximized
			= WindowHints_AutoIconify
			| WindowHints_Decorated
			| WindowHints_Doublebuffer
			| WindowHints_FocusOnShow
			| WindowHints_Focused
			| WindowHints_Maximized
			| WindowHints_Resizable,
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// renderer api
	ENUM_INT(RendererAPI)
	{
		RendererAPI_Unknown		, // unknown
		RendererAPI_OpenGL		, // opengl
		RendererAPI_Vulkan		, // vulkan
		RendererAPI_DirectX		, // directx
	};

	// renderer profile
	ENUM_INT(RendererProfile)
	{
		RendererProfile_Any		, // any
		RendererProfile_Core	, // core
		RendererProfile_Compat	, // compat
		RendererProfile_Debug	, // debug
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// video mode
	struct NODISCARD VideoMode
	{
		DEFAULT_COPYABLE_MOVABLE(VideoMode);

		Vec2	size			{ 1280, 720 };
		Vec4	bits_per_pixel	{ 8, 8, 8, 8 };
		int32_t	refresh_rate	{ -1 };

		NODISCARD bool operator==(VideoMode const & other) const noexcept { return compare(other) == 0; }
		NODISCARD bool operator!=(VideoMode const & other) const noexcept { return compare(other) != 0; }
		NODISCARD bool operator< (VideoMode const & other) const noexcept { return compare(other) < 0; }
		NODISCARD bool operator> (VideoMode const & other) const noexcept { return compare(other) > 0; }
		NODISCARD bool operator<=(VideoMode const & other) const noexcept { return compare(other) <= 0; }
		NODISCARD bool operator>=(VideoMode const & other) const noexcept { return compare(other) >= 0; }

		NODISCARD int32_t compare(VideoMode const & other) const noexcept
		{
			if (this == std::addressof(other)) { return 0; }
			if (auto cmp{ CMP(size, other.size) }; cmp != 0) { return cmp; }
			if (auto cmp{ CMP(bits_per_pixel, other.bits_per_pixel) }; cmp != 0) { return cmp; }
			if (auto cmp{ CMP(refresh_rate, other.refresh_rate) }; cmp != 0) { return cmp; }
			return 0;
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// context settings
	struct NODISCARD ContextSettings
	{
		DEFAULT_COPYABLE_MOVABLE(ContextSettings);

		RendererAPI		api				{ RendererAPI_OpenGL };
		int32_t			major			{ 4 };
		int32_t			minor			{ 6 };
		RendererProfile	profile			{ RendererProfile_Compat };
		int32_t			depth_bits		{ 24 };
		int32_t			stencil_bits	{ 8 };
		bool			multisample		{ true };
		bool			srgb_capable	{ false };
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// window settings
	struct NODISCARD DisplayServerSettings
	{
		DEFAULT_COPYABLE_MOVABLE(DisplayServerSettings);

		String			title	{ "New Window" };
		VideoMode		video	{};
		ContextSettings	context	{};
		WindowHints		hints	{ WindowHints_Default };
		MonitorID		monitor	{};
		WindowID		share	{};
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// display events
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API WindowCharEvent : public EventClass<WindowCharEvent>
	{
		OBJECT_COMMON(WindowCharEvent, Event);

	public:
		WindowCharEvent(...) noexcept {}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API WindowCharModsEvent : public EventClass<WindowCharModsEvent>
	{
		OBJECT_COMMON(WindowCharModsEvent, Event);

	public:
		WindowCharModsEvent(...) noexcept {}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API WindowCloseEvent : public EventClass<WindowCloseEvent>
	{
		OBJECT_COMMON(WindowCloseEvent, Event);

	public:
		WindowCloseEvent(...) noexcept {}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API WindowContentScaleEvent : public EventClass<WindowContentScaleEvent>
	{
		OBJECT_COMMON(WindowContentScaleEvent, Event);

	public:
		WindowContentScaleEvent(...) noexcept {}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API WindowDropEvent : public EventClass<WindowDropEvent>
	{
		OBJECT_COMMON(WindowDropEvent, Event);

	public:
		WindowDropEvent(...) noexcept {}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API WindowFocusEvent : public EventClass<WindowFocusEvent>
	{
		OBJECT_COMMON(WindowFocusEvent, Event);

	public:
		WindowFocusEvent(...) noexcept {}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API WindowFramebufferResizeEvent : public EventClass<WindowFramebufferResizeEvent>
	{
		OBJECT_COMMON(WindowFramebufferResizeEvent, Event);

	public:
		WindowFramebufferResizeEvent(...) noexcept {}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API WindowIconifyEvent : public EventClass<WindowIconifyEvent>
	{
		OBJECT_COMMON(WindowIconifyEvent, Event);

	public:
		WindowIconifyEvent(...) noexcept {}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API WindowKeyEvent : public EventClass<WindowKeyEvent>
	{
		OBJECT_COMMON(WindowKeyEvent, Event);

	public:
		WindowKeyEvent(...) noexcept {}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API WindowMaximizeEvent : public EventClass<WindowMaximizeEvent>
	{
		OBJECT_COMMON(WindowMaximizeEvent, Event);

	public:
		WindowMaximizeEvent(...) noexcept {}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API WindowMouseButtonEvent : public EventClass<WindowMouseButtonEvent>
	{
		OBJECT_COMMON(WindowMouseButtonEvent, Event);

	public:
		WindowMouseButtonEvent(...) noexcept {}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API WindowMouseEnterEvent : public EventClass<WindowMouseEnterEvent>
	{
		OBJECT_COMMON(WindowMouseEnterEvent, Event);

	public:
		WindowMouseEnterEvent(...) noexcept {}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API WindowMouseExitEvent : public EventClass<WindowMouseExitEvent>
	{
		OBJECT_COMMON(WindowMouseExitEvent, Event);

	public:
		WindowMouseExitEvent(...) noexcept {}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API WindowMousePositionEvent : public EventClass<WindowMousePositionEvent>
	{
		OBJECT_COMMON(WindowMousePositionEvent, Event);

	public:
		WindowMousePositionEvent(...) noexcept {}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API WindowScrollEvent : public EventClass<WindowScrollEvent>
	{
		OBJECT_COMMON(WindowScrollEvent, Event);

	public:
		WindowScrollEvent(...) noexcept {}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API WindowPositionEvent : public EventClass<WindowPositionEvent>
	{
		OBJECT_COMMON(WindowPositionEvent, Event);

	public:
		WindowPositionEvent(...) noexcept {}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API WindowRefreshEvent : public EventClass<WindowRefreshEvent>
	{
		OBJECT_COMMON(WindowRefreshEvent, Event);

	public:
		WindowRefreshEvent(...) noexcept {}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API WindowSizeEvent : public EventClass<WindowSizeEvent>
	{
		OBJECT_COMMON(WindowSizeEvent, Event);

	public:
		WindowSizeEvent(...) noexcept {}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// display server
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API DisplayServer : public Object
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		OBJECT_COMMON(DisplayServer, Object);

		static DisplayServer * singleton;

	protected:
		explicit DisplayServer() noexcept : Object{} { singleton = this; }
		
	public:
		virtual ~DisplayServer() override {}

		NODISCARD static DisplayServer * get_singleton() noexcept { return singleton; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		NODISCARD virtual WindowID get_context_main() const;
		NODISCARD virtual WindowID get_context_current() const;
		virtual void make_context_current(WindowID id);
		virtual void poll_events();
		virtual void swap_buffers(WindowID id);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual void focus_window(WindowID id);
		virtual void hide_window(WindowID id);
		virtual void iconify_window(WindowID id);
		virtual void maximize_window(WindowID id);
		virtual void restore_window(WindowID id);
		virtual void request_window_attention(WindowID id);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		NODISCARD virtual Rect window_get_bounds(WindowID id) const { return Rect{ window_get_position(id), window_get_size(id) }; }
		NODISCARD virtual String window_get_clipboard(WindowID id) const;
		NODISCARD virtual Vec2 window_get_content_scale(WindowID id) const;
		NODISCARD virtual Vec2 window_get_framebuffer_size(WindowID id) const;
		NODISCARD virtual int32_t window_get_input_mode(WindowID id, int32_t value) const;
		NODISCARD virtual int32_t window_get_key(WindowID id, int32_t value) const;
		NODISCARD virtual int32_t window_get_mouse_button(WindowID id, int32_t value) const;
		NODISCARD virtual Vec2 window_get_mouse_pos(WindowID id) const;
		NODISCARD virtual WindowID window_get_native_handle(WindowID id) const;
		NODISCARD virtual float_t window_get_opacity(WindowID id) const;
		NODISCARD virtual Vec2 window_get_position(WindowID id) const;
		NODISCARD virtual Vec2 window_get_size(WindowID id) const;
		NODISCARD virtual void * window_get_user_pointer(WindowID id) const;
		NODISCARD virtual Rect window_get_frame_size(WindowID id) const;
		NODISCARD virtual bool window_get_is_auto_iconify(WindowID id) const;
		NODISCARD virtual bool window_get_is_decorated(WindowID id) const;
		NODISCARD virtual bool window_get_is_floating(WindowID id) const;
		NODISCARD virtual bool window_get_is_focused(WindowID id) const;
		NODISCARD virtual bool window_get_is_focus_on_show(WindowID id) const;
		NODISCARD virtual bool window_get_is_hovered(WindowID id) const;
		NODISCARD virtual bool window_get_is_iconified(WindowID id) const;
		NODISCARD virtual bool window_get_is_maximized(WindowID id) const;
		NODISCARD virtual bool window_get_is_resizable(WindowID id) const;
		NODISCARD virtual bool window_get_is_transparent(WindowID id) const;
		NODISCARD virtual bool window_get_is_visible(WindowID id) const;
		NODISCARD virtual bool window_get_should_close(WindowID id) const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual void window_set_is_auto_iconify(WindowID id, bool value);
		virtual void window_set_clipboard(WindowID id, String const & value);
		virtual void window_set_cursor(WindowID id, CursorID value);
		virtual void window_set_cursor_mode(WindowID id, int32_t value);
		virtual void window_set_mouse_pos(WindowID id, Vec2 const & value);
		virtual void window_set_is_decorated(WindowID id, bool value);
		virtual void window_set_is_floating(WindowID id, bool value);
		virtual void window_set_is_focus_on_show(WindowID id, bool value);
		virtual void window_set_icons(WindowID id, int32_t width, int32_t height, uint8_t * pixels, int32_t count = 1);
		virtual void window_set_input_mode(WindowID id, int32_t mode, int32_t value);
		virtual void window_set_opacity(WindowID id, float_t value);
		virtual void window_set_position(WindowID id, Vec2 const & value);
		virtual void window_set_monitor(WindowID id, MonitorID monitor, Rect const & bounds);
		virtual void window_set_is_resizable(WindowID id, bool value);
		virtual void window_set_should_close(WindowID id, bool value);
		virtual void window_set_size(WindowID id, Vec2 const & value);
		virtual void window_set_title(WindowID id, String const & value);
		virtual void window_set_user_pointer(WindowID id, void * value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual WindowCharCallback window_set_char_callback(WindowID id, WindowCharCallback value);
		virtual WindowCharModsCallback window_set_char_mods_callback(WindowID id, WindowCharModsCallback value);
		virtual WindowCloseCallback window_set_close_callback(WindowID id, WindowCloseCallback value);
		virtual WindowContentCallback window_set_content_scale_callback(WindowID id, WindowContentCallback value);
		virtual WindowDropCallback window_set_drop_callback(WindowID id, WindowDropCallback value);
		virtual WindowFocusCallback window_set_focus_callback(WindowID id, WindowFocusCallback value);
		virtual WindowFramebufferResizeCallback window_set_framebuffer_resize_callback(WindowID id, WindowFramebufferResizeCallback value);
		virtual WindowIconifyCallback window_set_iconify_callback(WindowID id, WindowIconifyCallback value);
		virtual WindowKeyCallback window_set_key_callback(WindowID id, WindowKeyCallback value);
		virtual WindowMaximizeCallback window_set_maximize_callback(WindowID id, WindowMaximizeCallback value);
		virtual WindowMouseButtonCallback window_set_mouse_button_callback(WindowID id, WindowMouseButtonCallback value);
		virtual WindowMouseEnterCallback window_set_mouse_enter_callback(WindowID id, WindowMouseEnterCallback value);
		virtual WindowMousePositionCallback window_set_mouse_position_callback(WindowID id, WindowMousePositionCallback value);
		virtual WindowPositionCallback window_set_position_callback(WindowID id, WindowPositionCallback value);
		virtual WindowRefreshCallback window_set_refresh_callback(WindowID id, WindowRefreshCallback value);
		virtual WindowScrollCallback window_set_scroll_callback(WindowID id, WindowScrollCallback value);
		virtual WindowSizeCallback window_set_size_callback(WindowID id, WindowSizeCallback value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_DISPLAY_SERVER_HPP_
