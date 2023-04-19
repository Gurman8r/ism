#ifndef _ISM_SCRIPT_HPP_
#define _ISM_SCRIPT_HPP_

#include <core/io/resource.hpp>

namespace ism
{
	class ScriptServer;
	class Script;
	class ScriptInstance;
	class ScriptLanguage;
	class PlaceHolderScriptInstance;
}

// script server
namespace ism
{
	class ISM_API ScriptServer : public Object
	{
		DEFINE_CLASS(ScriptServer, Object);

		static ScriptServer * __singleton;

		Vector<ScriptLanguage *> m_languages{};
		bool m_scripting_enabled{ true };
		bool m_reload_scripts_on_save{};
		bool m_languages_finalized{};

	public:
		ScriptServer() noexcept { __singleton = this; }
		virtual ~ScriptServer() noexcept override = default;
		FORCE_INLINE static ScriptServer * get_singleton() noexcept { return __singleton; }

	public:
		NODISCARD bool is_scripting_enabled() const noexcept { return m_scripting_enabled; }
		void set_scripting_enabled(bool enabled);

		NODISCARD u32 get_language_count() const noexcept { return (u32)m_languages.size(); }
		NODISCARD ScriptLanguage * get_language(u32 index) noexcept { return m_languages[(size_t)index]; }
		Error_ register_language(ScriptLanguage * language);
		Error_ unregister_language(ScriptLanguage const * language);

		void initialize_languages();
		void finalize_languages();
		NODISCARD bool are_languages_finalized() const noexcept { return m_languages_finalized; }
	};
}

// script
namespace ism
{
	class ISM_API Script : public Resource
	{
		DEFINE_CLASS(Script, Resource);

	public:
		Script();
		Script(Path const & path) noexcept { set_path(path); reload_from_file(); }
		virtual ~Script() override;
		virtual Error_ reload_from_file() override;

		//virtual bool can_instantiate() const = 0;
		//virtual Ref<Script> get_base_script() const = 0;
		//virtual StringName get_global_name() const = 0;
		//virtual bool inherits_script(Ref<Script> const & script) const = 0;
		//virtual StringName get_instance_base_type() const = 0;
		//virtual ScriptInstance * instance_create(Object * self) = 0;
		//virtual PlaceHolderScriptInstance * placeholder_instance_create(Object * self) { return nullptr; }
		//virtual bool instance_has(Object const * self) const = 0;
		
		virtual bool has_source_code() const = 0;
		virtual String get_source_code() const = 0;
		virtual void set_source_code(String const & code) = 0;
		virtual Error_ reload(bool keep_state = false) = 0;
		
		//virtual bool is_tool() const = 0;
		//virtual bool is_valid() const = 0;
		//virtual ScriptLanguage * get_language() const = 0;
	};
}

// script instance
namespace ism
{
	class ISM_API ScriptInstance
	{
	public:
		virtual ~ScriptInstance();

		virtual bool get_constants(HashMap<StringName, OBJ> * out) const = 0;
		virtual bool get_properties(HashMap<StringName, PROPERTY> * out) const = 0;
		virtual bool get_functions(HashMap<StringName, FUNCTION> * out) const = 0;

		virtual void notification(i32 notification) = 0;

		virtual bool is_placeholder() const { return false; }
		virtual Ref<Script> get_script() const = 0;
		virtual ScriptLanguage * get_language() = 0;
		virtual Object * get_owner() = 0;
	};
}

// script language
namespace ism
{
	class ISM_API ScriptLanguage : public Object
	{
		DEFINE_CLASS(ScriptLanguage, Object);

	public:
		ScriptLanguage();
		virtual ~ScriptLanguage() override;

		NODISCARD virtual StringName get_name() const = 0;

		virtual void initialize() = 0;
		virtual void finalize() = 0;
	};
}

// placeholder script instance
namespace ism
{
	class ISM_API PlaceHolderScriptInstance : public ScriptInstance
	{
		Object *			m_owner{};
		Ref<Script>			m_script{};
		ScriptLanguage *	m_language{};

	public:
		PlaceHolderScriptInstance(ScriptLanguage * language, Ref<Script> script, Object * owner);
		virtual ~PlaceHolderScriptInstance() override;

		virtual bool get_constants(HashMap<StringName, OBJ> * out) const;
		virtual bool get_properties(HashMap<StringName, PROPERTY> * out) const;
		virtual bool get_functions(HashMap<StringName, FUNCTION> * out) const;

		virtual void notification(i32 notification) override {}
		
		virtual bool is_placeholder() const override { return true; }
		virtual Ref<Script> get_script() const override { return m_script; }
		virtual ScriptLanguage * get_language() { return m_language; }
		virtual Object * get_owner() override { return m_owner; }
	};
}

#endif // !_ISM_SCRIPT_HPP_
