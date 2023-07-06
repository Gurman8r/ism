#ifndef _ISM_LUA_HPP_
#define _ISM_LUA_HPP_

#include <core/object/class.hpp>

extern "C" {
#include <lua/lua.h>
#include <lua/lualib.h>
#include <lua/lauxlib.h>
}

namespace Ism
{
	class ISM_EXT_API Lua : public Object
	{
		DEFINE_CLASS(Lua, Object);

		lua_State * m_L{};

	public:
		Lua();
		~Lua();
	};
}

#endif // !_ISM_LUA_HPP_
