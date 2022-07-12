#ifndef _ISM_DISPLAY_SERVER_WINDOWS_HPP_
#define _ISM_DISPLAY_SERVER_WINDOWS_HPP_

#include <servers/display_server.hpp>

struct GLFWwindow;

namespace ism
{
	// windows display server
	class ISM_API DisplayServerWindows : public DisplayServer
	{
		OBJECT_COMMON(DisplayServerWindows, DisplayServer);

		struct _Window
		{
			GLFWwindow *	handle{};
			String			title{};
			i32			current_screen{};
			WindowMode_		window_mode{};
		};

		_Window * m_main_window{};

		Map<WindowID, _Window> m_windows{};

	public:
		DisplayServerWindows(String const & title, WindowMode_ mode, Vec2i const & size);
		~DisplayServerWindows();

	public:
		void get_desktop_video_mode(VideoMode & out) const;
		void get_fullscreen_video_modes(Vector<VideoMode> & out) const;

	public:
		// TODO: global menu stuff goes here

		virtual void alert(String const & message, String const & title = "ALERT!") override;

		virtual String get_clipboard() const override;
		virtual void set_clipboard(String const & text) override;

		virtual Input::Cursor_ cursor_get_shape() const override;
		virtual void cursor_set_shape(Input::Cursor_ shape) override;
		virtual void cursor_set_custom_image(RES const & cursor, Input::Cursor_ shape = {}, Vec2 const & hotspot = {}) override;

		virtual Input::MouseMode_ mouse_get_mode() const override;
		virtual void mouse_set_mode(Input::MouseMode_ mode) override;

		virtual i32 mouse_get_button(Input::MouseButton_ button) const override;
		virtual Vec2 mouse_get_position() const override;
		virtual void mouse_set_position(Vec2 const & position) override;

		virtual bool joystick_is_present(i32 device) const override;
		virtual String joystick_get_name(i32 device) const override;
		virtual String joystick_get_guid(i32 device) const override;
		virtual View<f32 const> joystick_get_axes(i32 device) const override;
		virtual View<byte const> joystick_get_buttons(i32 device) const override;
		virtual View<byte const> joystick_get_hats(i32 device) const override;
		virtual bool joystick_is_gamepad(i32 device) const override;
		virtual String gamepad_get_name(i32 device) const override;
		virtual bool gamepad_update_mappings(String const & mappings) override;

		virtual i32 get_screen_count() const override;
		virtual String screen_get_name(i32 screen = SCREEN_OF_MAIN_WINDOW) const override;
		virtual Vec2i screen_get_physical_size(i32 screen = SCREEN_OF_MAIN_WINDOW) const override;
		virtual IntRect screen_get_workarea(i32 screen = SCREEN_OF_MAIN_WINDOW) const override;
		virtual Vec2i screen_get_position(i32 screen = SCREEN_OF_MAIN_WINDOW) const override;
		virtual Vec2i screen_get_size(i32 screen = SCREEN_OF_MAIN_WINDOW) const override;
		virtual Vec2 screen_get_scale(i32 screen = SCREEN_OF_MAIN_WINDOW) const override;

		virtual Vector<WindowID> get_window_list() const override;

		virtual String window_get_title(WindowID window = MAIN_WINDOW_ID) const override;
		virtual void window_set_title(String const & title, WindowID window = MAIN_WINDOW_ID) override;

		virtual i32 window_get_current_screen(WindowID window = MAIN_WINDOW_ID) const override;
		virtual void window_set_current_screen(i32 screen, WindowID window = MAIN_WINDOW_ID) override;

		virtual Vec2i window_get_position(WindowID window = MAIN_WINDOW_ID) const override;
		virtual void window_set_position(Vec2i const & position, WindowID window = MAIN_WINDOW_ID) override;

		virtual Vec2i window_get_size(WindowID window = MAIN_WINDOW_ID) const override;
		virtual void window_set_size(Vec2i const & size, WindowID window = MAIN_WINDOW_ID) override;

