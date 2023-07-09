#include <core/object/script.hpp>

// script server
namespace Ism
{
	EMBED_CLASS(ScriptServer, t) {}

	ScriptServer * ScriptServer::__singleton{};

	ScriptServer::ScriptServer() noexcept
	{
		SINGLETON_CTOR();
		m_scripting_enabled = true;
		m_reload_scripts_on_save = false;
		m_languages_finalized = false;
	}

	ScriptServer::~ScriptServer() noexcept
	{
		SINGLETON_DTOR();
		ASSERT(m_languages.empty());
	}

	bool ScriptServer::is_scripting_enabled() const noexcept
	{
		return m_scripting_enabled;
	}

	void ScriptServer::set_scripting_enabled(bool enabled)
	{
		if (m_scripting_enabled != enabled) {
			m_scripting_enabled = enabled;
		}
	}

	size_t ScriptServer::get_language_count() const noexcept
	{
		return m_languages.size();
	}

	ScriptLanguage * ScriptServer::get_language(size_t index) const noexcept
	{
		ASSERT(index < get_language_count());
		return m_languages[index];
	}
	
	ScriptLanguage * ScriptServer::get_language(String const & name) const noexcept
	{
		if (auto const it{ m_languages.find_if([id = name.hash_code()](auto e) noexcept {
			return e && e->get_name().hash_code() == id;
		}) }; it != m_languages.end()) {
			return *it;
		}
		else {
			return nullptr;
		}
	}

	Error_ ScriptServer::register_language(ScriptLanguage * language)
	{
		if (!language) {
			return Error_Failed;
		}
		else if (get_language_count() >= MAX_LANGUAGES) {
			return Error_Failed;
		}
		else if (m_languages.contains(language)) {
			return Error_Failed;
		}
		else {
			return m_languages.push_back(language), Error_OK;
		}
	}
	
	Error_ ScriptServer::unregister_language(ScriptLanguage * language)
	{
		if (!language) {
			return Error_Failed;
		}
		else if (auto const it{ m_languages.find(language) }; it == m_languages.end()) {
			return Error_Failed;
		}
		else {
			return m_languages.erase(it), Error_OK;
		}
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

	bool ScriptServer::are_languages_finalized() const noexcept
	{
		return m_languages_finalized;
	}
}

// script language
namespace Ism
{
	EMBED_CLASS(ScriptLanguage, t, TypeFlags_IsAbstract) {}

	ScriptLanguage::ScriptLanguage() {}

	ScriptLanguage::~ScriptLanguage() {}

	void ScriptLanguage::get_core_type_words(Vector<String> * words) const
	{
	}
}

// script
namespace Ism
{
	EMBED_CLASS(Script, t, TypeFlags_IsAbstract) {}

	Script::Script() {}

	Script::~Script() {}
}

// script instance
namespace Ism
{
	EMBED_CLASS(ScriptInstance, t, TypeFlags_IsAbstract) {}

	ScriptInstance::~ScriptInstance() {}
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

	PlaceholderScriptInstance::~PlaceholderScriptInstance() {}
}