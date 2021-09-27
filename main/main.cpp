/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <main/main.hpp>
#include <runtime/scene/scene_tree.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <core/register_core_types.hpp>
#include <drivers/register_driver_types.hpp>
#include <platform/register_platform_apis.hpp>
#include <runtime/register_runtime_types.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <runtime/renderer/rendering_context_default.hpp>

#if TOOLS_ENABLED
#include <editor/register_editor_types.hpp>
#endif

#if ISM_OS_WINDOWS
#include <platform/windows/display_context_windows.hpp>
#define DISPLAY_SERVER_DEFAULT DisplayContextWindows
#endif

#if OPENGL_ENABLED
#include <drivers/opengl/rendering_device_opengl.hpp>
#define RENDERING_DEVICE_DEFAULT RenderingDeviceOpenGL
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

using namespace ism;

MEMBER_IMPL(Main::g_frame_count) {};
MEMBER_IMPL(Main::g_frame_index) {};
MEMBER_IMPL(Main::g_iterating) {};

static Input * g_input{};
static Internals * g_internals{};
static DisplayContext * g_display_context{};
static RenderingContext * g_rendering_context{};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

Error Main::setup(cstring exepath, int32_t argc, char * argv[])
{
	get_os().initialize();
	
	g_internals = memnew(Internals);

	register_core_types();
	
	register_core_driver_types();
	
	register_core_settings();

	//preregister_module_types();
	
	//preregister_server_types();

	register_core_singletons();
	
	register_runtime_types();
	
#if TOOLS_ENABLED
	register_editor_types();
#endif

	register_platform_apis();
	
	//register_module_types();

	//g_camera_server = CameraServer::create();
	
	register_driver_types();
	
	//initialize_physics();
	
	//register_server_singletons();
	
	get_os().set_cmdline(exepath, { argv, argv + argc });

	g_input = memnew(Input);

	g_display_context = memnew(DISPLAY_SERVER_DEFAULT);

	g_rendering_context = memnew(RenderingContextDefault);

	return Error_None;
}

bool Main::start()
{
	get_os().set_main_loop(memnew(SceneTree));

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
	unregister_editor_types();
#endif

	unregister_driver_types();
	//unregister_module_types();
	unregister_platform_apis();
	unregister_server_types();

	//memdelete(g_audio_server);
	//memdelete(g_camera_server);

	get_os().finalize();

	g_rendering_context->finalize();
	memdelete(g_rendering_context);
	memdelete(g_display_context);
	
	memdelete(g_input);

	unregister_core_driver_types();
	unregister_core_types();

	memdelete(g_internals);
	get_os().finalize_core();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */