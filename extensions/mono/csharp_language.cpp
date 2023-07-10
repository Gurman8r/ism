#include <extensions/mono/csharp_language.hpp>
#include <extensions/mono/csharp_script.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OBJECT_EMBED(CSharpLanguage, t) {}

	SINGLETON_EMBED(CSharpLanguage);

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

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Script * CSharpLanguage::new_scipt()
	{
		return memnew(CSharpScript);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void CSharpLanguage::reload_assemblies(bool soft_reload)
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}