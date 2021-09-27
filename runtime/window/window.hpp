#ifndef _ISM_WINDOW_HPP_
#define _ISM_WINDOW_HPP_

#include <runtime/window/viewport.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// window
	class ISM_API Window : public Viewport
	{
		OBJECT_COMMON(Window, Viewport);

	protected:
		WindowID	m_window	{};
		WindowHints	m_hints		{ WindowHints_Default };

	public:
		static Window * new_(WindowSettings const & settings);

		Window(WindowID window = nullptr, WindowHints hints = WindowHints_Default)
			: Viewport	{}
			, m_window	{ window }
			, m_hints	{ hints }
		{
		}

	public:
		virtual void poll_events();
		virtual void swap_buffers();
		virtual void make_context_current();

	public:
		virtual void focus();
		virtual void hide();
		virtual void iconify();
		virtual void maximize();
		virtual void restore();
		virtual void request_attention();

	public:
		NODISCARD virtual WindowID get_handle() const { return m_window; }
		NODISCARD virtual WindowHints get_hints() const { return m_hints; }
		NODISCARD inline bool has_hints(int32_t value) const noexcept { return flag_read((int32_t)get_hints(), value); }

	public:
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

	public:
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

	public:
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

	public:
		NODISCARD virtual window_char_callback get_char_callback() const;
		NODISCARD virtual window_char_mods_callback get_char_mods_callback() const;
		NODISCARD virtual window_close_callback get_close_callback() const;
		NODISCARD virtual window_content_scale_callback get_content_scale_callback() const;
		NODISCARD virtual window_mouse_enter_callback get_mouse_enter_callback() const;
		NODISCARD virtual window_mouse_pos_callback get_mouse_pos_callback() const;
		NODISCARD virtual window_drop_callback get_drop_callback() const;
		NODISCARD virtual window_focus_callback get_focus_callback() const;
		NODISCARD virtual window_framebuffer_resize_callback get_framebuffer_resize_callback() const;
		NODISCARD virtual window_iconify_callback get_iconify_callback() const;
		NODISCARD virtual window_key_callback get_key_callback() const;
		NODISCARD virtual window_maximize_callback get_maximize_callback() const;
		NODISCARD virtual window_mouse_button_callback get_mouse_button_callback() const;
		NODISCARD virtual window_position_callback get_position_callback() const;
		NODISCARD virtual window_refresh_callback get_refresh_callback() const;
		NODISCARD virtual window_resize_callback get_resize_callback() const;
		NODISCARD virtual window_scroll_callback get_scroll_callback() const;

	public:
		virtual window_char_callback set_char_callback(window_char_callback value);
		virtual window_char_mods_callback set_char_mods_callback(window_char_mods_callback value);
		virtual window_close_callback set_close_callback(window_close_callback value);
		virtual window_content_scale_callback set_content_scale_callback(window_content_scale_callback value);
		virtual window_mouse_enter_callback set_mouse_enter_callback(window_mouse_enter_callback value);
		virtual window_mouse_pos_callback set_mouse_pos_callback(window_mouse_pos_callback value);
		virtual window_drop_callback set_drop_callback(window_drop_callback value);
		virtual window_focus_callback set_focus_callback(window_focus_callback value);
		virtual window_framebuffer_resize_callback set_framebuffer_resize_callback(window_framebuffer_resize_callback value);
		virtual window_iconify_callback set_iconify_callback(window_iconify_callback value);
		virtual window_key_callback set_key_callback(window_key_callback value);
		virtual window_maximize_callback set_maximize_callback(window_maximize_callback value);
		virtual window_mouse_button_callback set_mouse_button_callback(window_mouse_button_callback value);
		virtual window_position_callback set_position_callback(window_position_callback value);
		virtual window_refresh_callback set_refresh_callback(window_refresh_callback value);
		virtual window_resize_callback set_resize_callback(window_resize_callback value);
		virtual window_scroll_callback set_scroll_callback(window_scroll_callback value);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_WINDOW_HPP_
