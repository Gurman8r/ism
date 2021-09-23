#ifndef _ISM_TEXT_FILE_HPP_
#define _ISM_TEXT_FILE_HPP_

#include <core/io/resource.hpp>

namespace ism
{
	API_CLASS(TextFile) : public Resource
	{
		OBJ_COMMON(TextFile, Resource);

	public:
		virtual ~TextFile();

		TextFile();

	private:

	};
}

#endif // !_ISM_TEXT_FILE_HPP_
