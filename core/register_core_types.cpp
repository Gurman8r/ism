#include <core/register_core_types.hpp>
#include <core/input/input.hpp>
#include <core/io/config_file.hpp>
#include <core/io/image.hpp>
#include <core/os/main_loop.hpp>

using namespace ism;
using namespace ism;

void ism::register_core_types()
{
	VERIFY(ism::typeof<Object>().ready());
	VERIFY(ism::typeof<TypeObject>().ready());
	VERIFY(ism::typeof<IntObject>().ready());
	VERIFY(ism::typeof<FloatObject>().ready());
	VERIFY(ism::typeof<StringObject>().ready());
	VERIFY(ism::typeof<ListObject>().ready());
	VERIFY(ism::typeof<DictObject>().ready());
	VERIFY(ism::typeof<CapsuleObject>().ready());
	VERIFY(ism::typeof<FunctionObject>().ready());
	VERIFY(ism::typeof<MethodObject>().ready());
	VERIFY(ism::typeof<CppFunctionObject>().ready());
	VERIFY(ism::typeof<PropertyObject>().ready());
	VERIFY(ism::typeof<ModuleObject>().ready());

	ism::get_internals()

		.bind_class<CppFunctionObject>() // <- cppfunction MUST go first
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

		.bind_class<Resource>()
		.bind_class<ConfigFile>()
		.bind_class<Image>()

		.bind_class<Input>()
		.bind_class<InputEvent>()
		.bind_class<InputMap>()

		.bind_class<MainLoop>()

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
	auto & db{ ism::get_internals().class_db };

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