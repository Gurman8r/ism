#ifndef _ISM_EDITOR_VIEWPORT_HPP_
#define _ISM_EDITOR_VIEWPORT_HPP_

#include <editor/editor_camera.hpp>
#include <editor/editor_panel.hpp>

namespace ism
{
	class ISM_API EditorViewport : public EditorPanel
	{
		OBJECT_COMMON(EditorViewport, EditorPanel);

		RID m_main_texture;
		
		EditorCamera m_editor_camera;

		bool	m_grid_enabled;
		Mat4	m_grid_matrix;
		float_t	m_grid_size;

	public:
		EditorViewport();

		virtual ~EditorViewport();

	public:
		auto get_main_texture() const -> RID { return m_main_texture; }
		void set_main_texture(RID value) { m_main_texture = value; }

		auto get_editor_camera() const noexcept { return const_cast<EditorCamera *>(&m_editor_camera); }

	public:
		virtual void process(Duration const & dt) override;
	};
}

#endif // !_ISM_EDITOR_VIEWPORT_HPP_
