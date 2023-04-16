#include <core/object/script_language.hpp>

// script server
namespace ism
{
	EMBED_CLASS(ScriptServer, t) {}

	ScriptServer * ScriptServer::__singleton{};

	bool ScriptServer::is_scripting_enabled() const noexcept
	{
		return m_scripting_enabled;
	}

	void ScriptServer::set_scripting_enabled(bool enabled)
	{
		if (m_scripting_enabled != enabled) { m_scripting_enabled = enabled; }
	}

	ScriptLanguage * ScriptServer::get_language(u32 index)
	{
		return m_languages[index];
	}
	
	Error_ ScriptServer::register_language(ScriptLanguage * language)
	{
		return Error_None;
	}
	
	Error_ ScriptServer::unregister_language(ScriptLanguage const * language)
	{
		return Error_None;
	}

	void ScriptServer::initialize_languages()
	{
	}

	void ScriptServer::finalize_languages()
	{
	}
}

// script
namespace ism
{
	EMBED_CLASS(Script, t) {}

	Script::Script()
	{
	}

	Script::~Script()
	{
	}

	Error_ Script::reload_from_file()
	{
		return Error_None;
	}
}

// script instance
namespace ism
{
	ScriptInstance::~ScriptInstance()
	{
	}
}

// script language
namespace ism
{
	EMBED_CLASS(ScriptLanguage, t) {}

	ScriptLanguage::ScriptLanguage()
	{
	}

	ScriptLanguage::~ScriptLanguage()
	{
	}
}

// placeholder script instance
namespace ism
{
	PlaceHolderScriptInstance::PlaceHolderScriptInstance(ScriptLanguage * language, Ref<Script> script, Object * owner)
		: m_owner{ owner }
		, m_script{ script }
		, m_language{ language }
	{
	}

	PlaceHolderScriptInstance::~PlaceHolderScriptInstance()
	{
	}

	bool PlaceHolderScriptInstance::get_properties(HashMap<StringName, PROPERTY> * out) const
	{
		return false;
	}

	bool PlaceHolderScriptInstance::get_functions(HashMap<StringName, FUNCTION> * out) const
	{
		return false;
	}
}