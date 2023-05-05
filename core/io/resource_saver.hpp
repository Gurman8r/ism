#ifndef _ISM_RESOURCE_SAVER_HPP_
#define _ISM_RESOURCE_SAVER_HPP_

#include <core/io/resource.hpp>

namespace Ism
{
	class ISM_API ResourceSaver
	{
		static ResourceSaver * __singleton;

	public:
		ResourceSaver();
		~ResourceSaver();
		FORCE_INLINE static ResourceSaver * get_singleton() noexcept { return __singleton; }

	public:
		Error_ save(RES const & res, String const & path);
	};

	SINGLETON_WRAPPER(ResourceSaver, get_resource_saver);
}

#endif // !_ISM_RESOURCE_LOADER_HPP_
