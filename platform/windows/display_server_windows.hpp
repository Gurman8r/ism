#ifndef _ISM_DISPLAY_SERVER_WINDOWS_HPP_
#define _ISM_DISPLAY_SERVER_WINDOWS_HPP_

#include <scene/main/window.hpp>

struct GLFWwindow;
struct GLFWmonitor;

namespace ism
{
	// windows display server
	class ISM_API DisplayServerWindows : public DisplayServer
	{
		OBJECT_COMMON(DisplayServerWindows, DisplayServer);

		GLFWwindow * m_glfw_window{};

	public:
		DisplayServerWindows(DisplayServerSettings const & settings);

		~DisplayServerWindows();

	public:
		virtual VideoMode const & get_desktop_video_mode() const override;
		virtual Vector<VideoMode> const & get_fullscreen_video_modes() const override;

	public:
		virtual WindowID get_current_context() const override;
		virtual void set_current_context(WindowID id) override;
		virtual void poll_events() override;
		virtual void swap_buffers(WindowID id) override;
		virtual void swap_interval(int32_t value) override;
		virtual void focus_window(WindowID id) override;
		virtual void hide_window(WindowID id) override;
		virtual void iconify_window(WindowID id) override;
		virtual void maximize_window(WindowID id) override;
		virtual void restore_window(WindowID id) override;
		virtual void request_window_attention(WindowID id) override;

	public:
		virtual CursorID create_custom_cursor(int32_t w, int32_t h, byte const * p, int32_t x, int32_t y) override;
		virtual CursorID create_standard_cursor(CursorShape_ shape) override;
		virtual void destroy_cursor(CursorID value) override;

	public:
		virtual String window_get_clipboard(WindowID id) const override;
		virtual Vec2f window_get_content_scale(WindowID id) const override;
		virtual IntRect window_get_frame_size(WindowID id) const override;
		virtual Vec2i window_get_framebuffer_size(WindowID id) const override;
		virtual int32_t window_get_input_mode(WindowID id, InputMode_ value) const override;
		virtual InputAction_ window_get_key(WindowID id, KeyCode_ value) const override;
		virtual InputAction_ window_get_mouse_button(WindowID id, MouseButton_ value) const override;
		virtual Vec2d window_get_mouse_position(WindowID id) const override;
		virtual void * window_get_native_handle(WindowID id) const override;
		virtual float_t window_get_opacity(WindowID id) const override;
		virtual Vec2i window_get_position(WindowID id) const override;
		virtual Vec2i window_get_size(WindowID id) const override;
		virtual bool window_get_should_close(WindowID id) const override;
		virtual void * window_get_user_pointer(WindowID id) const override;
		virtual bool window_is_decorated(WindowID id) const override;
		virtual bool window_is_floating(WindowID id) const override;
		virtual bool window_is_focused(WindowID id) const override;
		virtual bool window_is_hovered(WindowID id) const override;
		virtual bool window_is_iconified(WindowID id) const override;
		virtual bool window_is_maximized(WindowID id) const override;
		virtual bool window_is_resizable(WindowID id) const override;
		virtual bool window_is_transparent(WindowID id) const override;
		virtual bool window_is_visible(WindowID id) const override;

	public:
		virtual void window_set_clipboard(WindowID id, String const & value) override;
		virtual void window_set_cursor(WindowID id, CursorID value) override;
		virtual void window_set_cursor_mode(WindowID id, int32_t value) override;
		virtual void window_set_icons(WindowID id, int32_t width, int32_t height, uint8_t * pixels, int32_t count = 1) override;
		virtual void window_set_input_mode(WindowID id, int32_t mode, int32_t value) override;
		virtual void window_set_monitor(WindowID id, MonitorID monitor, IntRect const & bounds) override;
		virtual void window_set_mouse_position(WindowID id, Vec2d const & value) override;
		virtual void window_set_opacity(WindowID id, float_t value) override;
		virtual void window_set_position(WindowID id, Vec2i const & value) override;
		virtual void window_set_should_close(WindowID id, bool value) override;
		virtual void window_set_size(WindowID id, Vec2i const & value) override;
		virtual void window_set_title(WindowID id, String const & value) override;
		virtual void window_set_user_pointer(WindowID id, void * value) override;
		virtual void window_set_decorated(WindowID id, bool value) override;
		virtual void window_set_floating(WindowID id, bool value) override;
		virtual void window_set_resizable(WindowID id, bool value) override;

	public:
		virtual WindowCharCallback window_set_char_callback(WindowID id, WindowCharCallback value) override;
		virtual WindowCharModsCallback window_set_char_mods_callback(WindowID id, WindowCharModsCallback value) override;
		virtual WindowCloseCallback window_set_close_callback(WindowID id, WindowCloseCallback value) override;
		virtual WindowContentCallback window_set_content_scale_callback(WindowID id, WindowContentCallback value) override;
		virtual WindowDropCallback window_set_drop_callback(WindowID id, WindowDropCallback value) override;
		virtual WindowFocusCallback window_set_focus_callback(WindowID id, WindowFocusCallback value) override;
		virtual WindowFramebufferResizeCallback window_set_framebuffer_resize_callback(WindowID id, WindowFramebufferResizeCallback value) override;
		virtual WindowIconifyCallback window_set_iconify_callback(WindowID id, WindowIconifyCallback value) override;
		virtual WindowKeyCallback window_set_key_callback(WindowID id, WindowKeyCallback value) override;
		virtual WindowMaximizeCallback window_set_maximize_callback(WindowID id, WindowMaximizeCallback value) override;
		virtual WindowMouseButtonCallback window_set_mouse_button_callback(WindowID id, WindowMouseButtonCallback value) override;
		virtual WindowMouseEnterCallback window_set_mouse_enter_callback(WindowID id, WindowMouseEnterCallback value) override;
		virtual WindowMousePositionCallback window_set_mouse_position_callback(WindowID id, WindowMousePositionCallback value) override;
		virtual WindowPositionCallback window_set_position_callback(WindowID id, WindowPositionCallback value) override;
		virtual WindowRefreshCallback window_set_refresh_callback(WindowID id, WindowRefreshCallback value) override;
		virtual WindowScrollCallback window_set_scroll_callback(WindowID id, WindowScrollCallback value) override;
		virtual WindowSizeCallback window_set_size_callback(WindowID id, WindowSizeCallback value) override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ISM_DISPLAY_SERVER_WINDOWS_HPP_
