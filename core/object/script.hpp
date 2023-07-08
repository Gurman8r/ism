#ifndef _ISM_SCRIPT_HPP_
#define _ISM_SCRIPT_HPP_

#include <core/io/resource.hpp>

// script server
namespace Ism
{
	class Script;
	class ScriptInstance;
	class ScriptLanguage;
	class PlaceholderScriptInstance;

	class ISM_API ScriptManager : public Object
	{
		DEFINE_CLASS(ScriptManager, Object);

		static ScriptManager * __singleton;

		Vector<ScriptLanguage *> m_languages{};
		bool m_scripting_enabled{ true };
		bool m_reload_scripts_on_save{};
		bool m_languages_finalized{};

	public:
		ScriptManager() noexcept { SINGLETON_CTOR(__singleton, this); }
		virtual ~ScriptManager() noexcept override { SINGLETON_DTOR(__singleton, this); }
		SINGLETON_GETTER(ScriptManager, __singleton);

		NODISCARD bool is_scripting_enabled() const noexcept { return m_scripting_enabled; }
		void set_scripting_enabled(bool enabled);

		NODISCARD size_t get_language_count() const noexcept { return m_languages.size(); }
		NODISCARD ScriptLanguage * get_language(size_t index) noexcept { return m_languages[index]; }
		NODISCARD ScriptLanguage * get_language(String const & name);

		Error_ register_language(ScriptLanguage * language);
		Error_ unregister_language(ScriptLanguage const * language);

		void initialize_languages();
		void finalize_languages();
		NODISCARD bool are_languages_finalized() const noexcept { return m_languages_finalized; }
	};

	SINGLETON_WRAPPER(ScriptManager, get_scr);
}

// script language
namespace Ism
{
	class ISM_API ScriptLanguage : public Object
	{
		DEFINE_CLASS(ScriptLanguage, Object);

	public:
		ScriptLanguage();
		virtual ~ScriptLanguage() override;

		NODISCARD virtual String get_name() const = 0;

		virtual Error_ initialize() = 0;
		virtual Error_ finalize() = 0;

		virtual Script * new_scipt() = 0;
	};
}

// script
namespace Ism
{
	class ISM_API Script : public Resource
	{
		DEFINE_CLASS(Script, Resource);

	public:
		Script();
		explicit Script(String const & path);
		virtual ~Script() override;

	public:
		virtual bool can_instantiate() const = 0;
		virtual Ref<ScriptInstance> instance_create(Object * self) = 0;
		
		virtual bool has_source_code() const = 0;
		virtual String get_source_code() const = 0;
		virtual void set_source_code(String const & code) = 0;
		virtual Error_ reload(bool keep_state = false) = 0;

		virtual bool has_method(String const & method) const = 0;
		
		virtual bool is_tool() const = 0;
		virtual bool is_valid() const = 0;

		virtual ScriptLanguage * get_language() const = 0;

		virtual void get_constants(HashMap<String, ObjectRef> * constants) {}
		virtual void get_members(HashSet<String> * members) {}

		virtual bool is_placeholder_fallback_enabled() const { return false; }
	};
}

// script instance
namespace Ism
{
	class ISM_API ScriptInstance : public Object
	{
		DEFINE_CLASS(ScriptInstance, Object);

	public:
		virtual ~ScriptInstance();

		virtual void notification(i32 notification) = 0;

		virtual bool is_placeholder() const { return false; }
		virtual Ref<Script> get_script() const = 0;
		virtual ScriptLanguage * get_language() = 0;
		virtual Object * get_owner() = 0;

		virtual bool get_constants(Vector<ObjectRef> * out) const = 0;
		virtual bool get_properties(Vector<PropertyRef> * out) const = 0;
		virtual bool get_functions(Vector<FunctionRef> * out) const = 0;
	};
}

// placeholder script instance
namespace Ism
{
	class ISM_API PlaceholderScriptInstance : public ScriptInstance
	{
		DEFINE_CLASS(PlaceholderScriptInstance, ScriptInstance);

		Object *			m_owner{};
		Ref<Script>			m_script{};
		ScriptLanguage *	m_language{};

	public:
		PlaceholderScriptInstance(ScriptLanguage * language, Ref<Script> script, Object * owner);
		virtual ~PlaceholderScriptInstance() override;

		virtual void notification(i32 notification) override {}
		
		virtual bool is_placeholder() const override { return true; }
		virtual Ref<Script> get_script() const override { return m_script; }
		virtual ScriptLanguage * get_language() { return m_language; }
		virtual Object * get_owner() override { return m_owner; }

		virtual bool get_constants(Vector<ObjectRef> * out) const override;
		virtual bool get_properties(Vector<PropertyRef> * out) const override;
		virtual bool get_functions(Vector<FunctionRef> * out) const override;
	};
}

#endif // !_ISM_SCRIPT_HPP_
