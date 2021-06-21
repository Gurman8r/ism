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

	TypeDB::register_class<BaseObject>(nullptr);
	TypeDB::register_class<TypeObject>(nullptr);
	TypeDB::register_class<IntObject>(nullptr);
	TypeDB::register_class<FloatObject>(nullptr);
	TypeDB::register_class<StringObject>(nullptr);
	TypeDB::register_class<ListObject>(nullptr);
	TypeDB::register_class<DictObject>(nullptr);
	TypeDB::register_class<CapsuleObject>(nullptr);
	TypeDB::register_class<FunctionObject>(nullptr);
	TypeDB::register_class<MethodObject>(nullptr);
	TypeDB::register_class<PropertyObject>(nullptr);
	TypeDB::register_class<CppFunctionObject>(nullptr);
	TypeDB::register_class<ModuleObject>(nullptr);
	TypeDB::register_class<GenericObject>(nullptr);
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