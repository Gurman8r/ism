#include <modules/lua/lua_script.hpp>

namespace ism
{
	EMBED_CLASS(LuaLanguage, t) {}

	LuaLanguage::LuaLanguage()
	{
	}

	LuaLanguage::~LuaLanguage()
	{
	}

	void LuaLanguage::initialize()
	{
	}

	void LuaLanguage::finalize()
	{
	}
}

namespace ism
{
	EMBED_CLASS(LuaScript, t) {}

	LuaScript::LuaScript()
	{
	}

	LuaScript::LuaScript(Path const & path)
	{
	}

	LuaScript::~LuaScript()
	{
	}

	bool LuaScript::has_source_code() const
	{
		return false;
	}

	String LuaScript::get_source_code() const
	{
		return String();
	}

	void LuaScript::set_source_code(String const & code)
	{
	}

	Error_ LuaScript::reload(bool keep_state)
	{
		return Error_();
	}
}

namespace ism
{
	EMBED_CLASS(LuaInstance, t) {}

	LuaInstance::LuaInstance(ScriptLanguage * language, Ref<Script> script, Object * owner)
		: m_owner{ owner }
		, m_script{ script }
		, m_language{ language }
	{
	}

	LuaInstance::~LuaInstance()
	{
	}

	bool LuaInstance::get_constants(HashMap<StringName, OBJ> * out) const
	{
		return true;
	}

	bool LuaInstance::get_properties(HashMap<StringName, PROPERTY> * out) const
	{
		return true;
	}

	bool LuaInstance::get_functions(HashMap<StringName, FUNCTION> * out) const
	{
		return true;
	}

	void LuaInstance::notification(i32 notification)
	{
	}
}