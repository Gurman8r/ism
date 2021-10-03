#ifndef _ISM_EDITOR_NODE_HPP_
#define _ISM_EDITOR_NODE_HPP_

#include <scene/gui/imgui_node.hpp>

namespace ism
{
	class ISM_API EditorNode : public Node
	{
		OBJECT_COMMON(EditorNode, Node);

	public:
		EditorNode();

		~EditorNode();
	};
}

#endif // !_ISM_EDITOR_NODE_HPP_
