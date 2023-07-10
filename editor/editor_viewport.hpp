#if TOOLS_ENABLED
#ifndef _ISM_EDITOR_VIEWPORT_HPP_
#define _ISM_EDITOR_VIEWPORT_HPP_

#include <editor/editor_panel.hpp>

namespace Ism
{
	// viewport component of the main editor
	class ISM_API EditorViewport : public EditorPanel
	{
		OBJECT_CLASS(EditorViewport, EditorPanel);

		friend class EditorNode;

		RID m_viewport{};

		RID m_main_texture{};

		RID m_camera{};

		Mat4 m_camera_view{}, m_camera_proj{};

		bool m_is_dragging_view{};

		bool	m_grid_enabled;
		Mat4	m_grid_matrix;
		f32		m_grid_size;

	public:
		EditorViewport();

		virtual ~EditorViewport();

		virtual void process(Duration const & dt) override;
	};
}

#endif // !_ISM_EDITOR_VIEWPORT_HPP_
#endif // TOOLS_ENABLED
