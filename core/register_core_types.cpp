#include <core/register_core_types.hpp>
#include <core/input/input.hpp>
#include <core/io/config_file.hpp>
#include <core/io/image.hpp>
#include <core/os/main_loop.hpp>

using namespace ism;

void ism::register_core_types()
{
	Internals::get_singleton()->install_class
	<
		CppFunctionObject, // <- cppfunction must go first
		Object,
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