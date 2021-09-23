#ifndef _ISM_CONFIG_FILE_HPP_
#define _ISM_CONFIG_FILE_HPP_

#include <core/io/resource.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API ConfigFile : public Resource
	{
		OBJ_CLASS(ConfigFile, Resource);

	public:
		virtual ~ConfigFile();

		ConfigFile() : Resource{} {}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_CONFIG_FILE_HPP_
