#include <extensions/mono/csharp_script.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OBJECT_EMBED(CSharpScript, t) {}

	CSharpScript::CSharpScript() noexcept : m_language{ get_csharp_language() }
	{
	}

	CSharpScript::~CSharpScript() noexcept
	{
	}

	Error_ CSharpScript::reload(bool keep_state)
	{
		return Error_OK;
	}

	bool CSharpScript::can_instantiate() const
	{
		return false;
	}

	ScriptInstance * CSharpScript::new_instance(Object * self)
	{
		return nullptr;
	}

	PlaceholderScriptInstance * CSharpScript::new_placeholder_instance(Object * self)
	{
		return memnew(PlaceholderScriptInstance(get_csharp_language(), this, self));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OBJECT_EMBED(CSharpInstance, t) {}

	CSharpInstance::CSharpInstance(Ref<Script> const & script, Object * owner) noexcept
		: m_language{ get_csharp_language() }
		, m_script{ script }
		, m_owner{ owner }
	{
	}

	CSharpInstance::~CSharpInstance() noexcept
	{
	}

	bool CSharpInstance::get(String const & name, Var & value) const
	{
		return false;
	}

	bool CSharpInstance::set(String const & name, Var const & value)
	{
		return false;
	}

	void CSharpInstance::notification(i32 notification)
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OBJECT_EMBED(CSharpScriptFormatLoader, t) {}

	RES CSharpScriptFormatLoader::load(String const & path, Error_ * r_error)
	{
		Ref<CSharpScript> temp;
		temp.instance();
		temp->set_path(path);
		temp->reload();
		if (r_error) { *r_error = Error_OK; }
		return temp;
	}

	void CSharpScriptFormatLoader::get_recognized_extensions(Vector<String> * r_out) const
	{
		if (!r_out) { return; }
		r_out->push_back(".cs");
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}