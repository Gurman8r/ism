#ifndef _ISM_EDITOR_VIEWPORT_HPP_
#define _ISM_EDITOR_VIEWPORT_HPP_

#include <editor/editor_panel.hpp>

namespace ism
{
	class ISM_API EditorViewport : public EditorPanel
	{
		OBJECT_CLASS(EditorViewport, EditorPanel);

		friend class EditorNode;

		RID m_main_texture;

		Mat4 m_camera_view{}, m_camera_proj{};

		bool m_is_dragging_view{};

		bool	m_grid_enabled;
		Mat4	m_grid_matrix;
		f32		m_grid_size;

	public:
		EditorViewport();

		virtual ~EditorViewport();

		virtual void process(Duration const & delta_time) override;
	};
}

#endif // !_ISM_EDITOR_VIEWPORT_HPP_