		virtual Vec2i window_get_real_size(WindowID window = MAIN_WINDOW_ID) const override;

		virtual WindowMode_ window_get_mode(WindowID window = MAIN_WINDOW_ID) const override;
		virtual void window_set_mode(WindowMode_ mode, WindowID window = MAIN_WINDOW_ID) override;

		virtual bool window_get_flag(i32 flag, WindowID window = MAIN_WINDOW_ID) const override;
		virtual void window_set_flag(i32 flag, bool enabled, WindowID window = MAIN_WINDOW_ID) override;

		virtual void request_window_attention(WindowID window = MAIN_WINDOW_ID) override;
		virtual void move_window_to_foreground(WindowID window = MAIN_WINDOW_ID) override;

		virtual void window_set_visible(bool enabled, WindowID window = MAIN_WINDOW_ID) override;
		virtual bool window_is_visible(WindowID window = MAIN_WINDOW_ID) const override;

		virtual Vec2 window_get_scale(WindowID window = MAIN_WINDOW_ID) const override;

		virtual bool window_has_focus(WindowID window = MAIN_WINDOW_ID) const override;
		virtual void window_grab_focus(WindowID window = MAIN_WINDOW_ID) override;

		virtual void poll_events() override;
		virtual void swap_buffers() override;

		virtual void set_native_icon(Path const & path) override;
		virtual void set_icon(byte const * data, i32 width, i32 height) override;

	public:
		virtual CharCallback window_set_char_callback(CharCallback callback, WindowID window = MAIN_WINDOW_ID) override;
		virtual CharModsCallback window_set_char_mods_callback(CharModsCallback callback, WindowID window = MAIN_WINDOW_ID) override;
		virtual CloseCallback window_set_close_callback(CloseCallback callback, WindowID window = MAIN_WINDOW_ID) override;
		virtual DropCallback window_set_drop_callback(DropCallback callback, WindowID window = MAIN_WINDOW_ID) override;
		virtual FocusCallback window_set_focus_callback(FocusCallback callback, WindowID window = MAIN_WINDOW_ID) override;
		virtual FramebufferResizeCallback window_set_framebuffer_resize_callback(FramebufferResizeCallback callback, WindowID window = MAIN_WINDOW_ID) override;
		virtual IconifyCallback window_set_iconify_callback(IconifyCallback callback, WindowID window = MAIN_WINDOW_ID) override;
		virtual KeyCallback window_set_key_callback(KeyCallback callback, WindowID window = MAIN_WINDOW_ID) override;
		virtual MaximizeCallback window_set_maximize_callback(MaximizeCallback callback, WindowID window = MAIN_WINDOW_ID) override;
		virtual MouseButtonCallback window_set_mouse_button_callback(MouseButtonCallback callback, WindowID window = MAIN_WINDOW_ID) override;
		virtual MouseEnterCallback window_set_mouse_enter_callback(MouseEnterCallback callback, WindowID window = MAIN_WINDOW_ID) override;
		virtual MousePositionCallback window_set_mouse_position_callback(MousePositionCallback callback, WindowID window = MAIN_WINDOW_ID) override;
		virtual PositionCallback window_set_position_callback(PositionCallback callback, WindowID window = MAIN_WINDOW_ID) override;
		virtual RefreshCallback window_set_refresh_callback(RefreshCallback callback, WindowID window = MAIN_WINDOW_ID) override;
		virtual ScaleCallback window_set_scale_callback(ScaleCallback callback, WindowID window = MAIN_WINDOW_ID) override;
		virtual ScrollCallback window_set_scroll_callback(ScrollCallback callback, WindowID window = MAIN_WINDOW_ID) override;
		virtual SizeCallback window_set_size_callback(SizeCallback callback, WindowID window = MAIN_WINDOW_ID) override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ISM_DISPLAY_SERVER_WINDOWS_HPP_
