#ifndef _ISM_REGISTER_MODULE_TYPES_HPP_
#define _ISM_REGISTER_MODULE_TYPES_HPP_

#include <core/extension/extension_interface.hpp>

namespace Ism
{
	ISM_API_FUNC(void) initialize_modules(ExtensionInitializationLevel_ level);
	ISM_API_FUNC(void) finalize_modules(ExtensionInitializationLevel_ level);
}

#endif // !_ISM_REGISTER_MODULE_TYPES_HPP_
