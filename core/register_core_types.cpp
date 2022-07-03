#include <core/register_core_types.hpp>
#include <core/input/input.hpp>
#include <core/io/config_file.hpp>
#include <core/io/image.hpp>
#include <core/os/main_loop.hpp>

using namespace ism;

void ism::register_core_types()
{
	ASSERT(typeof<BaseObject>().ready());
	ASSERT(typeof<TypeObject>().ready());
	ASSERT(typeof<IntObject>().ready());
	ASSERT(typeof<FloatObject>().ready());
	ASSERT(typeof<StringObject>().ready());
	ASSERT(typeof<TupleObject>().ready());
	ASSERT(typeof<ListObject>().ready());
	ASSERT(typeof<DictObject>().ready());
	ASSERT(typeof<CapsuleObject>().ready());
	ASSERT(typeof<FunctionObject>().ready());
	ASSERT(typeof<MethodObject>().ready());
	ASSERT(typeof<CppFunctionObject>().ready());
	ASSERT(typeof<PropertyObject>().ready());
	ASSERT(typeof<ModuleObject>().ready());
	ASSERT(typeof<GenericObject>().ready());

	INTERNALS->install_class
	<
		CppFunctionObject, // <- cppfunction must go first
		BaseObject,
		TypeObject,
		IntObject,
		FloatObject,
		StringObject,
		TupleObject,
		ListObject,
		DictObject,
		CapsuleObject,
		FunctionObject,
		MethodObject,
		PropertyObject,
		ModuleObject,
		GenericObject,

		Resource,
		ConfigFile,
		Image,

		Input,
		InputEvent,
		InputMap,

		Event,
		EventHandler,
		DummyHandler,
		EventDelegate<Event>,
		EventBus,

		MainLoop

	>();
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
	auto & db{ Internals::get_singleton()->class_db };

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