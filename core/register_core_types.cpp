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

	OBJ scope{ nullptr };
	TypeDB::bind_class<CppFunctionObject>(scope);
	TypeDB::bind_class<Object>(scope);
	TypeDB::bind_class<TypeObject>(scope);
	TypeDB::bind_class<IntObject>(scope);
	TypeDB::bind_class<FloatObject>(scope);
	TypeDB::bind_class<StringObject>(scope);
	TypeDB::bind_class<ListObject>(scope);
	TypeDB::bind_class<DictObject>(scope);
	TypeDB::bind_class<CapsuleObject>(scope);
	TypeDB::bind_class<FunctionObject>(scope);
	TypeDB::bind_class<MethodObject>(scope);
	TypeDB::bind_class<PropertyObject>(scope);
	TypeDB::bind_class<ModuleObject>(scope);
	TypeDB::bind_class<GenericObject>(scope);
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