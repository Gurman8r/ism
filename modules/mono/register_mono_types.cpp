#include <modules/mono/register_mono_types.hpp>
#include <modules/mono/mono_script.hpp>
#include <modules/mono/mono_behavior.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

using namespace ism;

static MonoLanguage * mono_language{};

bool open_mono_library(ism::ExtensionInterface const * iface, ism::Extension * extension, ism::ExtensionInitialization * init)
{
	if (!iface || !extension || !init) { return false; }
	PRINT_LINE("open mono module");
	init->minimum_level = ExtensionInitializationLevel_Scene;
	init->user = nullptr;
	init->initialize = &initialize_mono_module;
	init->finalize = &finalize_mono_module;
	return true;
}

void initialize_mono_module(void * user, ism::ExtensionInitializationLevel_ level)
{
	if (level != ExtensionInitializationLevel_Scene) { return; }
	PRINT_LINE("initialize mono module");
	//INITIALIZE_CLASS(MonoLanguage, MonoScript, MonoBehavior);
	mono_language = memnew(MonoLanguage);
	ScriptServer::get_singleton()->register_language(mono_language);
}

void finalize_mono_module(void * user, ism::ExtensionInitializationLevel_ level)
{
	if (level != ExtensionInitializationLevel_Scene) { return; }
	PRINT_LINE("finalize mono module");
	ScriptServer::get_singleton()->unregister_language(mono_language);
	memdelete(mono_language);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */