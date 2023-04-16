#ifndef _ISM_EXTENSION_API_DUMP_HPP_
#define _ISM_EXTENSION_API_DUMP_HPP_

// PLACEHOLDER

#include <core/extension/extension.hpp>

namespace ism
{
	class ISM_API ExtensionAPIDump final
	{
	public:
		static DICT generate_extension_api();
		static void generate_extension_json_file(Path const & path);
	};
}

#endif // !_ISM_EXTENSION_API_DUMP_HPP_
