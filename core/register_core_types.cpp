#include <core/register_core_types.hpp>
#include <core/api/class.hpp>

using namespace ism;
using namespace ism;

void ism::register_core_types()
{
	VERIFY(api::typeof<Object>().ready());
	VERIFY(api::typeof<TypeObject>().ready());
	VERIFY(api::typeof<IntObject>().ready());
	VERIFY(api::typeof<FloatObject>().ready());
	VERIFY(api::typeof<StringObject>().ready());
	VERIFY(api::typeof<ListObject>().ready());
	VERIFY(api::typeof<DictObject>().ready());
	VERIFY(api::typeof<CapsuleObject>().ready());
	VERIFY(api::typeof<FunctionObject>().ready());
	VERIFY(api::typeof<MethodObject>().ready());
	VERIFY(api::typeof<CppFunctionObject>().ready());
	VERIFY(api::typeof<PropertyObject>().ready());
	VERIFY(api::typeof<ModuleObject>().ready());

	api::get_internals()
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
	auto & db{ api::get_internals().class_db };

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