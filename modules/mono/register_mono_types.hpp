#ifndef _ISM_REGISTER_MONO_TYPES_HPP_
#define _ISM_REGISTER_MONO_TYPES_HPP_

#include <core/extension/extension_interface.hpp>

namespace ism
{
	ISM_MOD_API bool initialize_mono_library(ExtensionInterface const * iface, Extension * library, ExtensionInitialization * init);
	ISM_MOD_API void initialize_mono_module(void * userdata, ExtensionInitializationLevel_ level);
	ISM_MOD_API void finalize_mono_module(void * userdata, ExtensionInitializationLevel_ level);
}

#endif // !_ISM_REGISTER_MONO_TYPES_HPP_
