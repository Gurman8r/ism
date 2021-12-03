#ifndef _ISM_WINDOW_HPP_
#define _ISM_WINDOW_HPP_

#include <scene/main/node.hpp>
#include <servers/display_server.hpp>
#include <servers/rendering_server.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// window
	class ISM_API Window : public Node
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		OBJECT_COMMON(Window, Node);

	protected:
		friend class SceneTree;

		WindowID m_window_id{};

	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		explicit Window() noexcept;
		virtual ~Window();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		NODISCARD operator WindowID() const noexcept { return m_window_id; }
		NODISCARD auto get_window_id() const noexcept -> WindowID { return m_window_id; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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

		NODISCARD inline IntRect get_bounds() const noexcept { return { get_position(), get_size() }; }
		NODISCARD virtual String get_clipboard() const;
		NODISCARD virtual Vec2f get_content_scale() const;
		NODISCARD virtual IntRect get_frame_size() const;
		NODISCARD virtual Vec2i get_framebuffer_size() const;
		NODISCARD virtual int32_t get_input_mode(InputMode value) const;
		NODISCARD virtual InputAction get_key(KeyCode value) const;
		NODISCARD virtual InputAction get_mouse_button(MouseButton value) const;
		NODISCARD virtual Vec2d get_mouse_position() const;
		NODISCARD virtual void * get_native_handle() const;
		NODISCARD virtual float_t get_opacity() const;
		NODISCARD virtual Vec2i get_position() const;
		NODISCARD virtual Vec2i get_size() const;
		NODISCARD virtual bool get_should_close() const;
		NODISCARD virtual void * get_user_pointer() const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		NODISCARD virtual bool is_decorated() const;
		NODISCARD virtual bool is_floating() const;
		NODISCARD virtual bool is_focused() const;
		NODISCARD virtual bool is_hovered() const;
		NODISCARD virtual bool is_iconified() const;
		NODISCARD virtual bool is_maximized() const;
		NODISCARD virtual bool is_resizable() const;
		NODISCARD virtual bool is_transparent() const;
		NODISCARD virtual bool is_visible() const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual void set_clipboard(String const & value);
		virtual void set_cursor(CursorID value);
		virtual void set_input_mode(InputMode value);
		virtual void set_icons(int32_t width, int32_t height, uint8_t * pixels, int32_t count = 1);
		virtual void set_input_mode(int32_t mode, int32_t value);
		virtual void set_monitor(MonitorID monitor, IntRect const & bounds);
		virtual void set_mouse_position(Vec2d const & value);
		virtual void set_opacity(float_t value);
		virtual void set_position(Vec2i const & value);
		virtual void set_title(String const & value);
		virtual void set_should_close(bool value);
		virtual void set_size(Vec2i const & value);
		virtual void set_user_pointer(void * value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual void set_is_decorated(bool value);
		virtual void set_is_floating(bool value);
		virtual void set_is_resizable(bool value);

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
		virtual WindowMousePositionCallback set_mouse_position_callback(WindowMousePositionCallback value);
		virtual WindowPositionCallback set_position_callback(WindowPositionCallback value);
		virtual WindowRefreshCallback set_refresh_callback(WindowRefreshCallback value);
		virtual WindowScrollCallback set_scroll_callback(WindowScrollCallback value);
		virtual WindowSizeCallback set_size_callback(WindowSizeCallback value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_WINDOW_HPP_
