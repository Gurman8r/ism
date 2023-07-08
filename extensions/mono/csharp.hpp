#ifndef _ISM_CSHARP_HPP_
#define _ISM_CSHARP_HPP_

#include <extensions/mono/mono.hpp>
#include <core/object/script.hpp>
#include <core/io/resource_loader.hpp>
#include <core/io/resource_saver.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class CSharpScript;
	class CSharpInstance;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// csharp language
	class ISM_EXT_API CSharpLanguage : public ScriptLanguage
	{
		DEFINE_CLASS(CSharpLanguage, ScriptLanguage);

		static CSharpLanguage * __singleton;

		friend class CSharpScript;
		friend class CSharpInstance;

		Mono * m_mono{};

	public:
		CSharpLanguage();
		virtual ~CSharpLanguage() override;
		SINGLETON_GETTER(CSharpLanguage, __singleton);

		virtual String get_name() const override { return "mono"; }

		virtual Error_ initialize() override;
		virtual Error_ finalize() override;

		virtual Script * new_scipt() override;

		void reload_assemblies(bool soft_reload);
	};

	SINGLETON_WRAPPER(CSharpLanguage, get_cs_language);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// csharp script
	class ISM_EXT_API CSharpScript : public Script
	{
		DEFINE_CLASS(CSharpScript, Script);

		ScriptLanguage * const m_language{};
		String m_source{};
		mutable bool m_changed{};

	public:
		CSharpScript();
		explicit CSharpScript(String const & path);
		virtual ~CSharpScript() override;

	public:
		virtual bool can_instantiate() const override { return true; }
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

	// csharp instance
	class ISM_EXT_API CSharpInstance : public ScriptInstance
	{
		DEFINE_CLASS(CSharpInstance, ScriptInstance);

		ScriptLanguage * const m_language{};
		Ref<Script> m_script{};
		Object * m_owner{};

		MonoObject * m_handle{};

	public:
		CSharpInstance(Ref<Script> script, Object * owner);
		virtual ~CSharpInstance() override;

		virtual void notification(i32 notification) override;

		virtual bool is_placeholder() const override { return false; }
		virtual Ref<Script> get_script() const override { return m_script; }
		virtual ScriptLanguage * get_language() { return m_language; }
		virtual Object * get_owner() override { return m_owner; }

		virtual bool get_constants(Vector<ObjectRef> * out) const override { return true; }
		virtual bool get_properties(Vector<PropertyRef> * out) const override { return true; }
		virtual bool get_functions(Vector<FunctionRef> * out) const override { return true; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// csharp script loader
	class ISM_EXT_API CSharpScriptFormatLoader : public ResourceFormatLoader {
		DEFINE_CLASS(CSharpScriptFormatLoader, ResourceFormatLoader);
	public:
		virtual RES load(String const & path, Error_ * error = nullptr) override;
		virtual void get_recognized_extensions(Vector<String> * out) const override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_CSHARP_HPP_
