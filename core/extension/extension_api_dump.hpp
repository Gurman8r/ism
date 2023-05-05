#ifndef _ISM_EXTENSION_API_DUMP_HPP_
#define _ISM_EXTENSION_API_DUMP_HPP_

// PLACEHOLDER

#include <core/extension/extension.hpp>

namespace Ism
{
	class ISM_API ExtensionAPIDump final
	{
	public:
		static DictRef generate_extension_api();
		static void generate_extension_json_file(String const & path);
	};
}

#endif // !_ISM_EXTENSION_API_DUMP_HPP_
