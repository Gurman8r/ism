#ifndef _ISM_REGISTER_MODULE_TYPES_HPP_
#define _ISM_REGISTER_MODULE_TYPES_HPP_

#include <core/extension/native_interface.hpp>

namespace ism
{
	enum ModuleInitializationLevel_
	{
		ModuleInitializationLevel_Core = InitializationLevel_Core,
		ModuleInitializationLevel_Servers = InitializationLevel_Servers,
		ModuleInitializationLevel_Scene = InitializationLevel_Scene,
		ModuleInitializationLevel_Editor = InitializationLevel_Editor,
	};

	ISM_API_FUNC(void) initialize_modules(ModuleInitializationLevel_ level);
	ISM_API_FUNC(void) uninitialize_modules(ModuleInitializationLevel_ level);
}

#endif // !_ISM_REGISTER_MODULE_TYPES_HPP_
