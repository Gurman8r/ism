#ifndef _ISM_WINDOWS_WINDOW_HPP_
#define _ISM_WINDOWS_WINDOW_HPP_

#include <scene/main/window.hpp>

#include <glfw/glfw3.h>

#if defined(ISM_OS_WINDOWS)
#	undef APIENTRY
#	include <Windows.h>
#	define GLFW_EXPOSE_NATIVE_WIN32
#	include <glfw/glfw3native.h>
#endif

namespace ism
{
	class ISM_API Windows_Window : public Window
	{
	protected:
		explicit Windows_Window(SceneTree * tree, Node * parent) : Window{ tree, parent } {}

		explicit Windows_Window(SceneTree * tree) : Window{ tree } {}

		explicit Windows_Window(Node * parent) : Window{ parent } {}

	public:
		virtual ~Windows_Window() override;

		static Windows_Window * new_(WindowSettings const & settings);

	public:
		virtual void focus() override;

		virtual void hide() override;

		virtual void iconify() override;

		virtual void maximize() override;

		virtual void restore() override;

		virtual void request_attention() override;

	public:
		NODISCARD virtual String get_clipboard() const override;

		NODISCARD virtual Vec2 get_content_scale() const override;

		NODISCARD virtual Vec2 get_framebuffer_size() const override;

		NODISCARD virtual WindowID get_handle() const override;

		NODISCARD virtual WindowHints get_hints() const override;

		NODISCARD virtual int32_t get_input_mode(int32_t value) const override;

		NODISCARD virtual int32_t get_key(int32_t value) const override;

		NODISCARD virtual int32_t get_mouse_button(int32_t value) const override;

		NODISCARD virtual Vec2 get_mouse_pos() const override;

		NODISCARD virtual WindowID get_native_handle() const override;

		NODISCARD virtual float_t get_opacity() const override;

		NODISCARD virtual Vec2 get_position() const override;

		NODISCARD virtual Vec2 get_size() const override;

		NODISCARD virtual String const & get_title() const override;

		NODISCARD virtual void * get_user_pointer() const override;

		NODISCARD virtual Rect get_window_frame_size() const override;

	public:
		NODISCARD virtual bool is_auto_iconify() const override;

		NODISCARD virtual bool is_decorated() const override;

		NODISCARD virtual bool is_floating() const override;

		NODISCARD virtual bool is_focused() const override;

		NODISCARD virtual bool is_focus_on_show() const override;

		NODISCARD virtual bool is_hovered() const override;

		NODISCARD virtual bool is_iconified() const override;

		NODISCARD virtual bool is_maximized() const override;

		NODISCARD virtual bool is_open() const override;

		NODISCARD virtual bool is_resizable() const override;

		NODISCARD virtual bool is_transparent() const override;

		NODISCARD virtual bool is_visible() const override;

	public:
		virtual void set_auto_iconify(bool value) override;

		virtual void set_clipboard(String const & value) override;

		virtual void set_cursor(CursorID value) override;

		virtual void set_cursor_mode(int32_t value) override;

		virtual void set_mouse_pos(Vec2 const & value) override;

		virtual void set_decorated(bool value) override;

		virtual void set_floating(bool value) override;

		virtual void set_focus_on_show(bool value) override;

		virtual void set_icons(int32_t count, size_t width, size_t height, byte const * pixels) override;

		virtual void set_input_mode(int32_t mode, int32_t value) override;

		virtual void set_opacity(float_t) override;

		virtual void set_position(Vec2 const & value) override;

		virtual void set_monitor(MonitorID monitor, Rect const & bounds) override;

		virtual void set_resizable(bool value) override;

		virtual void set_should_close(bool value) override;

		virtual void set_size(Vec2 const & value) override;

		virtual void set_title(String const & value) override;

		virtual void set_user_pointer(void * value) override;

	public:
		NODISCARD virtual window_char_callback get_char_callback() const override;

		NODISCARD virtual window_char_mods_callback get_char_mods_callback() const override;

