#ifndef _ISM_FILE_HPP_
#define _ISM_FILE_HPP_

#include <core/io/resource.hpp>

#include <fstream>

namespace ism
{
	// file
	class ISM_API File : public Resource
	{
		OBJECT_CLASS(File, Resource);

	protected:
		File();

	public:
		virtual ~File() override;
	};
}

#endif // !_ISM_FILE_HPP_
