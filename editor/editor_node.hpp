#ifndef _ISM_EDITOR_NODE_HPP_
#define _ISM_EDITOR_NODE_HPP_

#include <scene/gui/imgui_node.hpp>

namespace ism
{
	class ISM_API EditorNode : public Node
	{
		OBJECT_COMMON(EditorNode, Node);

		Ref<ImGuiNode> m_gui{};

	public:
		EditorNode();

		~EditorNode();

		virtual void initialize() override;

		virtual void finalize() override;

		virtual void step(Duration const & delta_time) override;
	};
}

#endif // !_ISM_EDITOR_NODE_HPP_
