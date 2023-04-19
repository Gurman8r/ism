#include <core/register_core_types.hpp>

#include <core/config/engine.hpp>
#include <core/config/project_settings.hpp>
#include <core/extension/extension_manager.hpp>
#include <core/input/input.hpp>
#include <core/object/script.hpp>
#include <core/os/main_loop.hpp>

#include <core/io/config_file.hpp>
#include <core/io/dir_access.hpp>
#include <core/io/event_system.hpp>
#include <core/io/file_access_memory.hpp>
#include <core/io/file_access_zip.hpp>
#include <core/io/image.hpp>
#include <core/io/lexer.hpp>

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
			ProjectSettings,

			Extension,
			ExtensionManager,

			ScriptServer,
			Script,
			ScriptLanguage,

			Event,
			EventListener,
			DummyListener,
			EventDelegate<Event>,
			EventBus,

			Input,
			InputEvent,
			InputMap,

			DirAccess,
			FileAccess,
			//FileAccessMemory,
			//FileAccessZip,
			ConfigFile,
			Image,

			Lexer,
			
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