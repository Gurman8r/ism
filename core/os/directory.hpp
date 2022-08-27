#ifndef _ISM_DIRECTORY_HPP_
#define _ISM_DIRECTORY_HPP_

#include <core/io/resource.hpp>

#include <fstream>

namespace ism
{
	// directory
	class ISM_API Directory : public Resource
	{
		OBJECT_CLASS(Directory, Resource);

	protected:
		Directory();

	public:
		virtual ~Directory() override;


	};
}

#endif // !_ISM_DIRECTORY_HPP_
