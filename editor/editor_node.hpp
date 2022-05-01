#ifndef _ISM_EDITOR_NODE_HPP_
#define _ISM_EDITOR_NODE_HPP_

#include <editor/editor_camera.hpp>
#include <editor/editor_hierarchy.hpp>
#include <editor/editor_log.hpp>
#include <editor/editor_viewport.hpp>
#include <servers/rendering/framebuffer.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API EditorNode : public Node
	{
		OBJECT_COMMON(EditorNode, Node);

		static EditorNode * singleton;

		bool				m_show_imgui_demo{};
		bool				m_show_main_menu_bar{ true };
		EditorCamera		m_camera{};
		EditorHierarchy		m_hierarchy{};
		EditorLog			m_log{};
		EditorViewport		m_viewport{};
		Ref<Framebuffer>	m_framebuffer{};
		Ref<SceneTree>		m_scene{};

		bool	m_grid_enabled	{ true };
		Mat4	m_grid_matrix	{ Mat4::identity() };
		float_t	m_grid_size		{ 100.f };

	public:
		EditorNode();

		~EditorNode();

		FORCE_INLINE static EditorNode * get_singleton() noexcept { return singleton; }

		virtual void process(Duration const & dt) override;

		void draw_interface();

	public:
		NODISCARD auto get_camera() noexcept -> EditorCamera & { return m_camera; }
		NODISCARD auto get_camera() const noexcept -> EditorCamera const & { return m_camera; }

	public:
		HashMap<String, Ref<Image>> images{};
		HashMap<String, Ref<Texture>> textures{};
		HashMap<String, Ref<Shader>> shaders{};
		HashMap<String, Ref<Mesh>> meshes{};
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// editor node singleton
#define EDITOR (ism::EditorNode::get_singleton())

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_EDITOR_NODE_HPP_
