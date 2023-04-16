#include <core/register_core_types.hpp>
#include <core/extension/extension_manager.hpp>
#include <core/input/input.hpp>
#include <core/io/config_file.hpp>
#include <core/io/image.hpp>
#include <core/io/file_access.hpp>
#include <core/os/main_loop.hpp>

namespace ism
{
	void register_core_types()
	{
		INITIALIZE_CLASS
		(
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
			FileAccess,
			ConfigFile,
			Image,

			Input,
			//InputEvent,
			//InputMap,

			MainLoop
		);
	}

	void register_core_settings()
	{
	}
	
	void register_core_extensions()
	{
		Extension::initialize_extensions();
		ExtensionManager::get_singleton()->load_extensions();
		ExtensionManager::get_singleton()->initialize_extensions(ExtensionInitializationLevel_Core);
	}
	
	void register_core_singletons()
	{
	}
	
	void unregister_core_types()
	{
	}

	void unregister_core_extensions()
	{
		ExtensionManager::get_singleton()->finalize_extensions(ExtensionInitializationLevel_Core);
	}
}