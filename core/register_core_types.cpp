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
	VERIFY(typeof<CPP_FUNCTION>().ready());
	VERIFY(typeof<PROPERTY>().ready());
	VERIFY(typeof<MODULE>().ready());
	VERIFY(typeof<GENERIC>().ready());

	Internals::bind_class<CppFunctionObject>();
	Internals::bind_class<Object>();
	Internals::bind_class<TypeObject>();
	Internals::bind_class<IntObject>();
	Internals::bind_class<FloatObject>();
	Internals::bind_class<StringObject>();
	Internals::bind_class<ListObject>();
	Internals::bind_class<DictObject>();
	Internals::bind_class<CapsuleObject>();
	Internals::bind_class<FunctionObject>();
	Internals::bind_class<MethodObject>();
	Internals::bind_class<PropertyObject>();
	Internals::bind_class<ModuleObject>();
	Internals::bind_class<GenericObject>();
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
	auto & db{ get_internals().classes };

	for (size_t i = 0; i < db.size(); ++i)
	{
		TYPE & t{ db.get<TYPE>(i) };
		if (!t) { continue; }
		t->tp_base = nullptr;
		t->tp_bases = nullptr;
		t->tp_dict = nullptr;
		t->tp_mro = nullptr;
		t->tp_subclasses = nullptr;
	}

	while (!db.empty())
	{
		db.pop_back();
	}
}