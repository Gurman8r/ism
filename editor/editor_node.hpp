#ifndef _ISM_EDITOR_NODE_HPP_
#define _ISM_EDITOR_NODE_HPP_

#include <scene/resources/font.hpp>
#include <scene/resources/material.hpp>
#include <scene/resources/mesh.hpp>
#include <scene/resources/text_file.hpp>

#include <editor/editor_hierarchy.hpp>
#include <editor/editor_log.hpp>
#include <editor/editor_viewport.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API EditorNode : public Node
	{
		OBJECT_COMMON(EditorNode, Node);

		static EditorNode * singleton;

		friend class EditorHierarchy;
		friend class EditorLog;
		friend class EditorViewport;

		bool				m_show_imgui_demo{};
		bool				m_show_main_menu_bar{ true };
		EditorHierarchy		m_hierarchy{};
		EditorLog			m_editor_log{};
		EditorViewport		m_editor_view{};
		Ref<SceneTree>		m_active_scene{};

		HashMap<String, Ref<Image>> m_images{};
		HashMap<String, Ref<Texture>> m_textures{};
		HashMap<String, Ref<Shader>> m_shaders{};
		HashMap<String, Ref<Mesh>> m_meshes{};

	public:
		EditorNode();

		~EditorNode();

		FORCE_INLINE static EditorNode * get_singleton() noexcept { return singleton; }

		virtual void handle_event(Event const & event) override;

		virtual void process(Duration const dt) override;

	public:
		NODISCARD auto get_hierarchy() const noexcept { return const_cast<EditorHierarchy *>(&m_hierarchy); }
		NODISCARD auto get_log() const noexcept { return const_cast<EditorLog *>(&m_editor_log); }
		NODISCARD auto get_viewport() const noexcept { return const_cast<EditorViewport *>(&m_editor_view); }

	protected:
		void _draw_dockspace();

		void _build_dockspace();

		void _draw_menu_bar();
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_EDITOR_NODE_HPP_
