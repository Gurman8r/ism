#include <core/register_core_types.hpp>

void ism::register_core_types()
{
	//VERIFY(Engine::get_singleton());
	//ObjectDB::setup();
	//register_global_constants();
	//Var::register_types();
	//CoreStringNames::create();
	//ClassDB::register_class<handle>();
	//ClassDB::register_class<Object>();
	//ClassDB::register_class<WeakRef>();
	//ClassDB::register_class<Resource>();
	//ClassDB::register_virtual_class<Script>();
	//ClassDB::register_class<ConfigFile>();
	//ClassDB::register_class<Image>();
	//ClassDB::register_class<MainLoop>();
}

void ism::register_core_driver_types()
{
}

void ism::register_core_settings()
{
}

void ism::register_core_singletons()
{
	//ClassDB::register_class<Input>();
	//ClassDB::register_class<ProjectSettings>();
	//get_engine().add_singleton({ "Input", Input::get_singleton() });
	//get_engine().add_singleton({ "ProjectSettings", ProjectSettings::get_singleton() });
}

void ism::unregister_core_driver_types()
{
}

void ism::unregister_core_types()
{
	//ClassDB::cleanup_defaults();
	//ObjectDB::cleanup();
	//Var::unregister_types();
	//unregister_global_constants();
	//ClassDB::cleanup();
	//CoreStringNames::free();
}