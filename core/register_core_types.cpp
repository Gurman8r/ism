#include <core/register_core_types.hpp>
#include <core/api/modsupport.hpp>

using namespace ism;

void ism::register_core_types()
{
	//VERIFY(Engine::get_singleton());
	//ObjectDB::setup();
	//register_global_constants();
	//Var::register_types();
	//CoreStringNames::create();
	//ClassDB::register_class<Object>();
	//ClassDB::register_class<WeakRef>();
	//ClassDB::register_class<Resource>();
	//ClassDB::register_virtual_class<Script>();
	//ClassDB::register_class<ConfigFile>();
	//ClassDB::register_class<Image>();
	//ClassDB::register_class<MainLoop>();

	ClassDB::register_type<TYPE>();
	ClassDB::register_type<OBJECT>();
	ClassDB::register_type<INT>();
	ClassDB::register_type<FLT>();
	ClassDB::register_type<STR>();
	ClassDB::register_type<LIST>();
	ClassDB::register_type<DICT>();
	ClassDB::register_type<CAPSULE>();
	ClassDB::register_type<FUNCTION>();
	ClassDB::register_type<PROPERTY>();
	ClassDB::register_type<CPP_FUNCTION>();
	ClassDB::register_type<MODULE>();
	ClassDB::register_type<GENERIC>();
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
	while (!ClassDB::classes.empty())
	{
		ClassDB::classes.pop_back();
	}
}