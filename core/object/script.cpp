#include <core/object/script.hpp>

// script server
namespace ism
{
	EMBED_CLASS(ScriptServer, t) {}

	ScriptServer * ScriptServer::__singleton{};

	void ScriptServer::set_scripting_enabled(bool enabled)
	{
		if (m_scripting_enabled != enabled) {
			m_scripting_enabled = enabled;
		}
	}
	
	Error_ ScriptServer::register_language(ScriptLanguage * language)
	{
		auto const it{ std::find_if(m_languages.begin(), m_languages.end(), [language](auto const e) { return (e == language) || (e->get_name() == language->get_name()); })};
		if (it != m_languages.end()) { return Error_Unknown; }
		m_languages.push_back(language);
		return Error_OK;
	}
	
	Error_ ScriptServer::unregister_language(ScriptLanguage const * language)
	{
		auto const it{ std::find_if(m_languages.begin(), m_languages.end(), [language](auto const e) { return (e == language) || (e->get_name() == language->get_name()); }) };
		if (it == m_languages.end()) { return Error_Unknown; }
		m_languages.erase(it);
		return Error_OK;
	}

	void ScriptServer::initialize_languages()
	{
		for (auto const e : m_languages) {
			e->initialize();
		}
	}

	void ScriptServer::finalize_languages()
	{
		for (auto const e : m_languages) {
			e->finalize();
		}
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
		return Error_OK;
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

	bool PlaceHolderScriptInstance::get_constants(HashMap<StringName, OBJ> * out) const
	{
		return false;
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