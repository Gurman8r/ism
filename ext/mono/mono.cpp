#include <ext/mono/mono.hpp>
#include <core/os/os.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	inline void CS_Ism_Object_print(MonoString * message) {
		PRINT_LINE(mono_string_to_utf8(message));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Mono * Mono::__singleton{};

	bool Mono::initialize()
	{
		String const lib_dir{ "C:\\Program Files\\Mono\\lib" };
		String const etc_dir{ "C:\\Program Files\\Mono\\etc" };
		mono_set_dirs(lib_dir.c_str(), etc_dir.c_str());

		if (!(m_dom = mono_jit_init("ism"))) {
			PRINT_ERROR("failed initializing mono jit compiler");
			return false;
		}

		if (load_assemblies() != Error_OK) {
			PRINT_ERROR("failed loading csharp assemblies");
			return false;
		}

		return true;
	}

	bool Mono::finalize()
	{
		if (m_dom) { mono_jit_cleanup(m_dom); m_dom = nullptr; }
		return true;
	}

	Error_ Mono::load_assemblies()
	{

		String const engine_dll{ get_os()->get_bin_dir().path_join("ism-CSharp.dll") };
		if (Error_ const err{ load_dll(engine_dll)}) { return err; }

		mono_add_internal_call("Ism.Object::print", &CS_Ism_Object_print);
		if (!(m_object_base = mono_class_from_name(m_data.get<MonoImage *>(0), "Ism", "Object"))) { return Error_Failed; }
		if (!(m_script_base = mono_class_from_name(m_data.get<MonoImage *>(0), "Ism", "Script"))) { return Error_Failed; }
		
		String const exe_dll{ get_os()->get_bin_dir().path_join(get_os()->get_exe_name() + "-CSharp.dll") };
		if (Error_ const err{ load_dll(exe_dll) }) { return err; }
		
		i32 const rows{ mono_image_get_table_rows(m_data.get<MonoImage *>(1), MONO_TABLE_TYPEDEF) };
		for (i32 i{ 1 }; i < rows; ++i)
		{
			MonoClass * klass{ mono_class_get(m_data.get<MonoImage *>(1), (i + 1) | MONO_TOKEN_TYPE_DEF) };
			if (!mono_class_is_subclass_of(klass, m_script_base, false)) { continue; }
			cstring const space{ mono_class_get_namespace(klass) };
			cstring const name{ mono_class_get_name(klass) };
			char signature[128]; sprintf(signature, "%s.%s", space, name);
			if (!m_classes.insert({ signature, klass }).second) { continue; }
		
			printf("class: %s\n", signature);
			for_mono_class_interfaces(klass, [&](MonoClass * t) { printf("implements: %s\n", mono_class_get_name(t)); });
			for_mono_class_nested_types(klass, [&](MonoClass * t) { printf("nested: %s\n", mono_class_get_name(t)); });
			for_mono_class_fields(klass, [&](MonoClassField * f) { printf("field: %s, flags 0x%x\n", mono_field_get_name(f), mono_field_get_flags(f)); });
			for_mono_class_methods(klass, [&](MonoMethod * m) { u32 iflags{}, flags{ mono_method_get_flags(m, &iflags) }; printf("Method: %s, flags 0x%x, iflags 0x%x\n", mono_method_get_name(m), flags, iflags); });
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

	Error_ Mono::load_dll(String const & path)
	{
		if (path.empty()) { return Error_Failed; }
		if (m_data.contains<String>(path)) { return Error_AlreadyExists; }
		auto const assembly{ mono_domain_assembly_open(m_dom, path.c_str()) };
		if (!assembly) { return Error_Failed; }
		auto const image{ mono_assembly_get_image(assembly) };
		if (!image) { return Error_Failed; }
		return m_data.push_back(path, assembly, image), Error_OK;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}