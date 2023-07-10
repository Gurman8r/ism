#if TOOLS_ENABLED
#ifndef _ISM_EDITOR_NODE_HPP_
#define _ISM_EDITOR_NODE_HPP_

#include <scene/main/window.hpp>
#include <scene/main/scene_tree.hpp>

#include <scene/resources/font.hpp>
#include <scene/resources/material.hpp>
#include <scene/resources/mesh.hpp>
#include <scene/resources/text_file.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class EditorPanel;
	class EditorAssets;
	class EditorHierarchy;
	class EditorInspector;
	class EditorLog;
	class EditorTerminal;
	class EditorViewport;

	// main editor
	class ISM_API EditorNode : public Node
	{
		OBJECT_CLASS(EditorNode, Node);

		SINGLETON_CLASS(EditorNode);

		friend class EditorPanel;
		friend class EditorAssets;
		friend class EditorHierarchy;
		friend class EditorInspector;
		friend class EditorLog;
		friend class EditorTerminal;
		friend class EditorViewport;

		Vector<EditorPanel *>	m_panels{};
		EditorAssets *			m_assets{};
		EditorHierarchy *		m_hierarchy{};
		EditorInspector *		m_inspector{};
		EditorLog *				m_log{};
		EditorTerminal *		m_terminal{};
		EditorViewport *		m_viewport{};

		template <class T> void init_panel(T *& p, bool start_open = true) noexcept {
			p = (T *)m_panels.emplace_back(memnew(T));
			p->set_open(start_open);
		}

		bool m_show_main_menu_bar{ true };
		bool m_show_imgui_demo{};

		Ref<SceneTree>				m_active_scene{};
		Map<String, Ref<Image>>		m_images{};
		Map<String, Ref<Material>>	m_materials{};
		Map<String, Ref<Mesh>>		m_meshes{};
		Map<String, Ref<Texture>>	m_textures{};
		Map<String, Ref<Shader>>	m_shaders{};

	public:
		EditorNode();
		virtual ~EditorNode() override;

	protected:
		void _notification(Notification_ id);

	private:
		void _draw_dockspace();
		void _build_dockspace();
		void _draw_menu_bar();
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	SINGLETON_WRAPPER(EditorNode, editor_node);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_EDITOR_NODE_HPP_
#endif // TOOLS_ENABLED
