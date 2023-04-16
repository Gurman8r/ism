#include <modules/mono/register_mono_types.hpp>
#include <modules/mono/mono_script_language.hpp>
#include <modules/mono/mono_behavior.hpp>

namespace ism
{
	bool initialize_mono_library(ExtensionInterface const * iface, Extension * library, ExtensionInitialization * init)
	{
		init->minimum_level = ExtensionInitializationLevel_Scene;
		init->userdata = nullptr;
		init->initialize = &initialize_mono_module;
		init->finalize = &finalize_mono_module;
		return true;
	}

	MonoLanguage * mono_language{};

	void initialize_mono_module(void * userdata, ExtensionInitializationLevel_ level)
	{
		if (level != ExtensionInitializationLevel_Scene) { return; }

		INITIALIZE_CLASS(MonoLanguage, MonoScript, MonoBehavior);

		mono_language = memnew(MonoLanguage);
		ScriptServer::get_singleton()->register_language(mono_language);
	}

	void finalize_mono_module(void * userdata, ExtensionInitializationLevel_ level)
	{
		if (level != ExtensionInitializationLevel_Scene) { return; }

		ScriptServer::get_singleton()->unregister_language(mono_language);
		memdelete(mono_language);
	}
}