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

		bool				m_show_main_menu_bar{ true };
		bool				m_show_imgui_demo{ true };
		Camera				m_camera;
		EditorHierarchy		m_hierarchy;
		EditorLog			m_log;
		EditorViewport		m_viewport;
		Ref<Framebuffer>	m_framebuffer;

	public:
		EditorNode();

		~EditorNode();

		virtual void process(Duration const & dt) override;

		void draw_interface();

	public:
		HashMap<String, Ref<Image>> images{};
		HashMap<String, Ref<Texture>> textures{};
		HashMap<String, Ref<Shader>> shaders{};
		HashMap<String, Ref<Mesh>> meshes{};
		RID framebuffer{};
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// editor node singleton
#define EDITOR (ism::EditorNode::get_singleton())

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_EDITOR_NODE_HPP_
