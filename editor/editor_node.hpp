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

	class EditorAssets;
	class EditorHierarchy;
	class EditorInspector;
	class EditorLog;
	class EditorViewport;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// main editor
	class ISM_API EditorNode : public Node
	{
		DEFINE_CLASS(EditorNode, Node);

		static EditorNode * __singleton;

		friend class EditorAssets;
		friend class EditorHierarchy;
		friend class EditorLog;
		friend class EditorInspector;
		friend class EditorViewport;

		EditorAssets		* m_assets{};
		EditorHierarchy		* m_hierarchy{};
		EditorInspector		* m_inspector{};
		EditorLog			* m_log{};
		EditorViewport		* m_viewport{};

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
		FORCE_INLINE static EditorNode * get_singleton() noexcept { return __singleton; }
#define EDITOR (Ism::EditorNode::get_singleton())

	protected:
		void _notification(Notification_ id);

	private:
		void _draw_dockspace();
		void _build_dockspace();
		void _draw_menu_bar();
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	SINGLETON_WRAPPER(EditorNode, get_editor_node);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_EDITOR_NODE_HPP_
