#ifndef _ISM_MISSING_NODE_HPP_
#define _ISM_MISSING_NODE_HPP_

#include <scene/main/node.hpp>

namespace Ism
{
	class ISM_API MissingNode : public Node
	{
		DEFINE_CLASS(MissingNode, Node);

	public:
		MissingNode();
		virtual ~MissingNode() override;
	};
}

#endif // !_ISM_MISSING_NODE_HPP_
