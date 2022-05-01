#ifndef _ISM_EDITOR_VIEWPORT_HPP_
#define _ISM_EDITOR_VIEWPORT_HPP_

#include <editor/editor_panel.hpp>

namespace ism
{
	class ISM_API EditorViewport : public EditorPanel
	{
		OBJECT_COMMON(EditorViewport, EditorPanel);

		RID m_main_texture{};
		bool m_dragging_view{};

	public:
		EditorViewport();

		virtual ~EditorViewport();

	public:
		auto get_main_texture() const -> RID { return m_main_texture; }
		void set_main_texture(RID value) { m_main_texture = value; }

		bool is_dragging_view() const noexcept { return m_dragging_view; }

	public:
		virtual void draw() override;
	};
}

#endif // !_ISM_EDITOR_VIEWPORT_HPP_
