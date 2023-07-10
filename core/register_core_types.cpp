#include <core/register_core_types.hpp>
#include <core/io/resource_loader.hpp>
#include <core/io/resource_saver.hpp>
#include <core/io/image_library.hpp>

#include <core/config/engine.hpp>
#include <core/config/project_settings.hpp>
#include <core/extension/extension_manager.hpp>
#include <core/input/input.hpp>
#include <core/object/script.hpp>
#include <core/os/main_loop.hpp>
#include <core/io/config_file.hpp>
#include <core/io/dir.hpp>
#include <core/io/event_system.hpp>
#include <core/io/zip.hpp>
#include <core/io/lexer.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static Internals *			_internals{};
	static ResourceLoader *		_resource_loader{};
	static ResourceSaver *		_resource_saver{};
	static ExtensionManager *	_extension_manager{};
	static ScriptServer *		_script_server{};

	static Ref<ExtensionFormatLoader> extension_format_loader;
	static Ref<ImageFormatLoader> image_format_loader;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void register_core_types()
	{
		_internals = memnew(Internals);
		_resource_loader = memnew(ResourceLoader);
		_resource_saver = memnew(ResourceSaver);
		_extension_manager = memnew(ExtensionManager);
		_script_server = memnew(ScriptServer);

		_resource_loader->add_resource_format_loader(extension_format_loader.instance());
		_resource_loader->add_resource_format_loader(image_format_loader.instance());

		REGISTER_CLASS
		(
			CppFunctionObject, // <- cppfunction must go first
			Object,
			TypeObject,
			IntObject,
			FloatObject,
			ArrayObject,
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
			GenericTypeObject,

			Resource, ResourceLoader, ResourceSaver,

			ScriptServer, ScriptLanguage, Script, ScriptInstance, PlaceholderScriptInstance,

			Extension, ExtensionFormatLoader, ExtensionManager,

			Dir, File, PackFile, ZippedFile,

			Event, EventListener, DummyListener, EventDelegate<Event>, EventBus,

			Input, InputEvent, InputMap,
			
			ConfigFile,
			
			Image, ImageFormatLoader,

			Lexer,

			ProjectSettings,
			
			MainLoop
		);

	}

	void register_core_settings()
	{
	}
	
	void register_core_extensions()
	{
		Extension::initialize_interface();
		_extension_manager->load_extensions();
		_extension_manager->initialize_extensions(ExtensionInitializationLevel_Core);
	}
	
	void register_core_singletons()
	{
	}

	void unregister_core_extensions()
	{
		_extension_manager->finalize_extensions(ExtensionInitializationLevel_Core);
		memdelete(_script_server);
		memdelete(_extension_manager);
	}

	void unregister_core_types()
	{
		_resource_loader->remove_resource_format_loader(image_format_loader); image_format_loader = nullptr;
		_resource_loader->remove_resource_format_loader(extension_format_loader); extension_format_loader = nullptr;

		memdelete(_resource_loader);
		memdelete(_resource_saver);
		memdelete(_internals);
	}
}