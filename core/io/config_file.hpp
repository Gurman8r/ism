#ifndef _ISM_CONFIG_FILE_HPP_
#define _ISM_CONFIG_FILE_HPP_

#include <core/io/resource.hpp>

namespace ism
{
	class ISM_API ConfigFile : public Resource
	{
		ISM_SUPER(ConfigFile, Resource);

	public:
		ConfigFile() {}

		virtual ~ConfigFile() override;
	};
}

#endif // !_ISM_CONFIG_FILE_HPP_
