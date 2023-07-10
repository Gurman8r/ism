#include <extensions/lua/lua_language.hpp>
#include <extensions/lua/lua_script.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OBJECT_EMBED(LuaLanguage, t) {}

	SINGLETON_EMBED(LuaLanguage);

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
}