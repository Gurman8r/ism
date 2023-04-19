#include <modules/mono/mono_script.hpp>

namespace ism
{
	EMBED_CLASS(MonoScript, t) {}

	MonoScript::MonoScript()
	{
	}

	MonoScript::~MonoScript()
	{
	}

	Error_ MonoScript::reload_from_file()
	{
		return Error_OK;
	}

	bool MonoScript::has_source_code() const
	{
		return false;
	}

	String MonoScript::get_source_code() const
	{
		return String();
	}

	void MonoScript::set_source_code(String const & code)
	{
	}

	Error_ MonoScript::reload(bool keep_state)
	{
		return Error_();
	}
}

namespace ism
{
	MonoInstance::MonoInstance(ScriptLanguage * language, Ref<Script> script, Object * owner)
		: m_owner{ owner }
		, m_script{ script }
		, m_language{ language }
	{
	}

	MonoInstance::~MonoInstance()
	{
	}

	bool MonoInstance::get_constants(HashMap<StringName, OBJ> * out) const
	{
		return true;
	}

	bool MonoInstance::get_properties(HashMap<StringName, PROPERTY> * out) const
	{
		return true;
	}

	bool MonoInstance::get_functions(HashMap<StringName, FUNCTION> * out) const
	{
		return true;
	}

	void MonoInstance::notification(i32 notification)
	{
	}
}

namespace ism
{
	EMBED_CLASS(MonoLanguage, t) {}

	MonoLanguage::MonoLanguage()
	{
	}

	MonoLanguage::~MonoLanguage()
	{
	}

	void MonoLanguage::initialize()
	{
	}

	void MonoLanguage::finalize()
	{
	}
}