		NODISCARD virtual window_close_callback get_close_callback() const override;

		NODISCARD virtual window_content_scale_callback get_content_scale_callback() const override;

		NODISCARD virtual window_mouse_enter_callback get_mouse_enter_callback() const override;

		NODISCARD virtual window_mouse_pos_callback get_mouse_pos_callback() const override;

		NODISCARD virtual window_drop_callback get_drop_callback() const override;

		NODISCARD virtual window_focus_callback get_focus_callback() const override;

		NODISCARD virtual window_framebuffer_resize_callback get_framebuffer_resize_callback() const override;

		NODISCARD virtual window_iconify_callback get_iconify_callback() const override;

		NODISCARD virtual window_key_callback get_key_callback() const override;

		NODISCARD virtual window_maximize_callback get_maximize_callback() const override;

		NODISCARD virtual window_mouse_button_callback get_mouse_button_callback() const override;

		NODISCARD virtual window_position_callback get_position_callback() const override;

		NODISCARD virtual window_refresh_callback get_refresh_callback() const override;

		NODISCARD virtual window_resize_callback get_resize_callback() const override;

		NODISCARD virtual window_scroll_callback get_scroll_callback() const override;

	public:
		virtual window_char_callback set_char_callback(window_char_callback value) override;

		virtual window_char_mods_callback set_char_mods_callback(window_char_mods_callback value) override;

		virtual window_close_callback set_close_callback(window_close_callback value) override;

		virtual window_content_scale_callback set_content_scale_callback(window_content_scale_callback value) override;

		virtual window_mouse_enter_callback set_mouse_enter_callback(window_mouse_enter_callback value) override;

		virtual window_mouse_pos_callback set_mouse_pos_callback(window_mouse_pos_callback value) override;

		virtual window_drop_callback set_drop_callback(window_drop_callback value) override;

		virtual window_focus_callback set_focus_callback(window_focus_callback value) override;

		virtual window_framebuffer_resize_callback set_framebuffer_resize_callback(window_framebuffer_resize_callback value) override;

		virtual window_iconify_callback set_iconify_callback(window_iconify_callback value) override;

		virtual window_key_callback set_key_callback(window_key_callback value) override;

		virtual window_maximize_callback set_maximize_callback(window_maximize_callback value) override;

		virtual window_mouse_button_callback set_mouse_button_callback(window_mouse_button_callback value) override;

		virtual window_position_callback set_position_callback(window_position_callback value) override;

		virtual window_refresh_callback set_refresh_callback(window_refresh_callback value) override;

		virtual window_resize_callback set_resize_callback(window_resize_callback value) override;

		virtual window_scroll_callback set_scroll_callback(window_scroll_callback value) override;

	private:
		String			m_title		{}; // 
		GLFWwindow *	m_window	{}; // 
		GLFWmonitor *	m_monitor	{}; // 
		WindowHints		m_hints		{}; // 

		struct // m_callbacks
		{
			window_char_callback				on_char					{}; // 
			window_char_mods_callback			on_char_mods			{}; // 
			window_close_callback				on_close				{}; // 
			window_content_scale_callback		on_content_scale		{}; // 
			window_drop_callback				on_drop					{}; // 
			window_focus_callback				on_focus				{}; // 
			window_framebuffer_resize_callback	on_framebuffer_resize	{}; // 
			window_iconify_callback				on_iconify				{}; // 
			window_key_callback					on_key					{}; // 
			window_maximize_callback			on_maximize				{}; // 
			window_mouse_button_callback		on_mouse_button			{}; // 
			window_mouse_enter_callback			on_mouse_enter			{}; // 
			window_mouse_pos_callback			on_mouse_pos			{}; // 
			window_position_callback			on_position				{}; // 
			window_refresh_callback				on_refresh				{}; // 
			window_resize_callback				on_resize				{}; // 
			window_scroll_callback				on_mouse_wheel			{}; // 
		}
		m_callbacks{};
	};
}

#endif // !_ISM_WINDOWS_WINDOW_HPP_
