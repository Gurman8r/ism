#ifndef _ISM_REGISTER_MONO_TYPES_HPP_
#define _ISM_REGISTER_MONO_TYPES_HPP_

#include <core/extension/extension.hpp>

extern "C"
{
	ISM_MOD_API bool open_mono_library(IsmExtensionInterfacePtr iface, IsmExtensionPtr library, IsmExtensionInitializationPtr init);
	ISM_MOD_API void initialize_mono_module(void * user, IsmExtensionInitializationLevel level);
	ISM_MOD_API void finalize_mono_module(void * user, IsmExtensionInitializationLevel level);
}

#endif // !_ISM_REGISTER_MONO_TYPES_HPP_
