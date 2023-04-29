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
#include <core/io/file_access_zip.hpp>
#include <core/io/image_library.hpp>
#include <core/io/lexer.hpp>

namespace ism
{
	static Ref<ImageFormatLoader> image_format_loader;

	void register_core_types()
	{
		REGISTER_CLASS
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
			ScriptInstance,
			ScriptLanguage,
			PlaceholderScriptInstance,

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
			FileAccessPack,
			FileAccessZip,
			ConfigFile,
			Image,
			ImageFormatLoader,
			Lexer,
			MainLoop
		);

		image_format_loader.instance(); RESOURCE_LOADER->add(image_format_loader);
	}

	void register_core_settings()
	{
	}
	
	void register_core_extensions()
	{
		Extension::initialize_extensions();
		EXTENSION_MANAGER->load_extensions();
		EXTENSION_MANAGER->initialize_extensions(ExtensionInitializationLevel_Core);
	}
	
	void register_core_singletons()
	{
	}
	
	void unregister_core_types()
	{
		RESOURCE_LOADER->remove(image_format_loader); image_format_loader = nullptr;
	}

	void unregister_core_extensions()
	{
		EXTENSION_MANAGER->finalize_extensions(ExtensionInitializationLevel_Core);
	}
}