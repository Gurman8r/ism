#include <core/register_core_types.hpp>
#include <core/api/class.hpp>

using namespace ism;
using namespace ism;

void ism::register_core_types()
{
	VERIFY(typeof<OBJECT>()->ready());
	VERIFY(typeof<TYPE>()->ready());
	VERIFY(typeof<INT>()->ready());
	VERIFY(typeof<FLT>()->ready());
	VERIFY(typeof<STR>()->ready());
	VERIFY(typeof<LIST>()->ready());
	VERIFY(typeof<DICT>()->ready());
	VERIFY(typeof<CAPSULE>()->ready());
	VERIFY(typeof<FUNCTION>()->ready());
	VERIFY(typeof<METHOD>()->ready());
	VERIFY(typeof<PROPERTY>()->ready());
	VERIFY(typeof<CPP_FUNCTION>()->ready());
	VERIFY(typeof<MODULE>()->ready());
	VERIFY(typeof<GENERIC>()->ready());

	TypeDB::register_class<BaseObject>();
	TypeDB::register_class<TypeObject>();
	TypeDB::register_class<IntObject>();
	TypeDB::register_class<FloatObject>();
	TypeDB::register_class<StringObject>();
	TypeDB::register_class<ListObject>();
	TypeDB::register_class<DictObject>();
	TypeDB::register_class<CapsuleObject>();
	TypeDB::register_class<FunctionObject>();
	TypeDB::register_class<MethodObject>();
	TypeDB::register_class<PropertyObject>();
	TypeDB::register_class<CppFunctionObject>();
	TypeDB::register_class<ModuleObject>();
	TypeDB::register_class<GenericObject>();
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
	auto & db{ TypeDB::classes };

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