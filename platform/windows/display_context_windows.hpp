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

	private:
		WindowCharCallback				m_on_char{};
		WindowCharModsCallback			m_on_char_mods{};
		WindowCloseCallback				m_on_close{};
		WindowContentCallback			m_on_content_scale{};
		WindowDropCallback				m_on_drop{};
		WindowFocusCallback				m_on_focus{};
		WindowFramebufferResizeCallback	m_on_framebuffer_resize{};
		WindowIconifyCallback			m_on_iconify{};
		WindowKeyCallback				m_on_key{};
		WindowMaximizeCallback			m_on_maximize{};
		WindowMouseButtonCallback		m_on_mouse_button{};
		WindowMouseEnterCallback		m_on_mouse_enter{};
		WindowMousePosCallback			m_on_mouse_pos{};
		WindowMouseScrollCallback		m_on_mouse_scroll{};
		WindowPositionCallback			m_on_position{};
		WindowRefreshCallback			m_on_refresh{};
		WindowResizeCallback			m_on_resize{};

	public:
		DisplayContextWindows();

		~DisplayContextWindows();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		NODISCARD virtual Window * window_new(WindowSettings const & settings) override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual WindowID get_current_context() const override;
		virtual void make_context_current(WindowID id) override;
		virtual void poll_events() override;
		virtual void swap_buffers(WindowID id) override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual void focus_window(WindowID id) override;
		virtual void hide_window(WindowID id) override;
		virtual void iconify_window(WindowID id) override;
		virtual void maximize_window(WindowID id) override;
		virtual void restore_window(WindowID id) override;
		virtual void request_window_attention(WindowID id) override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		NODISCARD virtual WindowCharCallback window_get_char_callback(WindowID id) const override;
		NODISCARD virtual WindowCharModsCallback window_get_char_mods_callback(WindowID id) const override;
		NODISCARD virtual WindowCloseCallback window_get_close_callback(WindowID id) const override;
		NODISCARD virtual WindowContentCallback window_get_content_scale_callback(WindowID id) const override;
		NODISCARD virtual WindowDropCallback window_get_drop_callback(WindowID id) const override;
		NODISCARD virtual WindowFocusCallback window_get_focus_callback(WindowID id) const override;
		NODISCARD virtual WindowFramebufferResizeCallback window_get_framebuffer_resize_callback(WindowID id) const override;
		NODISCARD virtual WindowIconifyCallback window_get_iconify_callback(WindowID id) const override;
		NODISCARD virtual WindowKeyCallback window_get_key_callback(WindowID id) const override;
		NODISCARD virtual WindowMaximizeCallback window_get_maximize_callback(WindowID id) const override;
		NODISCARD virtual WindowMouseButtonCallback window_get_mouse_button_callback(WindowID id) const override;
		NODISCARD virtual WindowMouseEnterCallback window_get_mouse_enter_callback(WindowID id) const override;
		NODISCARD virtual WindowMousePosCallback window_get_mouse_pos_callback(WindowID id) const override;
		NODISCARD virtual WindowPositionCallback window_get_position_callback(WindowID id) const override;
		NODISCARD virtual WindowRefreshCallback window_get_refresh_callback(WindowID id) const override;
		NODISCARD virtual WindowResizeCallback window_get_resize_callback(WindowID id) const override;
		NODISCARD virtual WindowMouseScrollCallback window_get_scroll_callback(WindowID id) const override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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
		virtual WindowMousePosCallback window_set_mouse_pos_callback(WindowID id, WindowMousePosCallback value) override;
		virtual WindowPositionCallback window_set_position_callback(WindowID id, WindowPositionCallback value) override;
		virtual WindowRefreshCallback window_set_refresh_callback(WindowID id, WindowRefreshCallback value) override;
		virtual WindowResizeCallback window_set_resize_callback(WindowID id, WindowResizeCallback value) override;
		virtual WindowMouseScrollCallback window_set_mouse_scroll_callback(WindowID id, WindowMouseScrollCallback value) override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		NODISCARD virtual VideoMode const & get_desktop_video_mode() const override;
		NODISCARD virtual Vector<VideoMode> const & get_fullscreen_video_modes() const override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ISM_DISPLAY_CONTEXT_WINDOWS_HPP_
