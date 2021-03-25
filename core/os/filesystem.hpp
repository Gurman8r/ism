#ifndef _ISM_FILESYSTEM_HPP_
#define _ISM_FILESYSTEM_HPP_

#include <core/string/path.hpp>

#include <fstream>

namespace ISM
{
	class ISM_API FileSystem
	{
	public:
		NODISCARD static Path get_current_path();

		NODISCARD static void set_current_path(Path const & value);
	};
}

#endif // !_ISM_FILESYSTEM_HPP_
