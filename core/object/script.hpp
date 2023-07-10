#ifndef _ISM_SCRIPT_HPP_
#define _ISM_SCRIPT_HPP_

#include <core/io/resource.hpp>
#include <core/object/var.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class Script;
	class ScriptInstance;
	class ScriptLanguage;
	class PlaceholderScriptInstance;

	// script server
	class ISM_API ScriptServer final : public Object
	{
		OBJECT_CLASS(ScriptServer, Object);

		SINGLETON_CLASS(ScriptServer);

		enum { MAX_LANGUAGES = 64 };
		Vector<ScriptLanguage *> m_languages{};
		bool m_scripting_enabled : 1, m_reload_scripts_on_save : 1, m_languages_finalized : 1;

	public:
		ScriptServer() noexcept;
		~ScriptServer() noexcept;

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

	SINGLETON_WRAPPER(ScriptServer, script_server);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	// script language
	class ISM_API ScriptLanguage : public Object
	{
		OBJECT_CLASS(ScriptLanguage, Object);

	public:
		virtual ~ScriptLanguage() noexcept override;

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

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	// script
	class ISM_API Script : public Resource
	{
		OBJECT_CLASS(Script, Resource);

	public:
		virtual ~Script() noexcept override;

		NODISCARD virtual ScriptLanguage * get_language() const noexcept = 0;

		NODISCARD virtual bool is_tool() const = 0;
		NODISCARD virtual bool is_valid() const = 0;
		
		NODISCARD virtual bool has_source_code() const = 0;
		NODISCARD virtual String get_source_code() const = 0;
		virtual void set_source_code(String const & code) = 0;
		virtual Error_ reload(bool keep_state = false) = 0;

		virtual void get_constants(HashMap<String, Var> * constants) const {}
		virtual void get_members(HashSet<String> * members) const {}
		virtual void get_properties(HashSet<String> * properties) const {}
		virtual void get_functions(HashSet<String> * functions) const {}

		virtual bool can_instantiate() const = 0;
		virtual ScriptInstance * new_instance(Object * self) = 0;
		virtual PlaceholderScriptInstance * new_placeholder_instance(Object * self) = 0;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	// script instance
	class ISM_API ScriptInstance : public Object
	{
		OBJECT_CLASS(ScriptInstance, Object);

	public:
		virtual ~ScriptInstance() noexcept override;
		NODISCARD virtual bool is_placeholder() const { return false; }

		NODISCARD virtual ScriptLanguage * get_language() const noexcept = 0;
		NODISCARD virtual Ref<Script> get_script() const noexcept = 0;
		NODISCARD virtual Object * get_owner() const noexcept = 0;

		virtual void get_constants(Vector<ObjectRef> * constants) const = 0;
		virtual void get_members(Vector<ObjectRef> * constants) const = 0;
		virtual void get_properties(Vector<PropertyRef> * properties) const = 0;
		virtual void get_functions(Vector<FunctionRef> * functions) const = 0;

		virtual bool get(String const & name, Var & value) const = 0;
		virtual bool set(String const & name, Var const & value) = 0;

		virtual void notification(i32 notification) = 0;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// placeholder script instance
	class ISM_API PlaceholderScriptInstance : public ScriptInstance
	{
		OBJECT_CLASS(PlaceholderScriptInstance, ScriptInstance);

		ScriptLanguage * const m_language;
		Ref<Script> const m_script;
		Object * const m_owner;

	public:
		PlaceholderScriptInstance(ScriptLanguage * language, Ref<Script> const & script, Object * owner) noexcept;
		virtual ~PlaceholderScriptInstance() noexcept override;
		virtual bool is_placeholder() const override { return true; }

		virtual ScriptLanguage * get_language() const noexcept override { return m_language; }
		virtual Ref<Script> get_script() const noexcept override { return m_script; }
		virtual Object * get_owner() const noexcept override { return m_owner; }

		virtual void get_constants(Vector<ObjectRef> * constants) const override {}
		virtual void get_members(Vector<ObjectRef> * members) const override {}
		virtual void get_properties(Vector<PropertyRef> * properties) const override {}
		virtual void get_functions(Vector<FunctionRef> * functions) const override {}

		virtual bool get(String const & name, Var & value) const override { return false; }
		virtual bool set(String const & name, Var const & value) override { return false; }

		virtual void notification(i32 notification) override {}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_SCRIPT_HPP_
