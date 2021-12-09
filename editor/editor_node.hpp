#ifndef _ISM_EDITOR_NODE_HPP_
#define _ISM_EDITOR_NODE_HPP_

#include <scene/main/scene_tree.hpp>

#include <scene/resources/mesh.hpp>
#include <scene/resources/shader.hpp>
#include <scene/resources/texture.hpp>

namespace ism
{
	class ISM_API EditorNode : public Node
	{
		OBJECT_COMMON(EditorNode, Node);

	public:
		EditorNode();

		~EditorNode();

		virtual void process(Duration const & dt) override;

		HashMap<String, Ref<Image>> images{};
		HashMap<String, Ref<Texture>> textures{};
		HashMap<String, Ref<Shader>> shaders{};
		HashMap<String, Ref<Mesh>> meshes{};
		RID color_buffer{}, depth_stencil_buffer{};
		RID framebuffer{};

	protected:
		void _show_dockspace(cstring label, bool has_main_menu_bar = false);

		void _show_viewport(cstring label);
	};
}

#endif // !_ISM_EDITOR_NODE_HPP_
