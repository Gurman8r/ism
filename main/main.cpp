#include <main/main.hpp>
#include <core/os/main_loop.hpp>
#include <core/api/internals.hpp>
#include <core/register_core_types.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	uint32_t	Main::g_frame_count	{};
	uint32_t	Main::g_frame_index	{};
	int32_t		Main::g_iterating	{};

	RuntimeState * g_runtime{};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Error Main::setup(cstring exepath, int32_t argc, char * argv[])
	{
		get_os().initialize();

		g_runtime = memnew(RuntimeState);

		register_core_types();
		register_core_driver_types();

		register_core_settings();

		//preregister_module_types();
		//preregister_server_types();

		register_core_singletons();

		//register_server_types();
		//register_scene_types();

#if TOOLS_ENABLED
		//ClassDB::set_current_api(ClassDB::API_EDITOR);
		//register_editor_types();
		//ClassDB::set_current_api(ClassDB::API_CORE);
#endif

		//register_platform_apis();
		
		//register_module_types();
		//register_driver_types();

		//initialize_physics();
		//register_server_singletons();
		//ScriptServer::init_languages();

		get_os().set_cmdline(exepath, { argv, argv + argc });

		return Error_None;
	}

	bool Main::start()
	{
		//get_os().set_main_loop(memnew(SceneTree));

		//ResourceLoader::add_custom_loaders();
		//ResourceSaver::add_custom_savers();

		return true;
	}

	bool Main::iteration()
	{
		++g_iterating;

		bool should_exit{ false };

		++g_frame_count;

		--g_iterating;

		return should_exit;
	}

	void Main::cleanup()
	{
		//ResourceLoader::remove_custom_loaders();
		//ResourceSaver::remove_custom_savers();

		get_os().delete_main_loop();

		//ScriptServer::finish_languages();

#ifdef TOOLS_ENABLED
		EditorNode::unregister_editor_types();
#endif

		//unregister_driver_types();
		//unregister_module_types();
		//unregister_platform_apis();
		//unregister_scene_types();
		//unregister_server_types();

		get_os().finalize();

		memdelete_nonzero(g_runtime);

		unregister_core_driver_types();
		unregister_core_types();

		get_os().finalize_core();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}