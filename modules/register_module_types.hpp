#ifndef _ISM_REGISTER_MODULE_TYPES_HPP_
#define _ISM_REGISTER_MODULE_TYPES_HPP_

#include <core/extension/native_interface.hpp>

namespace ism
{
	enum ModuleInitializationLevel_
	{
		ModuleInitializationLevel_Core = NativeInitializationLevel_Core,
		ModuleInitializationLevel_Servers = NativeInitializationLevel_Servers,
		ModuleInitializationLevel_Scene = NativeInitializationLevel_Scene,
		ModuleInitializationLevel_Editor = NativeInitializationLevel_Editor,
	};

	ISM_API_FUNC(void) initialize_modules(ModuleInitializationLevel_ level);
	ISM_API_FUNC(void) uninitialize_modules(ModuleInitializationLevel_ level);
}

#endif // !_ISM_REGISTER_MODULE_TYPES_HPP_
