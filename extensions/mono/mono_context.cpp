#include <extensions/mono/mono_context.hpp>
#include <core/os/os.hpp>
#include <core/config/project_settings.hpp>
#include <core/io/file.hpp>

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

	inline void CS_Ism_Object_print(MonoString * message) {
		PRINT_LINE(mono_string_to_utf8(message));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	SINGLETON_EMBED(MonoContext);

	Error_ MonoContext::initialize()
	{
		String const lib_dir{ "C:\\Program Files\\Mono\\lib" };
		String const etc_dir{ "C:\\Program Files\\Mono\\etc" };
		mono_set_dirs(lib_dir.c_str(), etc_dir.c_str());

		if (!(m_domain = mono_jit_init(VERSION_FULL_NAME))) {
			PRINT_ERROR("failed initializing mono jit");
			return Error_Failed;
		}

		if (Error_ const err{ load_assemblies() }) {
			PRINT_ERROR("failed loading csharp assemblies");
			return err;
		}

		return Error_OK;
	}

	Error_ MonoContext::finalize()
	{
		if (m_domain) { mono_jit_cleanup(m_domain); m_domain = nullptr; }
		return Error_OK;
	}

	Error_ MonoContext::load_assemblies()
	{
		// load engine dll
		String const engine_dll{ globals()->get_bin_path().path_join("ism-csharp.dll") };
		if (!File::exists(engine_dll)) { PRINT_ERROR("could not find engine assembly"); return Error_Failed; }
		if (Error_ const err{ load_dll(engine_dll)}) { PRINT_ERROR("failed loading engine assembly"); return err; }

		mono_add_internal_call("Ism.Object::print", &CS_Ism_Object_print);
		if (!(m_object_class = mono_class_from_name(m_dll.get<MonoImage *>(0), "Ism", "Object"))) { return Error_Failed; }
		if (!(m_script_class = mono_class_from_name(m_dll.get<MonoImage *>(0), "Ism", "Script"))) { return Error_Failed; }
		if (!(m_loop_class = mono_class_from_name(m_dll.get<MonoImage *>(0), "Ism", "MainLoop"))) { return Error_Failed; }
		
		// load app dll
		String const app_dll{ globals()->get_bin_path().path_join(os()->get_exec_path().stem() + "-csharp.dll")};
		if (!File::exists(app_dll)) { PRINT_ERROR("could not find application assembly"); return Error_Failed; }
		if (Error_ const err{ load_dll(app_dll) }) { PRINT_ERROR("failed loading engine assembly"); return err; }
		i32 const rows{ mono_image_get_table_rows(m_dll.get<MonoImage *>(1), MONO_TABLE_TYPEDEF) };
		for (i32 i{ 1 }; i < rows; ++i)
		{
			MonoClass * klass{ mono_class_get(m_dll.get<MonoImage *>(1), (i + 1) | MONO_TOKEN_TYPE_DEF) };
			if (!mono_class_is_subclass_of(klass, m_script_class, false)) { continue; }
			cstring const space{ mono_class_get_namespace(klass) };
			cstring const name{ mono_class_get_name(klass) };
			char signature[128]; sprintf(signature, "%s.%s", space, name);
			if (!m_classes.insert({ signature, klass }).second) { continue; }
		
			printf("class: %s\n", signature);
			for_mono_class_interfaces(klass, [&](MonoClass * t) { printf("implements: %s\n", mono_class_get_name(t)); });
			for_mono_class_nested_types(klass, [&](MonoClass * t) { printf("nested: %s\n", mono_class_get_name(t)); });
			for_mono_class_fields(klass, [&](MonoClassField * f) { printf("field: %s, flags 0x%x\n", mono_field_get_name(f), mono_field_get_flags(f)); });
			for_mono_class_methods(klass, [&](MonoMethod * m) { u32 iflags{}, flags{ mono_method_get_flags(m, &iflags) }; printf("method: %s, flags 0x%x, iflags 0x%x\n", mono_method_get_name(m), flags, iflags); });
			for_mono_class_properties(klass, [&](MonoProperty * p) { printf("property: %s, flags 0x%x\n", mono_property_get_name(p), mono_property_get_flags(p)); });
			for_mono_class_events(klass, [&](MonoEvent * e) { printf("event: %s, flags: 0x%x\n", mono_event_get_name(e), mono_event_get_flags(e)); });
			printf("\n");
		
			//auto get_method = [klass](cstring t, cstring f, cstring a = "") {
			//	char signature[128]; sprintf(signature, ".%s:%s(%s)", t, f, a);
			//	auto const d{ mono_method_desc_new(signature, false) };
			//	auto const m{ mono_method_desc_search_in_class(d, klass) };
			//	mono_method_desc_free(d);
			//	return m;
			//};
			//if (MonoMethod * method{ get_method(name, "SayHello") }) {
			//	mono_runtime_invoke(method, nullptr, nullptr, nullptr);
			//}
		}

		return Error_OK;
	}

	Error_ MonoContext::load_dll(String const & path)
	{
		if (path.empty()) { return Error_Failed; }
		if (m_dll.contains<String>(path)) { return Error_AlreadyExists; }
		auto const assembly{ mono_domain_assembly_open(m_domain, path.c_str()) };
		if (!assembly) { return Error_Failed; }
		auto const image{ mono_assembly_get_image(assembly) };
		if (!image) { return Error_Failed; }
		return m_dll.push_back(path, assembly, image), Error_OK;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}