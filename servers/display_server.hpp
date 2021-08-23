#ifndef _ISM_DISPLAY_SERVER_HPP_
#define _ISM_DISPLAY_SERVER_HPP_

#include <core/input/input.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class SceneTree;
	class Node;
	class Viewport;
	class Window;

	DECL_HANDLE(MonitorID);

	DECL_HANDLE(WindowID);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ALIAS(window_char_callback)					void(*)(WindowID, uint32_t);
	ALIAS(window_char_mods_callback)			void(*)(WindowID, uint32_t, int32_t);
	ALIAS(window_close_callback)				void(*)(WindowID);
	ALIAS(window_content_scale_callback)		void(*)(WindowID, float_t, float_t);
	ALIAS(window_drop_callback)					void(*)(WindowID, int32_t, cstring[]);
	ALIAS(window_focus_callback)				void(*)(WindowID, int32_t);
	ALIAS(window_framebuffer_resize_callback)	void(*)(WindowID, int32_t, int32_t);
	ALIAS(window_iconify_callback)				void(*)(WindowID, int32_t);
	ALIAS(window_key_callback)					void(*)(WindowID, int32_t, int32_t, int32_t, int32_t);
	ALIAS(window_maximize_callback)				void(*)(WindowID, int32_t);
	ALIAS(window_mouse_button_callback)			void(*)(WindowID, int32_t, int32_t, int32_t);
	ALIAS(window_mouse_enter_callback)			void(*)(WindowID, int32_t);
	ALIAS(window_mouse_pos_callback)			void(*)(WindowID, double_t, double_t);
	ALIAS(window_position_callback)				void(*)(WindowID, int32_t, int32_t);
	ALIAS(window_refresh_callback)				void(*)(WindowID);
	ALIAS(window_resize_callback)				void(*)(WindowID, int32_t, int32_t);
	ALIAS(window_scroll_callback)				void(*)(WindowID, double_t, double_t);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// window hints
	typedef enum WindowHints_ : int32_t
	{
		WindowHints_None			= 0		,		// none
		WindowHints_AutoIconify		= 1 << 0,		// auto iconify
		WindowHints_CenterCursor	= 1 << 1,		// center cursor
		WindowHints_Decorated		= 1 << 2,		// decorated
		WindowHints_Doublebuffer	= 1 << 3,		// doublebuffer
		WindowHints_Floating		= 1 << 4,		// floating
		WindowHints_FocusOnShow		= 1 << 5,		// focus on show
		WindowHints_Focused			= 1 << 6,		// focused
		WindowHints_Maximized		= 1 << 7,		// maximized
		WindowHints_Resizable		= 1 << 8,		// resizable
		WindowHints_Visible			= 1 << 9,		// visible

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
	}
	WindowHints;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// context api
	typedef enum ContextAPI_ : int32_t
	{
		ContextAPI_Unknown		, // unknown
		ContextAPI_OpenGL		, // opengl
		ContextAPI_Vulkan		, // vulkan
		ContextAPI_DirectX		, // directx
	}
	ContextAPI;

	// context profile
	typedef enum ContextProfile_ : int32_t
	{
		ContextProfile_Any		, // any
		ContextProfile_Core		, // core
		ContextProfile_Compat	, // compat
		ContextProfile_Debug	, // debug
	}
	ContextProfile;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// context settings
	struct NODISCARD ContextSettings
	{
		ContextAPI		api				{ ContextAPI_OpenGL };
		int32_t			major			{ 4 };
		int32_t			minor			{ 6 };
		ContextProfile	profile			{ ContextProfile_Compat };
		int32_t			depth_bits		{ 24 };
		int32_t			stencil_bits	{ 8 };
		bool			multisample		{ true };
		bool			srgb_capable	{ false };
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// video mode
	struct NODISCARD VideoMode
	{
		Vec2	size	{ 1280, 720 }	; // resolution
		Vec4	bpp		{ 8, 8, 8, 8 }	; // bits-per-pixel
		int32_t	rate	{ -1 }			; // refresh rate

		DEFAULT_COPYABLE_MOVABLE(VideoMode);
	};

	ISM_API_FUNC(VideoMode const &) get_desktop_video_mode();

	ISM_API_FUNC(Vector<VideoMode> const &) get_fullscreen_video_modes();

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// window settings
	struct NODISCARD WindowSettings
	{
		String			title		{}; // 
		VideoMode		video		{}; // 
		ContextSettings	context		{}; // 
		WindowHints		hints		{}; // 
		MonitorID		monitor		{}; // 
		WindowID		share		{}; // 
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// display server
	class ISM_API DisplayServer : Object
	{
		ISM_OBJECT(DisplayServer, Object);

	protected:
		static DisplayServer * singleton;

		explicit DisplayServer(TYPE const & type) noexcept : Object{ type } { singleton = this; }
		
	public:
		static DisplayServer * get_singleton() { return singleton; }

		virtual Window * create_window(SceneTree * tree, Node * parent, WindowSettings const & settings) = 0;

		virtual bool initialize() = 0;

		virtual void finalize() = 0;
		
		virtual void poll_events() = 0;

		virtual void swap_buffers(WindowID id) = 0;

		virtual WindowID get_current_context() const = 0;

		virtual void make_context_current(WindowID id) = 0;

	public:
		virtual void focus_window(WindowID id) = 0;

		virtual void hide_window(WindowID id) = 0;

		virtual void iconify_window(WindowID id) = 0;

		virtual void maximize_window(WindowID id) = 0;

		virtual void restore_window(WindowID id) = 0;

		virtual void request_window_attention(WindowID id) = 0;

	public:
		NODISCARD virtual String window_get_clipboard(WindowID id) const = 0;

		NODISCARD virtual Vec2 window_get_content_scale(WindowID id) const = 0;

		NODISCARD virtual Vec2 window_get_framebuffer_size(WindowID id) const = 0;
		
		NODISCARD virtual int32_t window_get_input_mode(WindowID id, int32_t value) const = 0;

		NODISCARD virtual int32_t window_get_key(WindowID id, int32_t value) const = 0;

		NODISCARD virtual int32_t window_get_mouse_button(WindowID id, int32_t value) const = 0;

		NODISCARD virtual Vec2 window_get_mouse_pos(WindowID id) const = 0;

		NODISCARD virtual WindowID window_get_native_handle(WindowID id) const = 0;

		NODISCARD virtual float_t window_get_opacity(WindowID id) const = 0;

		NODISCARD virtual Vec2 window_get_position(WindowID id) const = 0;

		NODISCARD virtual Vec2 window_get_size(WindowID id) const = 0;

		NODISCARD virtual void * window_get_user_pointer(WindowID id) const = 0;

		NODISCARD virtual Rect window_get_frame_size(WindowID id) const = 0;

	public:
		NODISCARD virtual bool window_is_auto_iconify(WindowID id) const = 0;

		NODISCARD virtual bool window_is_decorated(WindowID id) const = 0;

		NODISCARD virtual bool window_is_floating(WindowID id) const = 0;

		NODISCARD virtual bool window_is_focused(WindowID id) const = 0;

		NODISCARD virtual bool window_is_focus_on_show(WindowID id) const = 0;

		NODISCARD virtual bool window_is_hovered(WindowID id) const = 0;

		NODISCARD virtual bool window_is_iconified(WindowID id) const = 0;

		NODISCARD virtual bool window_is_maximized(WindowID id) const = 0;

		NODISCARD virtual bool window_is_open(WindowID id) const = 0;

		NODISCARD virtual bool window_is_resizable(WindowID id) const = 0;

		NODISCARD virtual bool window_is_transparent(WindowID id) const = 0;

		NODISCARD virtual bool window_is_visible(WindowID id) const = 0;

	public:
		virtual void window_set_auto_iconify(WindowID id, bool value) = 0;

		virtual void window_set_clipboard(WindowID id, String const & value) = 0;

		virtual void window_set_cursor(WindowID id, CursorID value) = 0;

		virtual void window_set_cursor_mode(WindowID id, int32_t value) = 0;

		virtual void window_set_mouse_pos(WindowID id, Vec2 const & value) = 0;

		virtual void window_set_decorated(WindowID id, bool value) = 0;

		virtual void window_set_floating(WindowID id, bool value) = 0;

		virtual void window_set_focus_on_show(WindowID id, bool value) = 0;

		virtual void window_set_icons(WindowID id, int32_t width, int32_t height, uint8_t * pixels, int32_t count = 1) = 0;

		virtual void window_set_input_mode(WindowID id, int32_t mode, int32_t value) = 0;

		virtual void window_set_opacity(WindowID id, float_t value) = 0;

		virtual void window_set_position(WindowID id, Vec2 const & value) = 0;

		virtual void window_set_monitor(WindowID id, MonitorID monitor, Rect const & bounds) = 0;

		virtual void window_set_resizable(WindowID id, bool value) = 0;

		virtual void window_set_should_close(WindowID id, bool value) = 0;

		virtual void window_set_size(WindowID id, Vec2 const & value) = 0;

		virtual void window_set_title(WindowID id, String const & value) = 0;

		virtual void window_set_user_pointer(WindowID id, void * value) = 0;

	public:
		NODISCARD virtual window_char_callback window_get_char_callback(WindowID id) const = 0;

		NODISCARD virtual window_char_mods_callback window_get_char_mods_callback(WindowID id) const = 0;

		NODISCARD virtual window_close_callback window_get_close_callback(WindowID id) const = 0;

		NODISCARD virtual window_content_scale_callback window_get_content_scale_callback(WindowID id) const = 0;

		NODISCARD virtual window_drop_callback window_get_drop_callback(WindowID id) const = 0;

		NODISCARD virtual window_focus_callback window_get_focus_callback(WindowID id) const = 0;

		NODISCARD virtual window_framebuffer_resize_callback window_get_framebuffer_resize_callback(WindowID id) const = 0;

		NODISCARD virtual window_iconify_callback window_get_iconify_callback(WindowID id) const = 0;

		NODISCARD virtual window_key_callback window_get_key_callback(WindowID id) const = 0;

		NODISCARD virtual window_maximize_callback window_get_maximize_callback(WindowID id) const = 0;

		NODISCARD virtual window_mouse_button_callback window_get_mouse_button_callback(WindowID id) const = 0;

		NODISCARD virtual window_mouse_enter_callback window_get_mouse_enter_callback(WindowID id) const = 0;

		NODISCARD virtual window_mouse_pos_callback window_get_mouse_pos_callback(WindowID id) const = 0;

		NODISCARD virtual window_position_callback window_get_position_callback(WindowID id) const = 0;

		NODISCARD virtual window_refresh_callback window_get_refresh_callback(WindowID id) const = 0;

		NODISCARD virtual window_resize_callback window_get_resize_callback(WindowID id) const = 0;

		NODISCARD virtual window_scroll_callback window_get_scroll_callback(WindowID id) const = 0;

	public:
		virtual window_char_callback window_set_char_callback(WindowID id, window_char_callback value) = 0;

		virtual window_char_mods_callback window_set_char_mods_callback(WindowID id, window_char_mods_callback value) = 0;

		virtual window_close_callback window_set_close_callback(WindowID id, window_close_callback value) = 0;

		virtual window_content_scale_callback window_set_content_scale_callback(WindowID id, window_content_scale_callback value) = 0;

		virtual window_drop_callback window_set_drop_callback(WindowID id, window_drop_callback value) = 0;

		virtual window_focus_callback window_set_focus_callback(WindowID id, window_focus_callback value) = 0;

		virtual window_framebuffer_resize_callback window_set_framebuffer_resize_callback(WindowID id, window_framebuffer_resize_callback value) = 0;

		virtual window_iconify_callback window_set_iconify_callback(WindowID id, window_iconify_callback value) = 0;

		virtual window_key_callback window_set_key_callback(WindowID id, window_key_callback value) = 0;

		virtual window_maximize_callback window_set_maximize_callback(WindowID id, window_maximize_callback value) = 0;

		virtual window_mouse_button_callback window_set_mouse_button_callback(WindowID id, window_mouse_button_callback value) = 0;

		virtual window_mouse_enter_callback window_set_mouse_enter_callback(WindowID id, window_mouse_enter_callback value) = 0;

		virtual window_mouse_pos_callback window_set_mouse_pos_callback(WindowID id, window_mouse_pos_callback value) = 0;

		virtual window_position_callback window_set_position_callback(WindowID id, window_position_callback value) = 0;

		virtual window_refresh_callback window_set_refresh_callback(WindowID id, window_refresh_callback value) = 0;

		virtual window_resize_callback window_set_resize_callback(WindowID id, window_resize_callback value) = 0;

		virtual window_scroll_callback window_set_scroll_callback(WindowID id, window_scroll_callback value) = 0;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	inline DisplayServer * get_display_server() noexcept { return DisplayServer::get_singleton(); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_DISPLAY_SERVER_HPP_
