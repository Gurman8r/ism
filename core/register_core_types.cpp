#include <core/register_core_types.hpp>
#include <core/object/detail/class.hpp>

using namespace ism;
using namespace ism;

void ism::register_core_types()
{
	VERIFY(typeof<Object>().ready());
	VERIFY(typeof<TypeObject>().ready());
	VERIFY(typeof<IntObject>().ready());
	VERIFY(typeof<FloatObject>().ready());
	VERIFY(typeof<StringObject>().ready());
	VERIFY(typeof<ListObject>().ready());
	VERIFY(typeof<DictObject>().ready());
	VERIFY(typeof<CapsuleObject>().ready());
	VERIFY(typeof<FunctionObject>().ready());
	VERIFY(typeof<MethodObject>().ready());
	VERIFY(typeof<CppFunctionObject>().ready());
	VERIFY(typeof<PropertyObject>().ready());
	VERIFY(typeof<ModuleObject>().ready());

	get_internals()
		.bind_class<CppFunctionObject>()
		.bind_class<Object>()
		.bind_class<TypeObject>()
		.bind_class<IntObject>()
		.bind_class<FloatObject>()
		.bind_class<StringObject>()
		.bind_class<ListObject>()
		.bind_class<DictObject>()
		.bind_class<CapsuleObject>()
		.bind_class<FunctionObject>()
		.bind_class<MethodObject>()
		.bind_class<PropertyObject>()
		.bind_class<ModuleObject>()
		;
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
	auto & db{ get_internals().class_db };

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