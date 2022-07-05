#ifndef _ISM_WINDOW_HPP_
#define _ISM_WINDOW_HPP_

#include <scene/main/viewport.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// window
	class ISM_API Window : public Viewport
	{
		OBJECT_COMMON(Window, Viewport);

		friend class SceneTree;

		WindowID m_window{};

	public:
		Window();
		virtual ~Window() override;

		WindowID get_window_id() const noexcept { return m_window; }
		operator WindowID() const noexcept { return m_window; }

	public:
		virtual void make_current_context();
		virtual void poll_events();
		virtual void swap_buffers();

	public:
		virtual void focus();
		virtual void hide();
		virtual void iconify();
		virtual void maximize();
		virtual void restore();
		virtual void request_attention();

	public:
		virtual String get_clipboard() const;
		virtual Vec2f get_content_scale() const;
		virtual IntRect get_frame_size() const;
		virtual Vec2i get_framebuffer_size() const;
		virtual int32_t get_input_mode(InputMode_ value) const;
		virtual InputAction_ get_key(KeyCode_ value) const;
		virtual InputAction_ get_mouse_button(MouseButton_ value) const;
		virtual Vec2d get_mouse_position() const;
		virtual void * get_native_handle() const;
		virtual float_t get_opacity() const;
		virtual Vec2i get_position() const override;
		virtual Vec2i get_size() const override;
		virtual bool get_should_close() const;
		virtual void * get_user_pointer() const;
		virtual bool is_decorated() const;
		virtual bool is_floating() const;
		virtual bool is_focused() const;
		virtual bool is_hovered() const;
		virtual bool is_iconified() const;
		virtual bool is_maximized() const;
		virtual bool is_resizable() const;
		virtual bool is_transparent() const;
		virtual bool is_visible() const;

	public:
		virtual void set_clipboard(String const & value);
		virtual void set_cursor(CursorID value);
		virtual void set_input_mode(InputMode_ value);
		virtual void set_icons(int32_t width, int32_t height, uint8_t * pixels, int32_t count = 1);
		virtual void set_input_mode(int32_t mode, int32_t value);
		virtual void set_monitor(MonitorID monitor, IntRect const & bounds);
		virtual void set_mouse_position(Vec2d const & value);
		virtual void set_opacity(float_t value);
		virtual void set_position(Vec2i const & value) override;
		virtual void set_title(String const & value);
		virtual void set_should_close(bool value);
		virtual void set_size(Vec2i const & value) override;
		virtual void set_user_pointer(void * value);
		virtual void set_decorated(bool value);
		virtual void set_floating(bool value);
		virtual void set_resizable(bool value);

	public:
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
		virtual WindowMousePositionCallback set_mouse_position_callback(WindowMousePositionCallback value);
		virtual WindowPositionCallback set_position_callback(WindowPositionCallback value);
		virtual WindowRefreshCallback set_refresh_callback(WindowRefreshCallback value);
		virtual WindowScrollCallback set_scroll_callback(WindowScrollCallback value);
		virtual WindowSizeCallback set_size_callback(WindowSizeCallback value);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_WINDOW_HPP_
