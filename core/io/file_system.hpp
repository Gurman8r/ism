#ifndef _ISM_FILE_SYSTEM_HPP_
#define _ISM_FILE_SYSTEM_HPP_

#include <core/string/string_utility.hpp>
#include <core/templates/buffer.hpp>
#include <core/templates/optional.hpp>

#include <fstream>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API FileSystem final
	{
	public:
		static bool get_file_contents(DynamicBuffer & out, Path const & path);

		NODISCARD static Optional<DynamicBuffer> get_file_contents(Path const & path);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ALIAS(FS) FileSystem;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_FILE_SYSTEM_HPP_
