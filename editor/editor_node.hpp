#ifndef _ISM_EDITOR_NODE_HPP_
#define _ISM_EDITOR_NODE_HPP_

#include <editor/editor_hierarchy.hpp>
#include <editor/editor_log.hpp>
#include <editor/editor_viewport.hpp>

namespace ism
{
	class ISM_API EditorNode : public Node
	{
		OBJECT_COMMON(EditorNode, Node);

		bool			m_show_main_menu_bar{ true };
		bool			m_show_imgui_demo{ true };
		EditorHierarchy	m_hierarchy;
		EditorLog		m_log;
		EditorViewport	m_viewport;

	public:
		EditorNode();

		~EditorNode();

		virtual void process(Duration const & dt) override;

	public:
		void on_gui();

		HashMap<String, Ref<Image>> images{};
		HashMap<String, Ref<Texture>> textures{};
		HashMap<String, Ref<Shader>> shaders{};
		HashMap<String, Ref<Mesh>> meshes{};
		RID color_buffer{}, depth_stencil_buffer{};
		RID framebuffer{};
	};
}

#endif // !_ISM_EDITOR_NODE_HPP_
