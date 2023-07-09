#ifndef _ISM_MONO_HPP_
#define _ISM_MONO_HPP_

#include <core/object/class.hpp>

#include <mono/jit/jit.h>
#include <mono/metadata/appdomain.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/class.h>
#include <mono/metadata/metadata.h>
#include <mono/metadata/object.h>
#include <mono/metadata/tokentype.h>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_EXT_API MonoContext
	{
		static MonoContext * __singleton;

		friend class CSharpLanguage;
		friend class CSharpScript;
		friend class CSharpInstance;

		MonoDomain * m_domain{};
		Batch<String, MonoAssembly *, MonoImage *> m_dll{};

		HashMap<String, MonoClass *> m_classes{};
		MonoClass * m_object_class{};
		MonoClass * m_script_class{};
		MonoClass * m_component_class{};
		MonoClass * m_behavior_class{};

	public:
		MonoContext() noexcept { SINGLETON_CTOR(); }
		~MonoContext() noexcept { SINGLETON_DTOR(); }
		SINGLETON_GETTER(MonoContext);

		Error_ initialize();
		Error_ finalize();

		Error_ load_assemblies();
		Error_ load_dll(String const & path);
	};

	SINGLETON_WRAPPER(MonoContext, get_mono);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_MONO_HPP_
