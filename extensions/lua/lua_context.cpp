#include <extensions/lua/lua_context.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	LuaContext * LuaContext::__singleton{};

	Error_ LuaContext::initialize()
	{
		static const struct luaL_Reg lib[] =
		{
			{ "print", [](lua_State * L) {
				for (i32 i = 1, imax = lua_gettop(L); i <= imax; ++i) {
					PRINT(lua_tostring(L, i));
				}
				return EXIT_SUCCESS;
			} },
			{/* SENTINAL */}
		};

		if (!(m_L = luaL_newstate())) {
			PRINT_ERROR("failed creating lua state");
			return Error_Failed;
		}
		luaL_openlibs(m_L);
		lua_getglobal(m_L, "_G");
		luaL_setfuncs(m_L, lib, 0);
		lua_pop(m_L, 1);
		return Error_OK;
	}

	Error_ LuaContext::finalize()
	{
		if (m_L) { lua_close(m_L); m_L = nullptr; }
		return Error_OK;
	}

	i32 LuaContext::do_string(cstring text)
	{
		return luaL_dostring(m_L, text);
	}

	i32 LuaContext::do_string(String const & text)
	{
		return luaL_dostring(m_L, text.c_str());
	}

	i32 LuaContext::do_file(String const & path)
	{
		return luaL_dofile(m_L, path.c_str());
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}