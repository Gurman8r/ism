#ifndef _ISM_GUI_NODE_HPP_
#define _ISM_GUI_NODE_HPP_

#include <scene/main/node.hpp>

namespace ism
{
	class ISM_API GuiNode : public Node
	{
		OBJECT_COMMON(GuiNode, Node);

	public:
		virtual ~GuiNode();
	};
}

#endif // !_ISM_GUI_NODE_HPP_
