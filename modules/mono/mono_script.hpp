#ifndef _ISM_MONO_SCRIPT_HPP_
#define _ISM_MONO_SCRIPT_HPP_

#include <core/object/script.hpp>

namespace ism
{
	// mono language
	class ISM_MOD_API MonoLanguage : public ScriptLanguage
	{
		DEFINE_CLASS(MonoLanguage, ScriptLanguage);

	public:
		MonoLanguage();
		virtual ~MonoLanguage() override;

		virtual StringName get_name() const override { return "mono"; }

		virtual void initialize() override;
		virtual void finalize() override;
	};
}

namespace ism
{
	// mono script
	class ISM_MOD_API MonoScript : public Script
	{
		DEFINE_CLASS(MonoScript, Script);

	public:
		MonoScript();
		explicit MonoScript(Path const & path);
		virtual ~MonoScript() override;
		
		virtual bool has_source_code() const override;
		virtual String get_source_code() const override;
		virtual void set_source_code(String const & code) override;
		virtual Error_ reload(bool keep_state = false) override;
	};
}

namespace ism
{
	// mono instance
	class ISM_MOD_API MonoInstance : public ScriptInstance
	{
		DEFINE_CLASS(MonoInstance, ScriptInstance);

		Object * m_owner{};
		Ref<Script> m_script{};
		ScriptLanguage * m_language{};

	public:
		MonoInstance(ScriptLanguage * language, Ref<Script> script, Object * owner);
		virtual ~MonoInstance() override;

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

#endif // !_ISM_MONO_SCRIPT_HPP_
