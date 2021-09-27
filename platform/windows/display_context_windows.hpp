#ifndef _ISM_DISPLAY_CONTEXT_WINDOWS_HPP_
#define _ISM_DISPLAY_CONTEXT_WINDOWS_HPP_

#include <runtime/window/window.hpp>

struct GLFWwindow;
struct GLFWmonitor;

namespace ism
{
	// windows display server
	class ISM_API DisplayContextWindows : public DisplayContext
	{
		OBJECT_COMMON(DisplayContextWindows, DisplayContext);

	public:
		DisplayContextWindows();

		~DisplayContextWindows();

		NODISCARD virtual Window * create_window(WindowSettings const & settings) override;

	public:
		virtual void poll_events() override;
		virtual void swap_buffers(WindowID id) override;
		virtual WindowID get_current_context() const override;
		virtual void make_context_current(WindowID id) override;

	public:
		virtual void focus_window(WindowID id) override;
		virtual void hide_window(WindowID id) override;
		virtual void iconify_window(WindowID id) override;
		virtual void maximize_window(WindowID id) override;
		virtual void restore_window(WindowID id) override;
		virtual void request_window_attention(WindowID id) override;

	public:
		NODISCARD virtual String window_get_clipboard(WindowID id) const override;
		NODISCARD virtual Vec2 window_get_content_scale(WindowID id) const override;
		NODISCARD virtual Vec2 window_get_framebuffer_size(WindowID id) const override;
		NODISCARD virtual int32_t window_get_input_mode(WindowID id, int32_t value) const override;
		NODISCARD virtual int32_t window_get_key(WindowID id, int32_t value) const override;
		NODISCARD virtual int32_t window_get_mouse_button(WindowID id, int32_t value) const override;
		NODISCARD virtual Vec2 window_get_mouse_pos(WindowID id) const override;
		NODISCARD virtual WindowID window_get_native_handle(WindowID id) const override;
		NODISCARD virtual float_t window_get_opacity(WindowID id) const override;
		NODISCARD virtual Vec2 window_get_position(WindowID id) const override;
		NODISCARD virtual Vec2 window_get_size(WindowID id) const override;
		NODISCARD virtual void * window_get_user_pointer(WindowID id) const override;
		NODISCARD virtual Rect window_get_frame_size(WindowID id) const override;

	public:
		NODISCARD virtual bool window_is_auto_iconify(WindowID id) const override;
		NODISCARD virtual bool window_is_decorated(WindowID id) const override;
		NODISCARD virtual bool window_is_floating(WindowID id) const override;
		NODISCARD virtual bool window_is_focused(WindowID id) const override;
		NODISCARD virtual bool window_is_focus_on_show(WindowID id) const override;
		NODISCARD virtual bool window_is_hovered(WindowID id) const override;
		NODISCARD virtual bool window_is_iconified(WindowID id) const override;
		NODISCARD virtual bool window_is_maximized(WindowID id) const override;
		NODISCARD virtual bool window_is_open(WindowID id) const override;
		NODISCARD virtual bool window_is_resizable(WindowID id) const override;
		NODISCARD virtual bool window_is_transparent(WindowID id) const override;
		NODISCARD virtual bool window_is_visible(WindowID id) const override;

	public:
		virtual void window_set_auto_iconify(WindowID id, bool value) override;
		virtual void window_set_clipboard(WindowID id, String const & value) override;
		virtual void window_set_cursor(WindowID id, CursorID value) override;
		virtual void window_set_cursor_mode(WindowID id, int32_t value) override;
		virtual void window_set_mouse_pos(WindowID id, Vec2 const & value) override;
		virtual void window_set_decorated(WindowID id, bool value) override;
		virtual void window_set_floating(WindowID id, bool value) override;
		virtual void window_set_focus_on_show(WindowID id, bool value) override;
		virtual void window_set_icons(WindowID id, int32_t width, int32_t height, uint8_t * pixels, int32_t count = 1) override;
		virtual void window_set_input_mode(WindowID id, int32_t mode, int32_t value) override;
		virtual void window_set_opacity(WindowID id, float_t value) override;
		virtual void window_set_position(WindowID id, Vec2 const & value) override;
		virtual void window_set_monitor(WindowID id, MonitorID monitor, Rect const & bounds) override;
		virtual void window_set_resizable(WindowID id, bool value) override;
		virtual void window_set_should_close(WindowID id, bool value) override;
		virtual void window_set_size(WindowID id, Vec2 const & value) override;
		virtual void window_set_title(WindowID id, String const & value) override;
		virtual void window_set_user_pointer(WindowID id, void * value) override;

