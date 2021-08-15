#ifndef _ISM_WINDOW_HPP_
#define _ISM_WINDOW_HPP_

#include <scene/main/viewport.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	DECL_HANDLE(CursorID);
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

	// cursor mode
	typedef enum CursorMode_ : int32_t
	{
		CursorMode_Normal		, // normal
		CursorMode_Hidden		, // hidden
		CursorMode_Disabled		, // disabled
	}
	CursorMode;

	// cursor shape
	typedef enum CursorShape_ : int32_t
	{
		CursorShape_Arrow			, // arrow
		CursorShape_IBeam			, // ibeam
		CursorShape_Crosshair		, // crosshair
		CursorShape_PointingHand	, // pointing_hand
		CursorShape_EW				, // ew
		CursorShape_NS				, // ns
		CursorShape_NESW			, // nesw
		CursorShape_NWSE			, // nwse
		CursorShape_ResizeAll		, // resize_all
		CursorShape_NotAllowed		, // not_allowed
		CursorShape_HResize			, // hresize
		CursorShape_VResize			, // vresize
		CursorShape_Hand			, // hand
	}
	CursorShape;

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

		DEFAULT_COPYABLE_MOVABLE(ContextSettings);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// window settings
	struct NODISCARD WindowSettings
	{
		SceneTree *		tree		{}; // 
		Node *			parent		{}; // 
		String			title		{}; // 
		VideoMode		video		{}; // 
		ContextSettings	context		{}; // 
		WindowHints		hints		{}; // 
		MonitorID		monitor		{}; // 
		WindowID		share		{}; // 

		DEFAULT_COPYABLE_MOVABLE(WindowSettings);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// window
	class ISM_API Window : public Viewport
	{
		ISM_SUPER(Window, Viewport);

	protected:
		explicit Window(SceneTree * tree, Node * parent) : Viewport{ tree, parent } {}

		explicit Window(SceneTree * tree) : Viewport{ tree } {}

		explicit Window(Node * parent) : Viewport{ parent } {}

	public:
		virtual ~Window() override;

		static Window * new_(WindowSettings const & settings);

	public:
		virtual void focus() = 0;

		virtual void hide() = 0;

		virtual void iconify() = 0;

		virtual void maximize() = 0;

		virtual void restore() = 0;

		virtual void request_attention() = 0;

	public:
		NODISCARD inline bool has_hints(int32_t value) const noexcept
		{
			return flag_read((int32_t)get_hints(), value);
		}

		NODISCARD virtual String get_clipboard() const = 0;

		NODISCARD virtual Vec2 get_content_scale() const = 0;

		NODISCARD virtual Vec2 get_framebuffer_size() const override = 0;

		NODISCARD virtual WindowID get_handle() const = 0;

		NODISCARD virtual WindowHints get_hints() const = 0;

		NODISCARD virtual int32_t get_input_mode(int32_t value) const = 0;

		NODISCARD virtual int32_t get_key(int32_t value) const = 0;

		NODISCARD virtual int32_t get_mouse_button(int32_t value) const = 0;

		NODISCARD virtual Vec2 get_mouse_pos() const = 0;

		NODISCARD virtual WindowID get_native_handle() const = 0;

		NODISCARD virtual float_t get_opacity() const = 0;

		NODISCARD virtual Vec2 get_position() const override = 0;

		NODISCARD virtual Vec2 get_size() const override = 0;

		NODISCARD virtual String const & get_title() const = 0;

		NODISCARD virtual void * get_user_pointer() const = 0;

		NODISCARD virtual Rect get_window_frame_size() const = 0;

	public:
		NODISCARD virtual bool is_auto_iconify() const = 0;

		NODISCARD virtual bool is_decorated() const = 0;

		NODISCARD virtual bool is_floating() const = 0;

		NODISCARD virtual bool is_focused() const = 0;

		NODISCARD virtual bool is_focus_on_show() const = 0;

		NODISCARD virtual bool is_hovered() const = 0;

		NODISCARD virtual bool is_iconified() const = 0;

		NODISCARD virtual bool is_maximized() const = 0;

		NODISCARD virtual bool is_open() const = 0;

		NODISCARD virtual bool is_resizable() const = 0;

		NODISCARD virtual bool is_transparent() const = 0;

		NODISCARD virtual bool is_visible() const = 0;

	public:
		virtual void set_auto_iconify(bool value) = 0;

		virtual void set_clipboard(String const & value) = 0;

		virtual void set_cursor(CursorID value) = 0;

		virtual void set_cursor_mode(int32_t value) = 0;

		virtual void set_mouse_pos(Vec2 const & value) = 0;

		virtual void set_decorated(bool value) = 0;

		virtual void set_floating(bool value) = 0;

		virtual void set_focus_on_show(bool value) = 0;

		virtual void set_icons(int32_t count, size_t width, size_t height, byte const * pixels) = 0;

		virtual void set_input_mode(int32_t mode, int32_t value) = 0;

		virtual void set_opacity(float_t value) = 0;

		virtual void set_position(Vec2 const & value) = 0;

		virtual void set_monitor(MonitorID monitor, Rect const & bounds) = 0;

		virtual void set_resizable(bool value) = 0;

		virtual void set_should_close(bool value) = 0;

		virtual void set_size(Vec2 const & value) = 0;

		virtual void set_title(String const & value) = 0;

		virtual void set_user_pointer(void * value) = 0;

	public:
		NODISCARD virtual window_char_callback get_char_callback() const = 0;
		
		NODISCARD virtual window_char_mods_callback get_char_mods_callback() const = 0;
		
		NODISCARD virtual window_close_callback get_close_callback() const = 0;
		
		NODISCARD virtual window_content_scale_callback get_content_scale_callback() const = 0;
		
		NODISCARD virtual window_mouse_enter_callback get_mouse_enter_callback() const = 0;
		
		NODISCARD virtual window_mouse_pos_callback get_mouse_pos_callback() const = 0;
		
		NODISCARD virtual window_drop_callback get_drop_callback() const = 0;
		
		NODISCARD virtual window_focus_callback get_focus_callback() const = 0;
		
		NODISCARD virtual window_framebuffer_resize_callback get_framebuffer_resize_callback() const = 0;
		
		NODISCARD virtual window_iconify_callback get_iconify_callback() const = 0;
		
		NODISCARD virtual window_key_callback get_key_callback() const = 0;
		
		NODISCARD virtual window_maximize_callback get_maximize_callback() const = 0;
		
		NODISCARD virtual window_mouse_button_callback get_mouse_button_callback() const = 0;
		
		NODISCARD virtual window_position_callback get_position_callback() const = 0;
		
		NODISCARD virtual window_refresh_callback get_refresh_callback() const = 0;
		
		NODISCARD virtual window_resize_callback get_resize_callback() const = 0;
		
		NODISCARD virtual window_scroll_callback get_scroll_callback() const = 0;

	public:
		virtual window_char_callback set_char_callback(window_char_callback value) = 0;
		
		virtual window_char_mods_callback set_char_mods_callback(window_char_mods_callback value) = 0;
		
		virtual window_close_callback set_close_callback(window_close_callback value) = 0;
		
		virtual window_content_scale_callback set_content_scale_callback(window_content_scale_callback value) = 0;
		
		virtual window_mouse_enter_callback set_mouse_enter_callback(window_mouse_enter_callback value) = 0;
		
		virtual window_mouse_pos_callback set_mouse_pos_callback(window_mouse_pos_callback value) = 0;
		
		virtual window_drop_callback set_drop_callback(window_drop_callback value) = 0;
		
		virtual window_focus_callback set_focus_callback(window_focus_callback value) = 0;
		
		virtual window_framebuffer_resize_callback set_framebuffer_resize_callback(window_framebuffer_resize_callback value) = 0;
		
		virtual window_iconify_callback set_iconify_callback(window_iconify_callback value) = 0;
		
		virtual window_key_callback set_key_callback(window_key_callback value) = 0;
		
		virtual window_maximize_callback set_maximize_callback(window_maximize_callback value) = 0;
		
		virtual window_mouse_button_callback set_mouse_button_callback(window_mouse_button_callback value) = 0;
		
		virtual window_position_callback set_position_callback(window_position_callback value) = 0;
		
		virtual window_refresh_callback set_refresh_callback(window_refresh_callback value) = 0;
		
		virtual window_resize_callback set_resize_callback(window_resize_callback value) = 0;
		
		virtual window_scroll_callback set_scroll_callback(window_scroll_callback value) = 0;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_WINDOW_HPP_
