#ifndef _ISM_LUA_LANGUAGE_HPP_
#define _ISM_LUA_LANGUAGE_HPP_

#include <extensions/lua/lua_context.hpp>
#include <core/object/script.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// lua language
	class ISM_EXT_API LuaLanguage : public ScriptLanguage
	{
		OBJECT_CLASS(LuaLanguage, ScriptLanguage);

		SINGLETON_CLASS(LuaLanguage);

		friend class LuaScript;
		friend class LuaInstance;

		LuaContext * m_context{};

	public:
		LuaLanguage() noexcept { SINGLETON_CTOR(); }
		virtual ~LuaLanguage() noexcept override { SINGLETON_DTOR(); }

		virtual String get_name() const override { return "lua"; }

		virtual Error_ initialize() override;
		virtual Error_ finalize() override;

		virtual void get_reserved_words(Vector<String> * words) const override;
		virtual void get_comment_delimiters(Vector<String> * delimiters) const override;
		virtual void get_string_delimiters(Vector<String> * delimiters) const override;
		virtual bool has_named_classes() const { return false; }

		virtual Script * new_scipt() override;
	};

	SINGLETON_WRAPPER(LuaLanguage, get_lua_language);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_LUA_LANGUAGE_HPP_
