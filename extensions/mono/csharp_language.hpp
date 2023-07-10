#ifndef _ISM_CSHARP_LANGUAGE_HPP_
#define _ISM_CSHARP_LANGUAGE_HPP_

#include <extensions/mono/mono_context.hpp>
#include <core/object/script.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// csharp language
	class ISM_EXT_API CSharpLanguage : public ScriptLanguage
	{
		OBJECT_CLASS(CSharpLanguage, ScriptLanguage);

		SINGLETON_CLASS(CSharpLanguage);

		friend class CSharpScript;
		friend class CSharpInstance;

		MonoContext * m_context{};

	public:
		CSharpLanguage() noexcept { SINGLETON_CTOR(); }
		virtual ~CSharpLanguage() noexcept override { SINGLETON_DTOR(); }

		virtual String get_name() const override { return "mono"; }

		virtual Error_ initialize() override;
		virtual Error_ finalize() override;

		virtual void get_reserved_words(Vector<String> * words) const override;
		virtual void get_comment_delimiters(Vector<String> * delimiters) const override;
		virtual void get_string_delimiters(Vector<String> * delimiters) const override;
		virtual bool has_named_classes() const { return true; }

		virtual Script * new_scipt() override;

		void reload_assemblies(bool soft_reload);
	};

	SINGLETON_WRAPPER(CSharpLanguage, get_csharp_language);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_CSHARP_LANGUAGE_HPP_
