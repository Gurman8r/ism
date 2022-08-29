#ifndef _ISM_RESOURCE_SAVER_HPP_
#define _ISM_RESOURCE_SAVER_HPP_

#include <core/io/resource.hpp>

namespace ism
{
	class ISM_API ResourceSaver
	{
		static ResourceSaver * __singleton;

	public:
		ResourceSaver();
		~ResourceSaver();
	};
}

#endif // !_ISM_RESOURCE_LOADER_HPP_