	public:
		NODISCARD virtual window_char_callback window_get_char_callback(WindowID id) const override;
		NODISCARD virtual window_char_mods_callback window_get_char_mods_callback(WindowID id) const override;
		NODISCARD virtual window_close_callback window_get_close_callback(WindowID id) const override;
		NODISCARD virtual window_content_scale_callback window_get_content_scale_callback(WindowID id) const override;
		NODISCARD virtual window_drop_callback window_get_drop_callback(WindowID id) const override;
		NODISCARD virtual window_focus_callback window_get_focus_callback(WindowID id) const override;
		NODISCARD virtual window_framebuffer_resize_callback window_get_framebuffer_resize_callback(WindowID id) const override;
		NODISCARD virtual window_iconify_callback window_get_iconify_callback(WindowID id) const override;
		NODISCARD virtual window_key_callback window_get_key_callback(WindowID id) const override;
		NODISCARD virtual window_maximize_callback window_get_maximize_callback(WindowID id) const override;
		NODISCARD virtual window_mouse_button_callback window_get_mouse_button_callback(WindowID id) const override;
		NODISCARD virtual window_mouse_enter_callback window_get_mouse_enter_callback(WindowID id) const override;
		NODISCARD virtual window_mouse_pos_callback window_get_mouse_pos_callback(WindowID id) const override;
		NODISCARD virtual window_position_callback window_get_position_callback(WindowID id) const override;
		NODISCARD virtual window_refresh_callback window_get_refresh_callback(WindowID id) const override;
		NODISCARD virtual window_resize_callback window_get_resize_callback(WindowID id) const override;
		NODISCARD virtual window_scroll_callback window_get_scroll_callback(WindowID id) const override;

	public:
		virtual window_char_callback window_set_char_callback(WindowID id, window_char_callback value) override;
		virtual window_char_mods_callback window_set_char_mods_callback(WindowID id, window_char_mods_callback value) override;
		virtual window_close_callback window_set_close_callback(WindowID id, window_close_callback value) override;
		virtual window_content_scale_callback window_set_content_scale_callback(WindowID id, window_content_scale_callback value) override;
		virtual window_drop_callback window_set_drop_callback(WindowID id, window_drop_callback value) override;
		virtual window_focus_callback window_set_focus_callback(WindowID id, window_focus_callback value) override;
		virtual window_framebuffer_resize_callback window_set_framebuffer_resize_callback(WindowID id, window_framebuffer_resize_callback value) override;
		virtual window_iconify_callback window_set_iconify_callback(WindowID id, window_iconify_callback value) override;
		virtual window_key_callback window_set_key_callback(WindowID id, window_key_callback value) override;
		virtual window_maximize_callback window_set_maximize_callback(WindowID id, window_maximize_callback value) override;
		virtual window_mouse_button_callback window_set_mouse_button_callback(WindowID id, window_mouse_button_callback value) override;
		virtual window_mouse_enter_callback window_set_mouse_enter_callback(WindowID id, window_mouse_enter_callback value) override;
		virtual window_mouse_pos_callback window_set_mouse_pos_callback(WindowID id, window_mouse_pos_callback value) override;
		virtual window_position_callback window_set_position_callback(WindowID id, window_position_callback value) override;
		virtual window_refresh_callback window_set_refresh_callback(WindowID id, window_refresh_callback value) override;
		virtual window_resize_callback window_set_resize_callback(WindowID id, window_resize_callback value) override;
		virtual window_scroll_callback window_set_scroll_callback(WindowID id, window_scroll_callback value) override;

	public:
		NODISCARD virtual VideoMode const & get_desktop_video_mode() const override;

		NODISCARD virtual Vector<VideoMode> const & get_fullscreen_video_modes() const override;
	};
}

#endif // !_ISM_DISPLAY_CONTEXT_WINDOWS_HPP_
