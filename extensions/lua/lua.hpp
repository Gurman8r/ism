#ifndef _ISM_LUA_HPP_
#define _ISM_LUA_HPP_

#include <core/object/script.hpp>
#include <core/io/resource_loader.hpp>
#include <core/io/resource_saver.hpp>
#include <extensions/lua/lua_context.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class LuaScript;
	class LuaInstance;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// lua language
	class ISM_EXT_API LuaLanguage : public ScriptLanguage
	{
		OBJECT_CLASS(LuaLanguage, ScriptLanguage);

		static LuaLanguage * __singleton;

		friend class LuaScript;
		friend class LuaInstance;

		LuaContext * m_context{};

	public:
		LuaLanguage()noexcept { SINGLETON_CTOR(); }
		virtual ~LuaLanguage() noexcept override { SINGLETON_DTOR(); }
		SINGLETON_GETTER(LuaLanguage);

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

	// lua script
	class ISM_EXT_API LuaScript : public Script
	{
		OBJECT_CLASS(LuaScript, Script);

		ScriptLanguage * const m_language{};
		String m_source{};
		mutable bool m_changed{};

	public:
		LuaScript();
		virtual ~LuaScript() override;

		virtual bool can_instantiate() const override;
		virtual Ref<ScriptInstance> instance_create(Object * self) override;

		virtual bool has_source_code() const override { return !m_source.empty(); }
		virtual String get_source_code() const override { return m_source; }
		virtual void set_source_code(String const & value) override { if (m_source != value) { m_source = value; m_changed = true; } }
		virtual Error_ reload(bool keep_state = false) override;

		virtual bool has_method(String const & method) const override;

		virtual bool is_tool() const override { return false; }
		virtual bool is_valid() const override { return false; }

		virtual ScriptLanguage * get_language() const override { return m_language; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// lua instance
	class ISM_EXT_API LuaInstance : public ScriptInstance
	{
		OBJECT_CLASS(LuaInstance, ScriptInstance);

		ScriptLanguage * const m_language{};
		Ref<Script> m_script{};
		Object * m_owner{};

	public:
		LuaInstance(Ref<Script> script, Object * owner);
		virtual ~LuaInstance() override;

		virtual bool is_placeholder() const override { return false; }
		virtual Ref<Script> get_script() const override { return m_script; }
		virtual ScriptLanguage * get_language() { return m_language; }
		virtual Object * get_owner() override { return m_owner; }

		virtual bool get(String const & name, ObjectRef & value) const override;
		virtual bool set(String const & name, ObjectRef const & value) override;

		virtual bool get_constants(Vector<ObjectRef> * out) const override { return true; }
		virtual bool get_properties(Vector<PropertyRef> * out) const override { return true; }
		virtual bool get_functions(Vector<FunctionRef> * out) const override { return true; }

		virtual void notification(i32 notification) override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// lua script loader
	class ISM_EXT_API LuaScriptFormatLoader : public ResourceFormatLoader {
		OBJECT_CLASS(LuaScriptFormatLoader, ResourceFormatLoader);
	public:
		virtual RES load(String const & path, Error_ * error = nullptr) override;
		virtual void get_recognized_extensions(Vector<String> * out) const override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_LUA_HPP_
