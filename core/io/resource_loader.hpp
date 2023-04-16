#ifndef _ISM_RESOURCE_LOADER_HPP_
#define _ISM_RESOURCE_LOADER_HPP_

#include <core/io/resource.hpp>

namespace ism
{
	class ResourceFormatLoader : public Object
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
	};
}

#endif // !_ISM_RESOURCE_LOADER_HPP_
