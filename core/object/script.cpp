#include <core/object/script.hpp>

// script server
namespace Ism
{
	EMBED_CLASS(ScriptManager, t) {}

	ScriptManager * ScriptManager::__singleton{};

	void ScriptManager::set_scripting_enabled(bool enabled)
	{
		if (m_scripting_enabled != enabled) {
			m_scripting_enabled = enabled;
		}
	}
	
	Error_ ScriptManager::register_language(ScriptLanguage * language)
	{
		auto const it{ std::find_if(m_languages.begin(), m_languages.end(), [language](auto const e) { return (e == language) || (e->get_name() == language->get_name()); })};
		if (it != m_languages.end()) { return Error_Unknown; }
		m_languages.push_back(language);
		return Error_OK;
	}
	
	Error_ ScriptManager::unregister_language(ScriptLanguage const * language)
	{
		auto const it{ std::find_if(m_languages.begin(), m_languages.end(), [language](auto const e) { return (e == language) || (e->get_name() == language->get_name()); }) };
		if (it == m_languages.end()) { return Error_Unknown; }
		m_languages.erase(it);
		return Error_OK;
	}

	void ScriptManager::initialize_languages()
	{
		for (auto const e : m_languages) {
			e->initialize();
		}
	}

	void ScriptManager::finalize_languages()
	{
		for (auto const e : m_languages) {
			e->finalize();
		}
	}
}

// script
namespace Ism
{
	EMBED_CLASS(Script, t, TypeFlags_IsAbstract) {}

	Script::Script()
	{
	}

	Script::Script(String const & path)
	{
	}

	Script::~Script()
	{
	}
}

// script instance
namespace Ism
{
	EMBED_CLASS(ScriptInstance, t, TypeFlags_IsAbstract) {}

	ScriptInstance::~ScriptInstance()
	{
	}
}

// script language
namespace Ism
{
	EMBED_CLASS(ScriptLanguage, t, TypeFlags_IsAbstract) {}

	ScriptLanguage::ScriptLanguage()
	{
	}

	ScriptLanguage::~ScriptLanguage()
	{
	}
}

// placeholder script instance
namespace Ism
{
	EMBED_CLASS(PlaceholderScriptInstance, t) {}

	PlaceholderScriptInstance::PlaceholderScriptInstance(ScriptLanguage * language, Ref<Script> script, Object * owner)
		: m_owner{ owner }
		, m_script{ script }
		, m_language{ language }
	{
	}

	PlaceholderScriptInstance::~PlaceholderScriptInstance()
	{
	}

	bool PlaceholderScriptInstance::get_constants(HashMap<String, ObjectRef> * out) const
	{
		return false;
	}

	bool PlaceholderScriptInstance::get_properties(HashMap<String, PropertyRef> * out) const
	{
		return false;
	}

	bool PlaceholderScriptInstance::get_functions(HashMap<String, FunctionRef> * out) const
	{
		return false;
	}
}