#ifndef _ISM_LUA_CONTEXT_HPP_
#define _ISM_LUA_CONTEXT_HPP_

#include <core/object/class.hpp>

extern "C" {
#include <lua/lua.h>
#include <lua/lualib.h>
#include <lua/lauxlib.h>
}

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class LuaLanguage;
	class LuaScript;
	class LuaInstance;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// lua context
	class ISM_EXT_API LuaContext
	{
		SINGLETON_CLASS(LuaContext);

		lua_State * m_L{};

	public:
		LuaContext() noexcept { SINGLETON_CTOR(); }
		~LuaContext() noexcept { SINGLETON_DTOR(); }
		NODISCARD lua_State * operator * () const { return m_L; }

		Error_ initialize();
		Error_ finalize();

		i32 do_string(cstring text);
		i32 do_string(String const & text);
		i32 do_file(String const & path);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	SINGLETON_WRAPPER(LuaContext, get_lua);

	NODISCARD inline lua_State * get_luaL() noexcept { return (**get_lua()); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_LUA_CONTEXT_HPP_
