#include <extensions/lua/lua.hpp>

namespace Ism
{
	EMBED_CLASS(Lua, t) {}

	Lua::Lua() : m_L{ luaL_newstate() }
	{
		if (!m_L) {
			CRASH("failed creating luaL state");
		}

		static const struct luaL_Reg lib[] =
		{
			{ "print", [](lua_State * L) {
				for (int32_t i = 1, imax = lua_gettop(L); i <= imax; ++i)
					PRINT(lua_tostring(L, i));
				return EXIT_SUCCESS;
			} },
			{ nullptr, nullptr }
		};

		luaL_openlibs(m_L);
		lua_getglobal(m_L, "_G");
		luaL_setfuncs(m_L, lib, 0);
		lua_pop(m_L, 1);
	}

	Lua::~Lua()
	{
		if (m_L) {
			lua_close(m_L);
			m_L = nullptr;
		}
	}
}