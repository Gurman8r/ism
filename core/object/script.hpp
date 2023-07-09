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

	class ISM_API ScriptServer final : public Object
	{
		OBJECT_CLASS(ScriptServer, Object);

		static ScriptServer * __singleton;
		enum { MAX_LANGUAGES = 64 };
		Vector<ScriptLanguage *> m_languages{};
		bool m_scripting_enabled : 1, m_reload_scripts_on_save : 1, m_languages_finalized : 1;

	public:
		ScriptServer() noexcept;
		~ScriptServer() noexcept;
		SINGLETON_GETTER(ScriptServer);

		NODISCARD bool is_scripting_enabled() const noexcept;
		void set_scripting_enabled(bool enabled);

		NODISCARD size_t get_language_count() const noexcept;
		NODISCARD ScriptLanguage * get_language(size_t index) const noexcept;
		NODISCARD ScriptLanguage * get_language(String const & name) const noexcept;

		Error_ register_language(ScriptLanguage * language);
		Error_ unregister_language(ScriptLanguage * language);

		void initialize_languages();
		void finalize_languages();
		NODISCARD bool are_languages_finalized() const noexcept;
	};

	SINGLETON_WRAPPER(ScriptServer, get_scr);

}

// script language
namespace Ism
{
	class ISM_API ScriptLanguage : public Object
	{
		OBJECT_CLASS(ScriptLanguage, Object);

	public:
		ScriptLanguage();
		virtual ~ScriptLanguage() override;

		NODISCARD virtual String get_name() const = 0;

		virtual Error_ initialize() = 0;
		virtual Error_ finalize() = 0;

		void get_core_type_words(Vector<String> * words) const;
		virtual void get_reserved_words(Vector<String> * words) const = 0;
		virtual void get_comment_delimiters(Vector<String> * delimiters) const = 0;
		virtual void get_string_delimiters(Vector<String> * delimiters) const = 0;
		virtual bool has_named_classes() const = 0;

		virtual Script * new_scipt() = 0;

	};
}

// script
namespace Ism
{
	class ISM_API Script : public Resource
	{
		OBJECT_CLASS(Script, Resource);

	public:
		Script();
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
		OBJECT_CLASS(ScriptInstance, Object);

	public:
		virtual ~ScriptInstance();

		virtual bool is_placeholder() const { return false; }
		virtual Ref<Script> get_script() const = 0;
		virtual ScriptLanguage * get_language() = 0;
		virtual Object * get_owner() = 0;

		virtual bool get(String const & name, ObjectRef & value) const = 0;
		virtual bool set(String const & name, ObjectRef const & value) = 0;

		virtual bool get_constants(Vector<ObjectRef> * out) const = 0;
		virtual bool get_properties(Vector<PropertyRef> * out) const = 0;
		virtual bool get_functions(Vector<FunctionRef> * out) const = 0;

		virtual void notification(i32 notification) = 0;
	};
}

// placeholder script instance
namespace Ism
{
	class ISM_API PlaceholderScriptInstance : public ScriptInstance
	{
		OBJECT_CLASS(PlaceholderScriptInstance, ScriptInstance);

		Object *			m_owner{};
		Ref<Script>			m_script{};
		ScriptLanguage *	m_language{};

	public:
		PlaceholderScriptInstance(ScriptLanguage * language, Ref<Script> script, Object * owner);
		virtual ~PlaceholderScriptInstance() override;

		virtual bool is_placeholder() const override { return true; }
		virtual Ref<Script> get_script() const override { return m_script; }
		virtual ScriptLanguage * get_language() { return m_language; }
		virtual Object * get_owner() override { return m_owner; }

		virtual bool get(String const & name, ObjectRef & value) const override { return false; }
		virtual bool set(String const & name, ObjectRef const & value) override { return false; }

		virtual bool get_constants(Vector<ObjectRef> * out) const override { return false; }
		virtual bool get_properties(Vector<PropertyRef> * out) const override { return false; }
		virtual bool get_functions(Vector<FunctionRef> * out) const override { return false; }

		virtual void notification(i32 notification) override {}
	};
}

#endif // !_ISM_SCRIPT_HPP_
