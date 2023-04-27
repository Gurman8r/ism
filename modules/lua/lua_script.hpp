#ifndef _ISM_LUA_SCRIPT_HPP_
#define _ISM_LUA_SCRIPT_HPP_

#include <core/object/script.hpp>

namespace ism
{
	// lua language
	class ISM_MOD_API LuaLanguage : public ScriptLanguage
	{
		DEFINE_CLASS(LuaLanguage, ScriptLanguage);

	public:
		LuaLanguage();
		virtual ~LuaLanguage() override;

		virtual StringName get_name() const override { return "lua"; }

		virtual void initialize() override;
		virtual void finalize() override;
	};
}

namespace ism
{
	// lua script
	class ISM_MOD_API LuaScript : public Script
	{
		DEFINE_CLASS(LuaScript, Script);

	public:
		LuaScript();
		explicit LuaScript(Path const & path);
		virtual ~LuaScript() override;

		virtual bool has_source_code() const override;
		virtual String get_source_code() const override;
		virtual void set_source_code(String const & code) override;
		virtual Error_ reload(bool keep_state = false) override;
	};
}

namespace ism
{
	// lua instance
	class ISM_MOD_API LuaInstance : public ScriptInstance
	{
		DEFINE_CLASS(LuaInstance, ScriptInstance);

		Object * m_owner{};
		Ref<Script> m_script{};
		ScriptLanguage * m_language{};

	public:
		LuaInstance(ScriptLanguage * language, Ref<Script> script, Object * owner);
		virtual ~LuaInstance() override;

		virtual bool get_constants(HashMap<StringName, OBJ> * out) const override;
		virtual bool get_properties(HashMap<StringName, PROPERTY> * out) const override;
		virtual bool get_functions(HashMap<StringName, FUNCTION> * out) const override;

		virtual void notification(i32 notification) override;

		virtual bool is_placeholder() const override { return false; }
		virtual Ref<Script> get_script() const override { return m_script; }
		virtual ScriptLanguage * get_language() { return m_language; }
		virtual Object * get_owner() override { return m_owner; }
	};
}

#endif // !_ISM_LUA_SCRIPT_HPP_
