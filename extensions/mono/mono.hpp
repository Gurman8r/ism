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

	template <class Fn = void(*)(MonoClass *)
	> void for_mono_class_interfaces(MonoClass * klass, Fn fn) { void * it{}; while (MonoClass * e{ mono_class_get_interfaces(klass, &it) }) { fn(e); } }

	template <class Fn = void(*)(MonoClass *)
	> void for_mono_class_nested_types(MonoClass * klass, Fn fn) { void * it{}; while (MonoClass * e{ mono_class_get_nested_types(klass, &it) }) { fn(e); } }

	template <class Fn = void(*)(MonoClassField *)
	> void for_mono_class_fields(MonoClass * klass, Fn fn) { void * it{}; while (MonoClassField * e{ mono_class_get_fields(klass, &it) }) { fn(e); } }

	template <class Fn = void(*)(MonoMethod *)
	> void for_mono_class_methods(MonoClass * klass, Fn fn) { void * it{}; while (MonoMethod * e{ mono_class_get_methods(klass, &it) }) { fn(e); } }

	template <class Fn = void(*)(MonoProperty *)
	> void for_mono_class_properties(MonoClass * klass, Fn fn) { void * it{}; while (MonoProperty * e{ mono_class_get_properties(klass, &it) }) { fn(e); } }

	template <class Fn = void(*)(MonoProperty *)
	> void for_mono_class_events(MonoClass * klass, Fn fn) { void * it{}; while (MonoEvent * e{ mono_class_get_events(klass, &it) }) { fn(e); } }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_EXT_API Mono
	{
		static Mono * __singleton;

		friend class CSharpLanguage;
		friend class CSharpScript;
		friend class CSharpInstance;

		MonoDomain * m_domain{};
		Batch<String, MonoAssembly *, MonoImage *> m_data{};

		HashMap<String, MonoClass *> m_classes{};
		MonoClass * m_object_class{};
		MonoClass * m_script_class{};
		MonoClass * m_component_class{};
		MonoClass * m_behavior_class{};

	public:
		Mono() noexcept { SINGLETON_CTOR(__singleton, this); }
		~Mono() noexcept { SINGLETON_DTOR(__singleton, this); }
		FORCE_INLINE static Mono * get_singleton() noexcept { return __singleton; }

		bool initialize();
		bool finalize();

		Error_ load_assemblies();

		Error_ load_dll(String const & path);
	};

	SINGLETON_WRAPPER(Mono, get_mono);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_MONO_HPP_
