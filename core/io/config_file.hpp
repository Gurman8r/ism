#ifndef _ISM_CONFIG_FILE_HPP_
#define _ISM_CONFIG_FILE_HPP_

#include <core/io/resource.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API ConfigFile : public Resource
	{
		ISM_SUPER_CLASS(ConfigFile, Resource);

	public:
		virtual ~ConfigFile() override;

		DEFAULT_COPY_AND_MOVE_CONSTRUCTABLE(ConfigFile);

	public:
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_CONFIG_FILE_HPP_
