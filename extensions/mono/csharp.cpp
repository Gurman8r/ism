#include <extensions/mono/csharp.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(CSharpLanguage, t) {}

	CSharpLanguage * CSharpLanguage::__singleton{};
	CSharpLanguage::CSharpLanguage() { SINGLETON_CTOR(); }
	CSharpLanguage::~CSharpLanguage() { SINGLETON_DTOR(); }

	Error_ CSharpLanguage::initialize()
	{
		if (!(m_context = get_mono())) { m_context = memnew(MonoContext); }
		m_context->initialize();
		return Error_OK;
	}

	Error_ CSharpLanguage::finalize()
	{
		m_context->finalize();
		if (m_context) { memdelete(m_context); m_context = nullptr; }
		return Error_OK;
	}

	void CSharpLanguage::get_reserved_words(Vector<String> * words) const
	{
	}

	void CSharpLanguage::get_comment_delimiters(Vector<String> * delimiters) const
	{
	}

	void CSharpLanguage::get_string_delimiters(Vector<String> * delimiters) const
	{
	}

	Script * CSharpLanguage::new_scipt()
	{
		return memnew(CSharpScript);
	}

	void CSharpLanguage::reload_assemblies(bool soft_reload)
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(CSharpScript, t) {}

	CSharpScript::CSharpScript() : Script{}, m_language{ get_csharp_language() }
	{
	}

	CSharpScript::~CSharpScript()
	{
	}

	bool CSharpScript::can_instantiate() const
	{
		return false;
	}

	Ref<ScriptInstance> CSharpScript::instance_create(Object * self)
	{
		return nullptr;
	}

	Error_ CSharpScript::reload(bool keep_state)
	{
		return Error_OK;
	}

	bool CSharpScript::has_method(String const & method) const
	{
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(CSharpInstance, t) {}

	CSharpInstance::CSharpInstance(Ref<Script> script, Object * owner)
		: m_script	{ VALIDATE(script) }
		, m_owner	{ owner }
		, m_language{ script->get_language() }
	{
	}

	CSharpInstance::~CSharpInstance()
	{
	}

	bool CSharpInstance::get(String const & name, ObjectRef & value) const
	{
		return false;
	}

	bool CSharpInstance::set(String const & name, ObjectRef const & value)
	{
		return false;
	}

	void CSharpInstance::notification(i32 notification)
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(CSharpScriptFormatLoader, t) {}

	RES CSharpScriptFormatLoader::load(String const & path, Error_ * error)
	{
		Ref<CSharpScript> temp{ memnew(CSharpScript) };
		temp->set_path(path);
		temp->reload();
		if (error) { *error = Error_OK; }
		return temp;
	}

	void CSharpScriptFormatLoader::get_recognized_extensions(Vector<String> * out) const
	{
		if (!out) { return; }
		out->push_back(".cs");
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}