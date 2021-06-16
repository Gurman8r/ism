#ifndef _ISM_CONFIG_FILE_HPP_
#define _ISM_CONFIG_FILE_HPP_

#include <core/io/resource.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API ConfigFile// : public Resource
	{
		//ISM_SUPER(ConfigFile, Resource);

	public:
		virtual ~ConfigFile();

		DEFAULT_COPYABLE_MOVABLE(ConfigFile);

	public:
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_CONFIG_FILE_HPP_
