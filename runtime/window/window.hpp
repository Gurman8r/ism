#ifndef _ISM_WINDOW_HPP_
#define _ISM_WINDOW_HPP_

#include <runtime/window/viewport.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// window
	class ISM_API Window : public Viewport
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		OBJECT_COMMON(Window, Viewport);

	protected:
		WindowID m_window_id{};

	public:
		Window(WindowID window = nullptr) noexcept : Viewport{} { m_window_id = window; }

		Window(Window && other) noexcept : Viewport{} { swap(std::move(other)); }
		
		Window & operator=(Window && other) noexcept { return swap(std::move(other)), (*this); }

		void swap(Window & other) noexcept { if (this != std::addressof(other)) { std::swap(m_window_id, other.m_window_id); } }

		NODISCARD WindowID get_id() const { return m_window_id; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static Window * new_(WindowSettings const & settings);
		virtual void make_context_current();
		virtual void poll_events();
		virtual void swap_buffers();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual void focus();
		virtual void hide();
		virtual void iconify();
		virtual void maximize();
		virtual void restore();
		virtual void request_attention();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		NODISCARD virtual String get_clipboard() const;
		NODISCARD virtual Vec2 get_content_scale() const;
		NODISCARD virtual Vec2 get_framebuffer_size() const;
		NODISCARD virtual int32_t get_input_mode(int32_t value) const;
		NODISCARD virtual int32_t get_key(int32_t value) const;
		NODISCARD virtual int32_t get_mouse_button(int32_t value) const;
		NODISCARD virtual Vec2 get_mouse_pos() const;
		NODISCARD virtual WindowID get_native_handle() const;
		NODISCARD virtual float_t get_opacity() const;
		NODISCARD virtual Vec2 get_position() const;
		NODISCARD virtual Vec2 get_size() const;
		NODISCARD virtual void * get_user_pointer() const;
		NODISCARD virtual Rect get_frame_size() const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		NODISCARD virtual bool is_auto_iconify() const;
		NODISCARD virtual bool is_decorated() const;
		NODISCARD virtual bool is_floating() const;
		NODISCARD virtual bool is_focused() const;
		NODISCARD virtual bool is_focus_on_show() const;
		NODISCARD virtual bool is_hovered() const;
		NODISCARD virtual bool is_iconified() const;
		NODISCARD virtual bool is_maximized() const;
		NODISCARD virtual bool is_open() const;
		NODISCARD virtual bool is_resizable() const;
		NODISCARD virtual bool is_transparent() const;
		NODISCARD virtual bool is_visible() const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual void set_auto_iconify(bool value);
		virtual void set_clipboard(String const & value);
		virtual void set_cursor(CursorID value);
		virtual void set_cursor_mode(int32_t value);
		virtual void set_mouse_pos(Vec2 const & value);
		virtual void set_decorated(bool value);
		virtual void set_floating(bool value);
		virtual void set_focus_on_show(bool value);
		virtual void set_icons(int32_t width, int32_t height, uint8_t * pixels, int32_t count = 1);
		virtual void set_input_mode(int32_t mode, int32_t value);
		virtual void set_opacity(float_t value);
		virtual void set_position(Vec2 const & value);
		virtual void set_monitor(MonitorID monitor, Rect const & bounds);
		virtual void set_resizable(bool value);
		virtual void set_should_close(bool value);
		virtual void set_size(Vec2 const & value);
		virtual void set_title(String const & value);
		virtual void set_user_pointer(void * value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		NODISCARD virtual WindowCharCallback get_char_callback() const;
		NODISCARD virtual WindowCharModsCallback get_char_mods_callback() const;
		NODISCARD virtual WindowCloseCallback get_close_callback() const;
		NODISCARD virtual WindowContentCallback get_content_scale_callback() const;
		NODISCARD virtual WindowMouseEnterCallback get_mouse_enter_callback() const;
		NODISCARD virtual WindowMousePosCallback get_mouse_pos_callback() const;
		NODISCARD virtual WindowDropCallback get_drop_callback() const;
		NODISCARD virtual WindowFocusCallback get_focus_callback() const;
		NODISCARD virtual WindowFramebufferResizeCallback get_framebuffer_resize_callback() const;
		NODISCARD virtual WindowIconifyCallback get_iconify_callback() const;
		NODISCARD virtual WindowKeyCallback get_key_callback() const;
		NODISCARD virtual WindowMaximizeCallback get_maximize_callback() const;
		NODISCARD virtual WindowMouseButtonCallback get_mouse_button_callback() const;
		NODISCARD virtual WindowPositionCallback get_position_callback() const;
		NODISCARD virtual WindowRefreshCallback get_refresh_callback() const;
		NODISCARD virtual WindowResizeCallback get_resize_callback() const;
		NODISCARD virtual WindowMouseScrollCallback get_scroll_callback() const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual WindowCharCallback set_char_callback(WindowCharCallback value);
		virtual WindowCharModsCallback set_char_mods_callback(WindowCharModsCallback value);
		virtual WindowCloseCallback set_close_callback(WindowCloseCallback value);
		virtual WindowContentCallback set_content_scale_callback(WindowContentCallback value);
		virtual WindowDropCallback set_drop_callback(WindowDropCallback value);
		virtual WindowFocusCallback set_focus_callback(WindowFocusCallback value);
		virtual WindowFramebufferResizeCallback set_framebuffer_resize_callback(WindowFramebufferResizeCallback value);
		virtual WindowIconifyCallback set_iconify_callback(WindowIconifyCallback value);
		virtual WindowKeyCallback set_key_callback(WindowKeyCallback value);
		virtual WindowMaximizeCallback set_maximize_callback(WindowMaximizeCallback value);
		virtual WindowMouseButtonCallback set_mouse_button_callback(WindowMouseButtonCallback value);
		virtual WindowMouseEnterCallback set_mouse_enter_callback(WindowMouseEnterCallback value);
		virtual WindowMousePosCallback set_mouse_pos_callback(WindowMousePosCallback value);
		virtual WindowMouseScrollCallback set_mouse_scroll_callback(WindowMouseScrollCallback value);
		virtual WindowPositionCallback set_position_callback(WindowPositionCallback value);
		virtual WindowRefreshCallback set_refresh_callback(WindowRefreshCallback value);
		virtual WindowResizeCallback set_resize_callback(WindowResizeCallback value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_WINDOW_HPP_
