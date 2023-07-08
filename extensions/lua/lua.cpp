#include <extensions/lua/lua.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Lua * Lua::__singleton{};

	Error_ Lua::initialize()
	{
		static const struct luaL_Reg lib[] =
		{
			{ "print", [](lua_State * L) {
				for (int32_t i = 1, imax = lua_gettop(L); i <= imax; ++i)
					PRINT(lua_tostring(L, i));
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

	Error_ Lua::finalize()
	{
		if (m_L) { lua_close(m_L); m_L = nullptr; }
		return Error_OK;
	}

	i32 Lua::do_string(cstring text)
	{
		return luaL_dostring(m_L, text);
	}

	i32 Lua::do_string(String const & text)
	{
		return luaL_dostring(m_L, text.c_str());
	}

	i32 Lua::do_file(String const & path)
	{
		return luaL_dofile(m_L, path.c_str());
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(LuaLanguage, t) {}

	LuaLanguage * LuaLanguage::__singleton{};

	Error_ LuaLanguage::initialize()
	{
		if (!(m_lua = get_lua())) { m_lua = memnew(Lua); }

		m_lua->initialize();

		return Error_OK;
	}

	Error_ LuaLanguage::finalize()
	{
		m_lua->finalize();

		if (m_lua) { memdelete(m_lua); m_lua = nullptr; }

		return Error_OK;
	}

	Script * LuaLanguage::new_scipt()
	{
		return memnew(LuaScript);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(LuaScript, t) {}

	LuaScript::LuaScript() : m_language{ get_lua_language() }
	{
	}

	LuaScript::LuaScript(String const & path)
	{
	}

	LuaScript::~LuaScript()
	{
	}

	Ref<ScriptInstance> LuaScript::instance_create(Object * self)
	{
		return nullptr;
	}

	Error_ LuaScript::reload(bool keep_state)
	{
		if (m_source.empty()) {
			return Error_Failed;
		}

		if (i32 const r{ get_lua()->do_string(m_source) }; r != LUA_OK) {
			return Error_Failed;
		}

		return Error_OK;
	}

	bool LuaScript::has_method(String const & method) const
	{
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(LuaInstance, t) {}

	LuaInstance::LuaInstance(Ref<Script> script, Object * owner)
		: m_script	{ VALIDATE(script) }
		, m_owner	{ owner }
		, m_language{ script->get_language() }
	{
	}

	LuaInstance::~LuaInstance()
	{
	}

	void LuaInstance::notification(i32 notification)
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(LuaScriptFormatLoader, t) {}

	RES LuaScriptFormatLoader::load(String const & path, Error_ * error)
	{
		Ref<LuaScript> temp;
		temp.instance();
		temp->set_path(path);
		temp->reload();
		if (error) { *error = Error_OK; }
		return temp;
	}

	void LuaScriptFormatLoader::get_recognized_extensions(Vector<String> * out) const
	{
		if (!out) { return; }
		out->push_back(".lua");
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}