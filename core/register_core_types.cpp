#include <core/register_core_types.hpp>
#include <core/api/object/generic_object.hpp>

using namespace ism;
using namespace ism::api;

void ism::register_core_types()
{
	ClassDB::register_class<BaseObject>();
	ClassDB::register_class<TypeObject>();
	ClassDB::register_class<PropertyObject>();
	ClassDB::register_class<CppFunctionObject>();

	ClassDB::register_class<IntObject>();
	ClassDB::register_class<FloatObject>();
	ClassDB::register_class<StringObject>();
	ClassDB::register_class<ListObject>();
	ClassDB::register_class<DictObject>();
	ClassDB::register_class<CapsuleObject>();
	ClassDB::register_class<ModuleObject>();
	ClassDB::register_class<GenericObject>();
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