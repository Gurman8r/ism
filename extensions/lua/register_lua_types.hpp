#ifndef _ISM_REGISTER_LUA_TYPES_HPP_
#define _ISM_REGISTER_LUA_TYPES_HPP_

#include <core/extension/extension.hpp>

extern "C"
{
	namespace Ism
	{
		ISM_EXT_API bool open_lua_extension(IsmExtensionInterfacePtr iface, IsmExtensionPtr library, IsmExtensionInitializationPtr init);
		ISM_EXT_API void initialize_lua_extension(void * user, IsmExtensionInitializationLevel level);
		ISM_EXT_API void finalize_lua_extension(void * user, IsmExtensionInitializationLevel level);
	}
}

#endif // !_ISM_REGISTER_LUA_TYPES_HPP_
