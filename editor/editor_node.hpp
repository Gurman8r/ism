#ifndef _ISM_EDITOR_NODE_HPP_
#define _ISM_EDITOR_NODE_HPP_

#include <scene/main/scene_tree.hpp>

namespace ism
{
	class ISM_API EditorNode : public Node
	{
		OBJECT_COMMON(EditorNode, Node);

	public:
		EditorNode();

		~EditorNode();

		virtual void process(Duration const & dt) override;
	};
}

#endif // !_ISM_EDITOR_NODE_HPP_
