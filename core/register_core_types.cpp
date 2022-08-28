#include <core/register_core_types.hpp>
#include <core/input/input.hpp>
#include <core/io/config_file.hpp>
#include <core/io/image.hpp>
#include <core/os/main_loop.hpp>

namespace ism
{
	void register_core_types()
	{
		Internals::get_singleton()->initialize_class
		<
			CppFunctionObject, // <- cppfunction must go first
			Object,
			TypeObject,
			IntObject,
			FloatObject,
			IteratorObject,
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
			//InputEvent,
			//InputMap,
	
			MainLoop
	
		>();
	}
	
	void register_core_driver_types()
	{
	}
	
	void register_core_settings()
	{
	}
	
	void register_core_singletons()
	{
	}
	
	void unregister_core_driver_types()
	{
	}
	
	void unregister_core_types()
	{
	}
}