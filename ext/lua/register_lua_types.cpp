#include <ext/lua/register_lua_types.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

using namespace Ism;

bool open_lua_library(IsmExtensionInterfacePtr iface, IsmExtensionPtr library, IsmExtensionInitializationPtr initialization)
{
	if (!iface || !library || !initialization) { return false; }
	PRINT_LINE("open lua module");
	auto init{ (ExtensionInitialization *)initialization };
	init->minimum_level = ExtensionInitializationLevel_Scene;
	init->user = nullptr;
	init->initialize = &initialize_lua_module;
	init->finalize = &finalize_lua_module;
	return true;
}

void initialize_lua_module(void * user, IsmExtensionInitializationLevel level)
{
	if (level != ExtensionInitializationLevel_Scene) { return; }
	PRINT_LINE("initialize lua module");
}

void finalize_lua_module(void * user, IsmExtensionInitializationLevel level)
{
	if (level != ExtensionInitializationLevel_Scene) { return; }
	PRINT_LINE("finalize lua module");
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */