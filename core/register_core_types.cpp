#include <core/register_core_types.hpp>
#include <core/api/class.hpp>

using namespace ism;
using namespace ism;

void ism::register_core_types()
{
	VERIFY(typeof<OBJ>().ready());
	VERIFY(typeof<TYPE>().ready());
	VERIFY(typeof<INT>().ready());
	VERIFY(typeof<FLT>().ready());
	VERIFY(typeof<STR>().ready());
	VERIFY(typeof<LIST>().ready());
	VERIFY(typeof<DICT>().ready());
	VERIFY(typeof<CAPSULE>().ready());
	VERIFY(typeof<FUNCTION>().ready());
	VERIFY(typeof<METHOD>().ready());
	VERIFY(typeof<PROPERTY>().ready());
	VERIFY(typeof<CPP_FUNCTION>().ready());
	VERIFY(typeof<MODULE>().ready());
	VERIFY(typeof<GENERIC>().ready());

	TypeDB::bind_class<CppFunctionObject>(nullptr);
	TypeDB::bind_class<Object>(nullptr);
	TypeDB::bind_class<TypeObject>(nullptr);
	TypeDB::bind_class<IntObject>(nullptr);
	TypeDB::bind_class<FloatObject>(nullptr);
	TypeDB::bind_class<StringObject>(nullptr);
	TypeDB::bind_class<ListObject>(nullptr);
	TypeDB::bind_class<DictObject>(nullptr);
	TypeDB::bind_class<CapsuleObject>(nullptr);
	TypeDB::bind_class<FunctionObject>(nullptr);
	TypeDB::bind_class<MethodObject>(nullptr);
	TypeDB::bind_class<PropertyObject>(nullptr);
	TypeDB::bind_class<ModuleObject>(nullptr);
	TypeDB::bind_class<GenericObject>(nullptr);
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