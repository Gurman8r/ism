#include <extensions/lua/register_lua_types.hpp>
#include <extensions/lua/lua.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace Ism
{
	static Lua * lua_language;

	bool Ism::open_lua_extension(IsmExtensionInterfacePtr iface, IsmExtensionPtr library, IsmExtensionInitializationPtr initialization)
	{
		if (!iface || !library || !initialization) { return false; }
		PRINT_INFO("opening lua");
		auto init{ (ExtensionInitialization *)initialization };
		init->minimum_level = ExtensionInitializationLevel_Scene;
		init->initialize = &initialize_lua_extension;
		init->finalize = &finalize_lua_extension;
		init->user = nullptr;
		return true;
	}

	void Ism::initialize_lua_extension(void * user, IsmExtensionInitializationLevel level)
	{
		if (level != ExtensionInitializationLevel_Scene) { return; }
		PRINT_INFO("initializing lua");
		lua_language = memnew(Lua); /* get_scr()->register_language(lua_language); */
	}

	void Ism::finalize_lua_extension(void * user, IsmExtensionInitializationLevel level)
	{
		if (level != ExtensionInitializationLevel_Scene) { return; }
		PRINT_INFO("finalizing lua");
		/* get_scr()->unregister_language(lua_language); */ memdelete(lua_language);
		
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */