#ifndef _ISM_RESOURCE_PRELOADER_HPP_
#define _ISM_RESOURCE_PRELOADER_HPP_

#include <scene/main/node.hpp>

namespace Ism
{
	class ISM_API ResourcePreloader : public Node
	{
		DEFINE_CLASS(ResourcePreloader, Node);

	public:
		ResourcePreloader();
		virtual ~ResourcePreloader() override;
	};
}

#endif // !_ISM_RESOURCE_PRELOADER_HPP_
