#include <extensions/mono/register_mono_types.hpp>
#include <extensions/mono/csharp_script.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace Ism
{
	static CSharpLanguage * cs_language;
	static Ref<CSharpScriptFormatLoader> cs_loader;

	bool Ism::open_mono_extension(IsmExtensionInterfacePtr iface, IsmExtensionPtr library, IsmExtensionInitializationPtr initialization)
	{
		if (!iface || !library || !initialization) { return false; }
		PRINT_INFO("opening mono");
		auto init{ (ExtensionInitialization *)initialization };
		init->minimum_level = ExtensionInitializationLevel_Scene;
		init->initialize = &initialize_mono_extension;
		init->finalize = &finalize_mono_extension;
		init->user = nullptr;
		return true;
	}

	void Ism::initialize_mono_extension(void * user, IsmExtensionInitializationLevel level)
	{
		if (level != ExtensionInitializationLevel_Scene) { return; }
		PRINT_INFO("initializing mono");
		resource_loader()->add_resource_format_loader(cs_loader.instance());
		REGISTER_CLASS(CSharpLanguage, CSharpScript, CSharpInstance);
		cs_language = memnew(CSharpLanguage); script_server()->register_language(cs_language);
	}

	void Ism::finalize_mono_extension(void * user, IsmExtensionInitializationLevel level)
	{
		if (level != ExtensionInitializationLevel_Scene) { return; }
		PRINT_INFO("finalizing mono");
		script_server()->unregister_language(cs_language); memdelete(cs_language);
		UNREGISTER_CLASS(CSharpLanguage, CSharpScript, CSharpInstance);
		resource_loader()->remove_resource_format_loader(cs_loader); cs_loader = nullptr;
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */