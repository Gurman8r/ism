#include <core/register_core_types.hpp>
#include <core/api/modsupport.hpp>

using namespace ism;

void ism::register_core_types()
{
	ClassDB::register_class<CoreObject>();
	ClassDB::register_class<CoreType>();
	ClassDB::register_class<CoreInt>();
	ClassDB::register_class<CoreFloat>();
	ClassDB::register_class<CoreString>();
	ClassDB::register_class<CoreList>();
	ClassDB::register_class<CoreDict>();
	ClassDB::register_class<CoreCapsule>();
	ClassDB::register_class<CoreFunction>();
	ClassDB::register_class<CoreProperty>();
	ClassDB::register_class<CoreCppFunction>();
	ClassDB::register_class<CoreModule>();
	ClassDB::register_class<CoreGeneric>();
}

void ism::register_core_driver_types()
{
}

void ism::register_core_settings()
{
}

void ism::register_core_singletons()
{
}

void ism::unregister_core_driver_types()
{
}

void ism::unregister_core_types()
{
	auto & db{ ClassDB::classes };

	for (size_t i = 0; i < db.size(); ++i)
	{
		TYPE & type{ db.get<TYPE>(i) };

		typeof(type)->tp_finalize(*type);
	}

	while (!db.empty())
	{
		db.pop_back();
	}
}