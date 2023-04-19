#ifndef _ISM_RESOURCE_LOADER_HPP_
#define _ISM_RESOURCE_LOADER_HPP_

#include <core/io/resource.hpp>

namespace ism
{
	class ISM_API ResourceFormatLoader : public Object
	{
		DEFINE_CLASS(ResourceFormatLoader, Object);

	public:
		virtual ~ResourceFormatLoader() override;
	};
}

namespace ism
{
	class ISM_API ResourceLoader
	{
		static ResourceLoader * __singleton;

	public:
		ResourceLoader();
		~ResourceLoader();
		FORCE_INLINE static ResourceLoader * get_singleton() noexcept { return __singleton; }
	};
}

#endif // !_ISM_RESOURCE_LOADER_HPP_
