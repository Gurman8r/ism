#ifndef _ISM_DISPLAY_SERVER_WINDOWS_HPP_
#define _ISM_DISPLAY_SERVER_WINDOWS_HPP_

#include <scene/main/window.hpp>

struct GLFWwindow;

namespace ism
{
	// windows display server
	class ISM_API DisplayServerWindows : public DisplayServer
	{
		OBJECT_COMMON(DisplayServerWindows, DisplayServer);

		struct _Window
		{
			GLFWwindow * handle{};
			String title{};
			WindowMode_ window_mode{};
			MouseMode_ mouse_mode{};
			CursorShape_ cursor_shape{};
		}
		m_window{};

	public:
		DisplayServerWindows(String const & title, WindowMode_ mode, Vec2i const & size);
		~DisplayServerWindows();

	public:
		VideoMode const & get_desktop_video_mode() const;
		List<VideoMode> const & get_fullscreen_video_modes() const;

	public:
		virtual WindowID get_current_context() const override;
		virtual void set_current_context(WindowID window) override;

		virtual bool window_should_close(WindowID window) const override;
		virtual void window_set_should_close(WindowID window, bool value) override;

		virtual String get_clipboard() const override;
		virtual void set_clipboard(String const & value) override;

		virtual MouseMode_ mouse_get_mode() const override;
		virtual void mouse_set_mode(MouseMode_ value) override;

		virtual int32_t mouse_get_button(MouseButton_ value) const override;
		virtual Vec2 mouse_get_position() const override;
		virtual void mouse_set_position(Vec2 const & value) override;

		virtual String window_get_title(WindowID window) const override;
		virtual void window_set_title(WindowID window, String const & value) override;

		virtual MonitorID window_get_monitor(WindowID window) const override;
		virtual void window_set_monitor(WindowID window, MonitorID monitor) override;

		virtual Vec2i window_get_position(WindowID window) const override;
		virtual void window_set_position(WindowID window, Vec2i const & value) override;

		virtual Vec2i window_get_size(WindowID window) const override;
		virtual void window_set_size(WindowID window, Vec2i const & value) override;

		virtual Vec2i window_get_real_size(WindowID window) const override;

		virtual WindowMode_ window_get_mode(WindowID window) const override;
		virtual void window_set_mode(WindowID window, WindowMode_ value) override;

		virtual bool window_get_flag(WindowID window, int32_t flag) const override;
		virtual void window_set_flag(WindowID window, int32_t flag, bool enabled) override;

		virtual void request_window_attention(WindowID window) override;
		virtual void move_window_to_foreground(WindowID window) override;

		virtual void window_set_visible(WindowID window, bool value) override;
		virtual bool window_is_visible(WindowID window) const override;

		virtual Vec2 window_get_content_scale(WindowID window) const override;

		virtual bool window_has_focus(WindowID window) const override;
		virtual void window_grab_focus(WindowID window) override;

		virtual CursorShape_ cursor_get_shape() const override;
		virtual void cursor_set_shape(CursorShape_ value) override;
		virtual void cursor_set_custom_image(RES const & cursor, CursorShape_ shape = {}, Vec2 const & hotspot = {}) override;

		virtual void poll_events() override;
		virtual void swap_buffers() override;

		virtual void set_native_icon(Path const & value) override;
		virtual void set_icon(Ref<Image> const & value) override;

	public:
		virtual ErrorCallback set_error_callback(ErrorCallback callback) override;
		virtual CharCallback window_set_char_callback(WindowID window, CharCallback value) override;
		virtual CharModsCallback window_set_char_mods_callback(WindowID window, CharModsCallback value) override;
		virtual CloseCallback window_set_close_callback(WindowID window, CloseCallback value) override;
		virtual ContentScaleCallback window_set_content_scale_callback(WindowID window, ContentScaleCallback value) override;
		virtual DropCallback window_set_drop_callback(WindowID window, DropCallback value) override;
		virtual FocusCallback window_set_focus_callback(WindowID window, FocusCallback value) override;
		virtual FramebufferResizeCallback window_set_framebuffer_resize_callback(WindowID window, FramebufferResizeCallback value) override;
		virtual IconifyCallback window_set_iconify_callback(WindowID window, IconifyCallback value) override;
		virtual KeyCallback window_set_key_callback(WindowID window, KeyCallback value) override;
		virtual MaximizeCallback window_set_maximize_callback(WindowID window, MaximizeCallback value) override;
		virtual MouseButtonCallback window_set_mouse_button_callback(WindowID window, MouseButtonCallback value) override;
		virtual MouseEnterCallback window_set_mouse_enter_callback(WindowID window, MouseEnterCallback value) override;
		virtual MousePositionCallback window_set_mouse_position_callback(WindowID window, MousePositionCallback value) override;
		virtual PositionCallback window_set_position_callback(WindowID window, PositionCallback value) override;
		virtual RefreshCallback window_set_refresh_callback(WindowID window, RefreshCallback value) override;
		virtual ScrollCallback window_set_scroll_callback(WindowID window, ScrollCallback value) override;
		virtual SizeCallback window_set_size_callback(WindowID window, SizeCallback value) override;
		virtual MonitorCallback set_monitor_callback(MonitorCallback callback) override;
		virtual JoystickCallback set_joystick_callback(JoystickCallback callback) override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ISM_DISPLAY_SERVER_WINDOWS_HPP_
