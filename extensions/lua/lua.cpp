#include <extensions/lua/lua.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(LuaLanguage, t) {}

	LuaLanguage * LuaLanguage::__singleton{};

	Error_ LuaLanguage::initialize()
	{
		if (!(m_context = get_lua())) { m_context = memnew(LuaContext); }

		m_context->initialize();

		return Error_OK;
	}

	Error_ LuaLanguage::finalize()
	{
		m_context->finalize();

		if (m_context) { memdelete(m_context); m_context = nullptr; }

		return Error_OK;
	}

	void LuaLanguage::get_reserved_words(Vector<String> * words) const
	{
	}

	void LuaLanguage::get_comment_delimiters(Vector<String> * delimiters) const
	{
	}

	void LuaLanguage::get_string_delimiters(Vector<String> * delimiters) const
	{
	}

	Script * LuaLanguage::new_scipt()
	{
		return memnew(LuaScript);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(LuaScript, t) {}

	LuaScript::LuaScript() : Script{}, m_language { get_lua_language() }
	{
	}

	LuaScript::~LuaScript()
	{
	}

	bool LuaScript::can_instantiate() const
	{
		return false;
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

	bool LuaInstance::get(String const & name, ObjectRef & value) const
	{
		return false;
	}

	bool LuaInstance::set(String const & name, ObjectRef const & value)
	{
		return false;
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