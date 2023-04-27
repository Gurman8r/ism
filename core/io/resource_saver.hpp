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
		FORCE_INLINE static ResourceSaver * get_singleton() noexcept { return __singleton; }
#define RESOURCE_SAVER (ism::ResourceSaver::get_singleton())

	public:
		Error_ save(RES const & res, Path const & path);
	};
}

#endif // !_ISM_RESOURCE_LOADER_HPP_
