#ifndef _ISM_CSHARP_SCRIPT_HPP_
#define _ISM_CSHARP_SCRIPT_HPP_

#include <extensions/mono/csharp_language.hpp>
#include <core/io/resource_loader.hpp>
#include <core/io/resource_saver.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// csharp script
	class ISM_EXT_API CSharpScript : public Script
	{
		OBJECT_CLASS(CSharpScript, Script);

		CSharpLanguage * const m_language;

		bool m_tool : 1, m_global_class : 1, m_valid : 1;

		Ref<CSharpScript> m_base_script{};

		String m_source{};
		String m_class_name{};

	public:
		CSharpScript() noexcept;
		virtual ~CSharpScript() noexcept override;

		virtual ScriptLanguage * get_language() const noexcept override { return m_language; }

		virtual bool is_tool() const override { return m_tool; }
		virtual bool is_valid() const override { return m_valid; }

		virtual String get_source_code() const override { return m_source; }
		virtual bool has_source_code() const override { return !m_source.empty(); }
		virtual void set_source_code(String const & value) override { m_source = value; }
		virtual Error_ reload(bool keep_state = false) override;

		virtual bool can_instantiate() const override;
		virtual ScriptInstance * new_instance(Object * self) override;
		virtual PlaceholderScriptInstance * new_placeholder_instance(Object * self) override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// csharp instance
	class ISM_EXT_API CSharpInstance : public ScriptInstance
	{
		OBJECT_CLASS(CSharpInstance, ScriptInstance);

		CSharpLanguage * const m_language;
		Ref<CSharpScript> const m_script;
		Object * const m_owner;

		MonoObject * m_handle{};

	public:
		CSharpInstance(Ref<Script> const & script, Object * owner) noexcept;
		virtual ~CSharpInstance() noexcept override;
		virtual bool is_placeholder() const override { return false; }

		virtual ScriptLanguage * get_language() const noexcept override { return m_language; }
		virtual Ref<Script> get_script() const noexcept override { return m_script; }
		virtual Object * get_owner() const noexcept override { return m_owner; }

		virtual void get_constants(Vector<ObjectRef> * constants) const override {}
		virtual void get_members(Vector<ObjectRef> * members) const override {}
		virtual void get_properties(Vector<PropertyRef> * properties) const override {}
		virtual void get_functions(Vector<FunctionRef> * functions) const override {}

		virtual bool get(String const & name, Var & value) const override;
		virtual bool set(String const & name, Var const & value) override;

		virtual void notification(i32 notification) override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// csharp script loader
	class ISM_EXT_API CSharpScriptFormatLoader : public ResourceFormatLoader {
		OBJECT_CLASS(CSharpScriptFormatLoader, ResourceFormatLoader);
	public:
		virtual RES load(String const & path, Error_ * r_error = nullptr) override;
		virtual void get_recognized_extensions(Vector<String> * r_out) const override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_CSHARP_SCRIPT_HPP_
