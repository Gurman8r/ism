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

		DS::WindowID m_window{};

	public:
		Window();
		virtual ~Window() override;

		DS::WindowID get_window_id() const noexcept { return m_window; }

		String get_title() const;
		void set_title(String const & value);

		DS::MonitorID get_monitor() const;
		void set_monitor(DS::MonitorID value, IntRect const & rect, int32_t refresh_rate = -1);

		Vec2i get_position() const;
		void set_position(Vec2i const & value);

		Vec2i get_size() const;
		void set_size(Vec2i const & value);

		DS::WindowMode_ get_mode() const;
		void set_mode(DS::WindowMode_ mode);

		Vec2i get_real_size() const;

		bool get_flag(int32_t flag) const;
		void set_flag(int32_t flag, bool enabled);

		void request_attention();
		void move_to_foreground();

		void set_visible(bool value);
		bool is_visible() const;

		void hide();
		void show();

		Vec2 get_content_scale() const;

		void grab_focus();
		bool has_focus() const;

		bool should_close() const;
		void set_should_close(bool value);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_WINDOW_HPP_
