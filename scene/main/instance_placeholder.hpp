#ifndef _ISM_INSTANCE_PLACEHOLDER_HPP_
#define _ISM_INSTANCE_PLACEHOLDER_HPP_

#include <scene/main/node.hpp>

namespace Ism
{
	class ISM_API InstancePlaceholder : public Node
	{
		DEFINE_CLASS(InstancePlaceholder, Node);

	public:
		InstancePlaceholder();
		virtual ~InstancePlaceholder();
	};
}

#endif // !_ISM_INSTANCE_PLACEHOLDER_HPP_
