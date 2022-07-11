#ifndef _ISM_DISPLAY_SERVER_HPP_
#define _ISM_DISPLAY_SERVER_HPP_

#include <core/os/os.hpp>
#include <core/input/input.hpp>
#include <core/io/image.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// display server
	class ISM_API DisplayServer : public Object
	{
		OBJECT_COMMON(DisplayServer, Object);

		static DisplayServer * __singleton;

	public:
		DisplayServer();
		virtual ~DisplayServer() override;
		FORCE_INLINE static DisplayServer * get_singleton() noexcept { return __singleton; }

	public:
		// TODO: global menu stuff goes here

		virtual void alert(String const & message, String const & title = "ALERT!") = 0;

		virtual String get_clipboard() const = 0;
		virtual void set_clipboard(String const & text) = 0;

		enum CursorShape_
		{
			CursorShape_Arrow,
			CursorShape_IBeam,
			CursorShape_Crosshair,
			CursorShape_PointingHand,
			CursorShape_EW,
			CursorShape_NS,
			CursorShape_NESW,
			CursorShape_NWSE,
			CursorShape_ResizeAll,
			CursorShape_NotAllowed,
			CursorShape_HResize,
			CursorShape_VResize,
			CursorShape_Hand,
			CursorShape_MAX
		};

		virtual CursorShape_ cursor_get_shape() const = 0;
		virtual void cursor_set_shape(CursorShape_ shape) = 0;
		virtual void cursor_set_custom_image(RES const & cursor, CursorShape_ shape = {}, Vec2 const & hotspot = {}) = 0;

		enum MouseMode_
		{
			MouseMode_Normal,
			MouseMode_Hidden,
			MouseMode_Disabled,
			MouseMode_MAX
		};

		virtual MouseMode_ mouse_get_mode() const = 0;
		virtual void mouse_set_mode(MouseMode_ mode) = 0;

		virtual int32_t mouse_get_button(MouseButton_ button) const = 0;
		virtual Vec2 mouse_get_position() const = 0;
		virtual void mouse_set_position(Vec2 const & position) = 0;

		enum { SCREEN_OF_MAIN_WINDOW = -1 };
		virtual int32_t get_screen_count() const = 0;
		virtual String screen_get_name(int32_t screen = SCREEN_OF_MAIN_WINDOW) const = 0;
		virtual IntRect screen_get_rect(int32_t screen = SCREEN_OF_MAIN_WINDOW) const = 0;
		virtual Vec2i screen_get_position(int32_t screen = SCREEN_OF_MAIN_WINDOW) const = 0;
		virtual Vec2i screen_get_size(int32_t screen = SCREEN_OF_MAIN_WINDOW) const = 0;
		virtual Vec2 screen_get_scale(int32_t screen = SCREEN_OF_MAIN_WINDOW) const = 0;
		virtual Vec2 screen_get_max_scale() const {
			Vec2 scale{ 1.f, 1.f };
			for (int32_t i = 0; i < get_screen_count(); ++i) {
				scale = mpl::max(scale, screen_get_scale(i));
			}
			return scale;
		}

		struct VideoMode
		{
			Vec2i	size			{ 1280, 720 };
			Color32	bits_per_pixel	{ COLOR32(8, 8, 8, 8) };
			int32_t	refresh_rate	{ -1 };
		};

		enum WindowMode_
		{
			WindowMode_Windowed,
			WindowMode_Minimized,
			WindowMode_Maximized,
			WindowMode_Fullscreen,
			WindowMode_MAX
		};

		using WindowID = intptr_t;

		enum { MAIN_WINDOW_ID = 0, INVALID_WINDOW_ID = -1 };

		virtual List<WindowID> get_window_list() const = 0;

		virtual String window_get_title(WindowID window = MAIN_WINDOW_ID) const = 0;
		virtual void window_set_title(String const & title, WindowID window = MAIN_WINDOW_ID) = 0;

		virtual int32_t window_get_current_screen(WindowID window = MAIN_WINDOW_ID) const = 0;
		virtual void window_set_current_screen(int32_t screen, WindowID window = MAIN_WINDOW_ID) = 0;

		virtual Vec2i window_get_position(WindowID window = MAIN_WINDOW_ID) const = 0;
		virtual void window_set_position(Vec2i const & position, WindowID window = MAIN_WINDOW_ID) = 0;

		virtual Vec2i window_get_size(WindowID window = MAIN_WINDOW_ID) const = 0;
		virtual void window_set_size(Vec2i const & size, WindowID window = MAIN_WINDOW_ID) = 0;

		virtual Vec2i window_get_real_size(WindowID window = MAIN_WINDOW_ID) const = 0;

		virtual WindowMode_ window_get_mode(WindowID window = MAIN_WINDOW_ID) const = 0;
		virtual void window_set_mode(WindowMode_ mode, WindowID window = MAIN_WINDOW_ID) = 0;

		virtual bool window_get_flag(int32_t flag, WindowID window = MAIN_WINDOW_ID) const = 0;
		virtual void window_set_flag(int32_t flag, bool enabled, WindowID window = MAIN_WINDOW_ID) = 0;

		virtual void request_window_attention(WindowID window = MAIN_WINDOW_ID) = 0;
		virtual void move_window_to_foreground(WindowID window = MAIN_WINDOW_ID) = 0;

		virtual void window_set_visible(bool enabled, WindowID window = MAIN_WINDOW_ID) = 0;
		virtual bool window_is_visible(WindowID window = MAIN_WINDOW_ID) const = 0;

		virtual Vec2 window_get_scale(WindowID window = MAIN_WINDOW_ID) const = 0;

		virtual void window_grab_focus(WindowID window = MAIN_WINDOW_ID) = 0;
		virtual bool window_has_focus(WindowID window = MAIN_WINDOW_ID) const = 0;

		virtual void * window_get_native_handle(WindowID window = MAIN_WINDOW_ID) const = 0;

		virtual WindowID get_current_context() const = 0;
		virtual void make_context_current(WindowID window) = 0;

		virtual void poll_events() = 0;
		virtual void swap_buffers() = 0;

		virtual void set_native_icon(Path const & path) = 0;
		virtual void set_icon(uint8_t const * data, int32_t width, int32_t height) = 0;

		void vsync_set_enabled(bool enabled);
		bool vsync_is_enabled() const;

	public:
		using CharCallback =				void(*)(WindowID, uint32_t c);
		using CharModsCallback =			void(*)(WindowID, uint32_t c, int32_t mods);
		using CloseCallback =				void(*)(WindowID);
		using ScaleCallback =				void(*)(WindowID, float_t x, float_t y);
		using DropCallback =				void(*)(WindowID, int32_t path_count, cstring paths[]);
		using FocusCallback =				void(*)(WindowID, bool focused);
		using FramebufferResizeCallback =	void(*)(WindowID, int32_t w, int32_t h);
		using IconifyCallback =				void(*)(WindowID, bool iconified);
		using KeyCallback =					void(*)(WindowID, KeyCode_ key, int32_t scancode, int32_t action, int32_t mods);
		using MaximizeCallback =			void(*)(WindowID, bool maximized);
		using MouseButtonCallback =			void(*)(WindowID, MouseButton_ button, int32_t action, int32_t mods);
		using MouseEnterCallback =			void(*)(WindowID, bool entered);
		using MousePositionCallback =		void(*)(WindowID, double_t x, double_t y);
		using PositionCallback =			void(*)(WindowID, int32_t x, int32_t y);
		using RefreshCallback =				void(*)(WindowID);
		using SizeCallback =				void(*)(WindowID, int32_t w, int32_t h);
		using ScrollCallback =				void(*)(WindowID, double_t x, double_t y);

		virtual CharCallback window_set_char_callback(CharCallback callback, WindowID window = MAIN_WINDOW_ID) = 0;
		virtual CharModsCallback window_set_char_mods_callback(CharModsCallback callback, WindowID window = MAIN_WINDOW_ID) = 0;
		virtual CloseCallback window_set_close_callback(CloseCallback callback, WindowID window = MAIN_WINDOW_ID) = 0;
		virtual DropCallback window_set_drop_callback(DropCallback callback, WindowID window = MAIN_WINDOW_ID) = 0;
		virtual FocusCallback window_set_focus_callback(FocusCallback callback, WindowID window = MAIN_WINDOW_ID) = 0;
		virtual FramebufferResizeCallback window_set_framebuffer_resize_callback(FramebufferResizeCallback callback, WindowID window = MAIN_WINDOW_ID) = 0;
		virtual IconifyCallback window_set_iconify_callback(IconifyCallback callback, WindowID window = MAIN_WINDOW_ID) = 0;
		virtual KeyCallback window_set_key_callback(KeyCallback callback, WindowID window = MAIN_WINDOW_ID) = 0;
		virtual MaximizeCallback window_set_maximize_callback(MaximizeCallback callback, WindowID window = MAIN_WINDOW_ID) = 0;
		virtual MouseButtonCallback window_set_mouse_button_callback(MouseButtonCallback callback, WindowID window = MAIN_WINDOW_ID) = 0;
		virtual MouseEnterCallback window_set_mouse_enter_callback(MouseEnterCallback callback, WindowID window = MAIN_WINDOW_ID) = 0;
		virtual MousePositionCallback window_set_mouse_position_callback(MousePositionCallback callback, WindowID window = MAIN_WINDOW_ID) = 0;
		virtual PositionCallback window_set_position_callback(PositionCallback callback, WindowID window = MAIN_WINDOW_ID) = 0;
		virtual RefreshCallback window_set_refresh_callback(RefreshCallback callback, WindowID window = MAIN_WINDOW_ID) = 0;
		virtual ScaleCallback window_set_scale_callback(ScaleCallback callback, WindowID window = MAIN_WINDOW_ID) = 0;
		virtual ScrollCallback window_set_scroll_callback(ScrollCallback callback, WindowID window = MAIN_WINDOW_ID) = 0;
		virtual SizeCallback window_set_size_callback(SizeCallback callback, WindowID window = MAIN_WINDOW_ID) = 0;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ALIAS(DS) DisplayServer;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_DISPLAY_SERVER_HPP_
