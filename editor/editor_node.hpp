#ifndef _ISM_EDITOR_NODE_HPP_
#define _ISM_EDITOR_NODE_HPP_

#include <scene/main/scene_tree.hpp>

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

		Ref<Image> test_image;
		Ref<ImageTexture> test_texture;

	protected:
		void _edit_node(NODE node, int32_t tree_node_flags);

		void _show_dockspace(cstring label, bool has_main_menu_bar = false);

		void _show_viewport(cstring label);
	};
}

#endif // !_ISM_EDITOR_NODE_HPP_
