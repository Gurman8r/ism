#ifndef _ISM_RESOURCE_IMPORTER_HPP_
#define _ISM_RESOURCE_IMPORTER_HPP_

#include <core/io/resource.hpp>

namespace ism
{
	class ISM_API ResourceImporter
	{
		static ResourceImporter * __singleton;

	public:
		ResourceImporter();
		~ResourceImporter();
	};
}

#endif // !_ISM_RESOURCE_IMPORTER_HPP_
