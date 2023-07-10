#include <extensions/lua/lua_script.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OBJECT_EMBED(LuaScript, t) {}

	LuaScript::LuaScript() noexcept : m_language{ get_lua_language() }
	{
	}

	LuaScript::~LuaScript() noexcept
	{
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

	bool LuaScript::can_instantiate() const
	{
		return false;
	}

	ScriptInstance * LuaScript::new_instance(Object * self)
	{
		return nullptr;
	}

	PlaceholderScriptInstance * LuaScript::new_placeholder_instance(Object * self)
	{
		return memnew(PlaceholderScriptInstance(get_lua_language(), this, self));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OBJECT_EMBED(LuaInstance, t) {}

	LuaInstance::LuaInstance(Ref<Script> const & script, Object * owner) noexcept
		: m_language{ get_lua_language() }
		, m_script{ script }
		, m_owner{ owner }
	{
	}

	LuaInstance::~LuaInstance() noexcept
	{
	}

	bool LuaInstance::get(String const & name, Var & value) const
	{
		return false;
	}

	bool LuaInstance::set(String const & name, Var const & value)
	{
		return false;
	}

	void LuaInstance::notification(i32 notification)
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OBJECT_EMBED(LuaScriptFormatLoader, t) {}

	RES LuaScriptFormatLoader::load(String const & path, Error_ * error)
	{
		Ref<LuaScript> temp;
		temp.instance();
		temp->set_path(path);
		temp->reload();
		if (error) { *error = Error_OK; }
		return temp;
	}

	void LuaScriptFormatLoader::get_recognized_extensions(Vector<String> * r_out) const
	{
		if (!r_out) { return; }
		r_out->push_back(".lua");
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}