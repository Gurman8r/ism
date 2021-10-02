#ifndef _ISM_EDITOR_NODE_HPP_
#define _ISM_EDITOR_NODE_HPP_

#include <scene/main/node.hpp>

struct ImGuiContext;

namespace ism
{
	class ISM_API EditorNode : public Node
	{
		OBJECT_COMMON(EditorNode, Node);

		ImGuiContext * m_imgui_context{};

	public:
		EditorNode();

		~EditorNode();
	};
}

#endif // !_ISM_EDITOR_NODE_HPP_
