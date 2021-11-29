#include <core/register_core_types.hpp>
#include <core/input/input.hpp>
#include <core/io/config_file.hpp>
#include <core/io/image.hpp>
#include <core/os/main_loop.hpp>

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

	SINGLETON(Internals)->bind_class
	<
		CppFunctionObject, // <- cppfunction MUST go first
		Object,
		TypeObject,
		IntObject,
		FloatObject,
		StringObject,
		ListObject,
		DictObject,
		CapsuleObject,
		FunctionObject,
		MethodObject,
		PropertyObject,
		ModuleObject,

		Resource,
		ConfigFile,
		Image,

		Input,
		InputEvent,
		InputMap,

		Event,
		EventHandler,
		DummyHandler,
		EventDelegate<void>,
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
	auto & db{ SINGLETON(Internals)->class_db };

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