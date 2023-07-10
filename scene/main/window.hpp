#ifndef _ISM_WINDOW_HPP_
#define _ISM_WINDOW_HPP_

#include <scene/main/viewport.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// window
	class ISM_API Window : public Viewport
	{
		OBJECT_CLASS(Window, Viewport);

		DS::WindowID m_window{ DS::MAIN_WINDOW_ID };

	public:
		Window();
		virtual ~Window() override;

		DS::WindowID get_window_id() const noexcept { return m_window; }

		String get_title() const;
		void set_title(String const & title);

		i32 get_current_screen() const;
		void set_current_screen(i32 screen);

		Vec2i get_position() const;
		void set_position(Vec2i const & position);

		Vec2i get_size() const;
		void set_size(Vec2i const & size);

		Vec2i get_real_size() const;

		DS::WindowMode_ get_mode() const;
		void set_mode(DS::WindowMode_ mode);

		bool get_flag(i32 flag) const;
		void set_flag(i32 flag, bool enabled);

		void request_attention();
		void move_to_foreground();

		void set_visible(bool value);
		bool is_visible() const;

		void hide();
		void show();

		Vec2 get_scale() const;

		void grab_focus();
		bool has_focus() const;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_WINDOW_HPP_
